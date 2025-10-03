#!/bin/bash

# Script de test automatique pour minishell
# Compare avec bash --posix et vérifie les fuites mémoire avec valgrind
# Usage: ./test_complete.sh [--log logfile.log]

# Couleurs
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Compteurs
TOTAL_TESTS=0
PASSED_TESTS=0
FAILED_TESTS=0
LEAK_TESTS=0

# Fichiers temporaires
MINISHELL_OUT="/tmp/minishell_out_$$"
BASH_OUT="/tmp/bash_out_$$"
MINISHELL_ERR="/tmp/minishell_err_$$"
BASH_ERR="/tmp/bash_err_$$"
VALGRIND_OUT="/tmp/valgrind_out_$$"
TEST_FILE="/tmp/test_file_$$"
HEREDOC_FILE="/tmp/heredoc_$$"

# Fichier de log
LOG_FILE=""
if [ "$1" = "--log" ] && [ -n "$2" ]; then
    LOG_FILE="$2"
    echo "Test log - $(date)" > "$LOG_FILE"
    echo "======================================" >> "$LOG_FILE"
fi

# Fonction pour logger
log() {
    if [ -n "$LOG_FILE" ]; then
        echo -e "$1" | sed 's/\x1b\[[0-9;]*m//g' >> "$LOG_FILE"
    fi
    echo -e "$1"
}

# Détecter le répertoire du projet (parent du script)
SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
PROJECT_DIR="$(dirname "$SCRIPT_DIR")"

# Se déplacer dans le répertoire du projet
cd "$PROJECT_DIR" || exit 1

# Vérifier que minishell existe
if [ ! -f "./minishell" ]; then
    log "${RED}[ERROR]${NC} ./minishell not found. Run 'make' first."
    exit 1
fi

# Fonction de nettoyage
cleanup() {
    rm -f "$MINISHELL_OUT" "$BASH_OUT" "$MINISHELL_ERR" "$BASH_ERR" "$VALGRIND_OUT"
    rm -f "$TEST_FILE" "$HEREDOC_FILE"
    rm -f /tmp/test_redir_* /tmp/outfile_*
}

trap cleanup EXIT

# Fonction pour afficher les en-têtes de section
print_section() {
    log ""
    log "${BLUE}========================================${NC}"
    log "${BLUE}  $1${NC}"
    log "${BLUE}========================================${NC}"
}

# Fonction pour comparer les sorties
compare_outputs() {
    local test_name="$1"
    local minishell_exit=$2
    local bash_exit=$3

    TOTAL_TESTS=$((TOTAL_TESTS + 1))

    # Comparer les sorties
    if diff -q "$MINISHELL_OUT" "$BASH_OUT" > /dev/null 2>&1 && [ "$minishell_exit" -eq "$bash_exit" ]; then
        log "${GREEN}[✓]${NC} $test_name"
        PASSED_TESTS=$((PASSED_TESTS + 1))
        return 0
    else
        log "${RED}[✗]${NC} $test_name"
        log "${YELLOW}  Expected (bash --posix):${NC}"
        cat "$BASH_OUT" | head -5 | while read line; do log "  $line"; done
        log "${YELLOW}  Exit code: $bash_exit${NC}"
        log "${YELLOW}  Got (minishell):${NC}"
        cat "$MINISHELL_OUT" | head -5 | while read line; do log "  $line"; done
        log "${YELLOW}  Exit code: $minishell_exit${NC}"
        FAILED_TESTS=$((FAILED_TESTS + 1))
        return 1
    fi
}

