/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mohchams <mohchams@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/27 19:32:04 by mohchams          #+#    #+#             */
/*   Updated: 2025/08/06 12:36:01 by mohchams         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <readline/readline.h>
# include <readline/history.h>
# include "libft/libft.h"
# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <string.h>
# include <stdio.h>
# include <errno.h>
# include <ctype.h> // Pour isspace

typedef enum e_token_type
{
    TOKEN_WORD,      /* Commande ou argument */
    TOKEN_PIPE,      /* | */
    TOKEN_REDIR_IN,  /* < */
    TOKEN_REDIR_OUT, /* > */
    TOKEN_APPEND,    /* >> */
    TOKEN_HEREDOC,   /* << */
    TOKEN_QUOTE,     /* ' ou " (à ajouter plus tard) */
    TOKEN_VAR        /* $VAR ou $? (à ajouter plus tard) */
} t_token_type;

typedef struct s_token
{
    char            *value; /* Valeur du token (ex. "ls", "|") */
    t_token_type    type;   /* Type du token */
    struct s_token  *next;  /* Pointeur vers le token suivant */
} t_token;

typedef struct s_redir
{
    t_token_type    type;      /* Type de redirection */
    char            *file;     /* Nom du fichier */
    int             fd;        /* File descriptor */
    struct s_redir  *next;     /* Redirection suivante */
} t_redir;

typedef struct s_cmd
{
    char            **args;    /* Arguments de la commande */
    char            *cmd_path; /* Chemin de la commande */
    t_redir         *redirs;   /* Liste des redirections */
    int             pipe_in;   /* FD d'entrée du pipe */
    int             pipe_out;  /* FD de sortie du pipe */
    pid_t           pid;       /* PID du processus */
    struct s_cmd    *next;     /* Commande suivante dans le pipeline */
} t_cmd;

typedef struct s_ast
{
    t_token_type    type;      /* Type du nœud AST */
    char            *value;    /* Valeur du nœud */
    struct s_ast    *left;     /* Enfant gauche */
    struct s_ast    *right;    /* Enfant droit */
    t_cmd           *cmd;      /* Commande associée */
} t_ast;

typedef struct s_env
{
    char            *key;      /* Nom de la variable */
    char            *value;    /* Valeur de la variable */
    struct s_env    *next;     /* Variable suivante */
} t_env;

typedef struct s_shell
{
    char            *input;    /* Input utilisateur */
    t_token         *tokens;   /* Liste des tokens */
    t_ast           *ast;      /* Arbre syntaxique */
    t_cmd           *cmds;     /* Liste des commandes */
    t_env           *env;      /* Variables d'environnement */
    int             exit_code; /* Code de sortie du dernier processus */
    int             pipe_fd[2];/* File descriptors pour les pipes */
} t_shell;

/* === LEXER === */
t_token *split_tokens(char *input);
int     handle_operator(char *input, int *i);
void    add_word_token(t_token **head, char *input, int *i);
void    add_token_back(t_token **head, t_token *new);
void    add_operator_token(t_token **head, int handle_return, char *input, int *i);
int     get_token_length_with_quotes(char *input, int *i);
int     check_quotes(char *input);
int     operator(char c);

/* === PARSER === */
t_ast   *parse_tokens(t_token *tokens);
t_ast   *parse_pipeline(t_token **tokens);
t_ast   *parse_command(t_token **tokens);
t_redir *parse_redirections(t_token **tokens);

/* === EXECUTOR === */
int     execute_ast(t_ast *ast, t_shell *shell);
int     execute_command(t_cmd *cmd, t_shell *shell);
int     execute_pipeline(t_cmd *cmds, t_shell *shell);
char    *find_command_path(char *cmd, t_env *env);

/* === BUILT-INS === */
int     builtin_echo(char **args);
int     builtin_cd(char **args, t_env *env);
int     builtin_pwd(void);
int     builtin_export(char **args, t_env **env);
int     builtin_unset(char **args, t_env **env);
int     builtin_env(t_env *env);
int     builtin_exit(char **args, t_shell *shell);
int     is_builtin(char *cmd);

/* === ENVIRONMENT === */
t_env   *init_env(char **envp);
char    *get_env_value(char *key, t_env *env);
int     set_env_value(char *key, char *value, t_env **env);
int     unset_env_value(char *key, t_env **env);
char    **env_to_array(t_env *env);

/* === UTILS === */
void    free_tokens(t_token *head);
void    free_ast(t_ast *ast);
void    free_cmds(t_cmd *cmds);
void    free_env(t_env *env);
void    free_shell(t_shell *shell);
void    error_exit(char *msg, int code);

/* === SIGNALS === */
void    setup_signals(void);
void    signal_handler(int sig);

#endif