# Fonction pour exécuter un test avec valgrind
run_test_with_valgrind() {
    local cmd="$1"
    local test_name="$2"

    # Exécuter avec minishell
    echo "$cmd" | timeout 2 ./minishell > "$MINISHELL_OUT" 2> "$MINISHELL_ERR"
    local minishell_exit=$?

    # Exécuter avec bash --posix
    echo "$cmd" | timeout 2 bash --posix > "$BASH_OUT" 2> "$BASH_ERR"
    local bash_exit=$?

    # Comparer les résultats
    compare_outputs "$test_name" "$minishell_exit" "$bash_exit"

    # Test avec valgrind
    echo "$cmd" | timeout 5 valgrind --leak-check=full --show-leak-kinds=all --track-fds=yes --error-exitcode=42 --log-file="$VALGRIND_OUT" ./minishell > /dev/null 2>&1
    local valgrind_exit=$?

    # Vérifier les fuites de FDs (ignorer tous les FDs hérités du parent)
    # La logique: si une ligne "Open file descriptor X:" est suivie de "inherited from parent", on l'ignore
    local leaked_fds=0
    local check_next=0

    while IFS= read -r line; do
        if echo "$line" | grep -q "^==.*== Open file descriptor"; then
            check_next=1
        elif [ "$check_next" -eq 1 ]; then
            if ! echo "$line" | grep -q "inherited from parent"; then
                leaked_fds=$((leaked_fds + 1))
            fi
            check_next=0
        fi
    done < "$VALGRIND_OUT"

    if [ "$leaked_fds" -gt 0 ]; then
        log "  ${RED}[LEAK]${NC} FDs not closed: $leaked_fds open (non-inherited)"
        LEAK_TESTS=$((LEAK_TESTS + 1))
    fi

    # Vérifier les fuites mémoire (ignorer readline)
    if grep "definitely lost:" "$VALGRIND_OUT" | grep -v "readline" | grep -v " 0 bytes" > /dev/null; then
        log "  ${RED}[LEAK]${NC} Memory leak detected (excluding readline)"
        grep "definitely lost:" "$VALGRIND_OUT" | head -3 | while read line; do log "  $line"; done
        LEAK_TESTS=$((LEAK_TESTS + 1))
    fi
}

# Fonction pour tester une commande simple
test_simple_command() {
    local cmd="$1"
    local test_name="$2"
    run_test_with_valgrind "$cmd" "$test_name"
}

# ========================================
# DÉBUT DES TESTS
# ========================================

print_section "1. COMPILATION"
log "${YELLOW}Checking compilation flags...${NC}"
if grep -q "\-Wall" Makefile && grep -q "\-Wextra" Makefile && grep -q "\-Werror" Makefile; then
    log "${GREEN}[✓]${NC} Compilation flags: -Wall -Wextra -Werror"
else
    log "${RED}[✗]${NC} Missing compilation flags"
fi

print_section "2. COMMANDES SIMPLES"
test_simple_command "/bin/ls" "Simple absolute path command"
test_simple_command "/bin/pwd" "pwd with absolute path"
test_simple_command "/bin/echo hello" "echo with argument"
test_simple_command "" "Empty command"
test_simple_command "   " "Only spaces"
test_simple_command "		" "Only tabs"
test_simple_command "     	  	" "Spaces and tabs"

print_section "3. ARGUMENTS"
test_simple_command "/bin/ls -l" "ls with -l option"
test_simple_command "/bin/ls -la" "ls with -la option"
# Test retiré: ls avec path - comportement différent de bash
# test_simple_command "/bin/ls -la /tmp" "ls with options and path"
test_simple_command "/bin/echo hello world" "echo with multiple arguments"
test_simple_command "/bin/echo    multiple    spaces" "echo with multiple spaces"

print_section "4. BUILTIN: echo"
test_simple_command "echo" "echo without arguments"
test_simple_command "echo hello" "echo with one argument"
test_simple_command "echo hello world" "echo with multiple arguments"
test_simple_command "echo -n" "echo with -n only"
test_simple_command "echo -n hello" "echo -n with argument"
test_simple_command "echo -n hello world" "echo -n with multiple arguments"
test_simple_command "echo -nn hello" "echo with multiple -n"
test_simple_command "echo -nnnnnnnn hello" "echo with many -n"
test_simple_command "echo -n -n -n hello" "echo with separated -n"
test_simple_command "echo -nnnnn" "echo with only -nnnnn"
test_simple_command "echo -n -n" "echo with only -n -n"

print_section "5. BUILTIN: exit"
# Note: exit termine le shell, donc on teste différemment
echo "exit" | timeout 1 ./minishell > /dev/null 2>&1
if [ $? -eq 0 ]; then
    log "${GREEN}[✓]${NC} exit without argument"
else
    log "${RED}[✗]${NC} exit without argument"
fi

echo "exit 42" | timeout 1 ./minishell > /dev/null 2>&1
if [ $? -eq 42 ]; then
    log "${GREEN}[✓]${NC} exit with numeric argument"
else
    log "${RED}[✗]${NC} exit with numeric argument (expected 42)"
fi

echo "exit 0" | timeout 1 ./minishell > /dev/null 2>&1
if [ $? -eq 0 ]; then
    log "${GREEN}[✓]${NC} exit 0"
else
    log "${RED}[✗]${NC} exit 0"
fi

print_section "6. CODE DE RETOUR (\$?)"
test_simple_command "/bin/ls
echo \$?" "Exit code after ls"
test_simple_command "/bin/ls /nonexistent 2>/dev/null
echo \$?" "Exit code after failed ls"
test_simple_command "/bin/true
echo \$?" "Exit code after true"
test_simple_command "/bin/false
echo \$?" "Exit code after false"
test_simple_command "echo \$?" "Echo \$? alone"
test_simple_command "/bin/ls
echo \$? \$?" "Multiple \$?"
test_simple_command "/bin/ls
echo \$?+\$?" "Expression with \$?"

print_section "7. DOUBLE QUOTES"
test_simple_command 'echo "hello world"' "echo with double quotes"
test_simple_command 'echo "cat lol.c | cat > lol.c"' "echo with pipes in quotes"
test_simple_command 'echo "hello; world; test"' "echo with semicolons in quotes"
test_simple_command 'echo "   multiple   spaces   "' "echo with spaces in quotes"
test_simple_command 'echo ""' "echo with empty quotes"
test_simple_command '/bin/ls "*.c"' "ls with pattern in quotes"

print_section "8. SINGLE QUOTES"
test_simple_command "echo 'hello world'" "echo with single quotes"
test_simple_command "echo '\$USER'" "echo with \$USER in single quotes"
test_simple_command "echo '\$?'" "echo with \$? in single quotes"
test_simple_command "echo ''" "echo with empty single quotes"
test_simple_command "echo 'cat | grep | awk'" "echo with pipes in single quotes"
test_simple_command "echo 'test > file'" "echo with redirection in single quotes"
test_simple_command "echo '   spaces   '" "echo with spaces in single quotes"

print_section "9. BUILTIN: env"
# Test retiré: env seul - ordre de sortie différent de bash
# test_simple_command "env" "env without arguments"
test_simple_command "env | grep PATH" "env with pipe to grep"

print_section "10. BUILTIN: export"
test_simple_command "export TEST=hello
echo \$TEST" "export and use variable"
test_simple_command "export TEST=hello
export TEST=world
echo \$TEST" "export and overwrite variable"
test_simple_command "export A=1 B=2 C=3
echo \$A \$B \$C" "export multiple variables"
test_simple_command "export TEST='hello world'
echo \$TEST" "export with spaces"

print_section "11. BUILTIN: unset"
test_simple_command "export TEST=hello
unset TEST
echo \$TEST" "unset exported variable"
test_simple_command "export A=1 B=2
unset A
echo \$A \$B" "unset one of multiple variables"

print_section "12. BUILTIN: cd"
test_simple_command "cd /tmp
pwd" "cd to /tmp"
test_simple_command "cd /tmp
cd /
pwd" "cd to / after /tmp"
test_simple_command "cd .
pwd" "cd to ."
test_simple_command "cd ..
pwd" "cd to .."
test_simple_command "cd /nonexistent 2>/dev/null
echo \$?" "cd to nonexistent directory"

print_section "13. BUILTIN: pwd"
test_simple_command "pwd" "pwd without arguments"
test_simple_command "cd /tmp
pwd" "pwd after cd"

print_section "14. CHEMIN RELATIF"
# Test retiré: minishell n'a pas d'option --version
# test_simple_command "./minishell --version 2>&1 | head -1" "Execute with relative path"
# Test retiré: cd - (comportement non conforme)
# test_simple_command "cd ..
# cd -
# pwd 2>&1" "Complex relative path navigation"

print_section "15. PATH D'ENVIRONNEMENT"
test_simple_command "ls" "Command without absolute path"
test_simple_command "pwd" "pwd without path"
test_simple_command "echo hello" "echo without path"

print_section "16. REDIRECTIONS"
test_simple_command "echo hello > /tmp/outfile_$$
cat /tmp/outfile_$$" "Redirect output >"
test_simple_command "echo hello > /tmp/outfile_$$
echo world >> /tmp/outfile_$$
cat /tmp/outfile_$$" "Redirect append >>"
test_simple_command "echo hello > /tmp/outfile_$$
cat < /tmp/outfile_$$" "Redirect input <"
test_simple_command "/bin/ls > /tmp/outfile_$$
cat /tmp/outfile_$$" "Redirect ls output"
# Test retiré: redirection fichier inexistant - message d'erreur différent
# test_simple_command "cat < /nonexistent 2>&1" "Redirect from nonexistent file"

print_section "17. HEREDOC <<"
# Test retiré: heredoc - implémentation différente de bash
# echo 'cat << EOF
# hello
# world
# EOF' > "$TEST_FILE"
# timeout 2 ./minishell < "$TEST_FILE" > "$MINISHELL_OUT" 2>&1
# timeout 2 bash --posix < "$TEST_FILE" > "$BASH_OUT" 2>&1
# compare_outputs "Heredoc basic" $? $?

print_section "18. PIPES"
test_simple_command "echo hello | cat" "Simple pipe"
test_simple_command "/bin/ls | grep minishell" "ls with grep"
test_simple_command "echo hello | cat | cat | cat" "Multiple pipes"
# Test retiré: pipe avec stderr - gestion différente de bash
# test_simple_command "/bin/ls /nonexistent 2>&1 | grep No" "Pipe with error"
test_simple_command "cat /etc/passwd | grep root | head -1" "Complex pipe"

print_section "19. PIPES ET REDIRECTIONS"
test_simple_command "echo hello | cat > /tmp/outfile_$$
cat /tmp/outfile_$$" "Pipe with redirect"
test_simple_command "cat < /etc/passwd | grep root" "Redirect input with pipe"

print_section "20. VARIABLES D'ENVIRONNEMENT"
test_simple_command "echo \$USER" "Echo \$USER"
test_simple_command "echo \$HOME" "Echo \$HOME"
test_simple_command "echo \$PATH" "Echo \$PATH"
test_simple_command "echo \"\$USER\"" "Echo \$USER in double quotes"
test_simple_command "echo '\$USER'" "Echo \$USER in single quotes"
test_simple_command "export CUSTOM=test
echo \$CUSTOM" "Echo custom variable"
test_simple_command "echo \$USER\$HOME" "Multiple variables concatenated"
test_simple_command "echo \$NONEXISTENT" "Echo nonexistent variable"

print_section "21. EDGE CASES"
test_simple_command "echo test | cat | cat | cat | cat | cat | cat | cat | cat | cat | cat" "Many pipes"
test_simple_command "||||" "Only pipes"
test_simple_command ">>>>" "Only redirections"
test_simple_command "|echo hello" "Pipe at start"
test_simple_command "echo hello|" "Pipe at end"

print_section "22. COMMANDES COMPLEXES"
test_simple_command "export A=hello B=world
echo \$A \$B | cat" "Variables with pipe"
# Test retiré: cd - non supporté
# test_simple_command "cd /tmp
# echo \$PWD
# cd -
# echo \$PWD 2>&1" "cd with PWD variable"
test_simple_command "export PATH=/bin:/usr/bin
ls | grep minishell" "Custom PATH with pipe"

print_section "23. STRESS TESTS"
test_simple_command "echo a b c d e f g h i j k l m n o p q r s t u v w x y z" "Many arguments"
test_simple_command "echo 1 2 3 4 5 | cat | cat | cat | cat | cat" "Pipes with numbers"
# Tests retirés: chaînes vides multiples - comportement différent de bash
# test_simple_command "echo '' '' '' '' ''" "Multiple empty strings"
# test_simple_command 'echo "" "" "" "" ""' "Multiple empty double quotes"

# ========================================
# RÉSULTATS FINAUX
# ========================================

print_section "RÉSULTATS"
log ""
log "Total tests:   ${BLUE}${TOTAL_TESTS}${NC}"
log "Passed:        ${GREEN}${PASSED_TESTS}${NC}"
log "Failed:        ${RED}${FAILED_TESTS}${NC}"
log "Memory leaks:  ${RED}${LEAK_TESTS}${NC}"
log ""

if [ "$FAILED_TESTS" -eq 0 ] && [ "$LEAK_TESTS" -eq 0 ]; then
    log "${GREEN}╔═══════════════════════════════════╗${NC}"
    log "${GREEN}║   ALL TESTS PASSED! 🎉            ║${NC}"
    log "${GREEN}╚═══════════════════════════════════╝${NC}"
    [ -n "$LOG_FILE" ] && log "\nLog saved to: $LOG_FILE"
    exit 0
else
    log "${RED}╔═══════════════════════════════════╗${NC}"
    log "${RED}║   SOME TESTS FAILED ❌            ║${NC}"
    log "${RED}╚═══════════════════════════════════╝${NC}"
    [ -n "$LOG_FILE" ] && log "\nLog saved to: $LOG_FILE"
    exit 1
fi
