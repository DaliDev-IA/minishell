/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pgavel <pgavel@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/23 11:50:44 by pgavel            #+#    #+#             */
/*   Updated: 2025/10/01 13:51:11 by pgavel           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define _GNU_SOURCE
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <string.h>
# include <sys/wait.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <fcntl.h>
# include <dirent.h>
# include <signal.h>
# include <errno.h>
# include <termios.h>
# include <readline/readline.h>
# include <readline/history.h>
# include "libft/libft.h"

# define PROMPT "minishell$ "
# define MAX_PATH 4096

extern int	g_signal_received;

typedef enum e_token_type
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIR_IN,
	TOKEN_REDIR_OUT,
	TOKEN_REDIR_APPEND,
	TOKEN_REDIR_HEREDOC,
	TOKEN_EOF
}	t_token_typ;

typedef struct s_token
{
	t_token_typ			type;
	char				*value;
	struct s_token		*next;
}	t_token;

typedef struct s_redir
{
	t_token_typ			type;
	char				*file;
	int					fd;
	struct s_redir		*next;
}	t_redir;

typedef struct s_cmd
{
	char			**args;
	t_redir			*redirs;
	struct s_cmd	*next;
}	t_cmd;

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

typedef struct s_shell
{
	t_env	*env;
	int		exit_status;
	int		stdin_backup;
	int		stdout_backup;
}	t_shell;

typedef struct s_expand_data
{
	char	*str;
	char	*result;
	int		*i;
	int		*j;
}	t_expand_data;

int			main(int argc, char **argv, char **envp);
void		process_input(t_shell *shell, char *input);
void		main_loop(t_shell *shell);

void		init_shell(t_shell *shell, char **envp);
void		cleanup_shell(t_shell *shell);

t_token		*tokenize(char *input);
t_token		*process_tokens(char *input, t_token *tokens);
void		free_tokens(t_token *tokens);
t_cmd		*parse_tokens(t_token *tokens);
t_token		*parse_single_cmd(t_token *tokens, t_cmd *cmd);
int			validate_syntax(t_token *tokens);
int			check_pipe_error(t_token_typ type);
t_token		*process_cmd(t_token *tokens, t_cmd *new_cmd, t_cmd **commands);
void		add_cmd_to_list(t_cmd **commands, t_cmd *new_cmd);
void		free_commands(t_cmd *cmds);

int			execute_commands(t_shell *shell, t_cmd *cmds);
int			execute_single_cmd(t_shell *shell, t_cmd *cmd);
int			execute_external_cmd(t_shell *shell, t_cmd *cmd);
int			execute_builtin(t_shell *shell, t_cmd *cmd);
int			run_builtin_command(t_shell *shell, t_cmd *cmd);
int			is_builtin(char *cmd);
int			check_file_error(char *cmd);

int			builtin_echo(char **args);
int			builtin_cd(t_shell *shell, char **args);
int			builtin_pwd(void);
int			builtin_export(t_shell *shell, char **args);
int			builtin_unset(t_shell *shell, char **args);
int			builtin_env(t_shell *shell);
int			builtin_exit(t_shell *shell, char **args);

char		*get_env_value(t_shell *shell, char *key);
void		set_env_value(t_shell *shell, char *key, char *value);
void		unset_env_value(t_shell *shell, char *key);
char		**env_to_array(t_shell *shell);

char		*expand_variables(t_shell *shell, char *str);

char		*process_single_arg(t_shell *shell, char *arg);
void		process_cmd_args(t_shell *shell, t_cmd *cmd);
char		**filter_empty_args(char **args);

int			has_quotes(char *str);
char		**split_unquoted_arg(char *arg, char *original);
int			count_total_args(char ***all_splits, int count);
void		build_new_args(char **new_args, char ***splits, int count);
void		free_splits(char ***splits, int count);
char		***prepare_splits(t_shell *shell, t_cmd *cmd, int *count);
void		rebuild_args(t_cmd *cmd, char ***all_splits, int count);

int			setup_redirections(t_redir *redirs);
void		restore_redirections(t_shell *shell);

void		setup_signals(void);
void		setup_child_signals(void);
void		signal_handler(int sig);

char		*ft_strdup(const char *s);
char		*ft_strjoin(char const *s1, char const *s2);
char		**ft_split(char const *s, char c);
int			ft_strcmp(const char *s1, const char *s2);
int			ft_strncmp(const char *s1, const char *s2, size_t n);
char		*ft_strchr(const char *s, int c);
size_t		ft_strlen(const char *s);
char		*ft_substr(char const *s, unsigned int start, size_t len);
void		ft_putstr_fd(char *s, int fd);
void		ft_putendl_fd(char *s, int fd);
int			ft_atoi(const char *str);
int			ft_isdigit(int c);
int			ft_isalpha(int c);
int			ft_isalnum(int c);

t_redir		*create_redir(t_token_typ type, char *file);
void		add_redir(t_redir **redirs, t_redir *new_redir);
int			count_args(t_token *tokens);
t_cmd		*create_cmd(void);

t_token		*create_token(t_token_typ type, char *value);
void		add_token(t_token **tokens, t_token *new_token);
int			skip_whitespace(char *input, int i);
int			handle_quotes(char *input, int start, char quote_char);
int			get_word_end(char *input, int start);
t_token_typ	get_redirect_type(char *input, int *i);

char		*find_command_path(char *cmd, t_shell *shell);
void		free_paths(char **paths);
char		*check_path_dirs(char **paths, char *cmd);
void		print_cmd_not_found(char *cmd, int path_exists);
void		free_env_array(char **env_array);
int			execute_child_process(t_cmd *cmd, char *cmd_path, char **env_array,
				t_shell *shell);
int			execute_pipeline(t_shell *shell, t_cmd *cmds);

void		execute_pipeline_builtin(t_shell *shell, t_cmd *current);
void		execute_pipeline_external(t_shell *shell, t_cmd *current);
void		handle_pipes(int *prev_fd, int *pipefd, t_cmd *current);
int			wait_for_children(pid_t last_pid);

int			is_valid_identifier(char *str);
void		parse_export_arg(char *arg, char **key, char **value);
int			handle_invalid_identifier(char *arg, char *key, char *value);

char		*get_var_name(char *str, int *i);
char		*get_var_value(t_shell *shell, char *var_name);
int			count_expanded_length(t_shell *shell, char *str);

int			handle_heredoc(char *delimiter);
int			handle_input_redir(char *file);
int			handle_output_redir(char *file, int append);
int			process_redir_type(t_redir *redir);

char		*preprocess_input(char *input);
t_token		*process_pipe_token(char *input, int *i);
char		*append_str(char *dst, const char *src);

char		*get_next_line(int fd);
char		*ft_itoa(int n);
void		ft_bzero(void *s, size_t n);

#endif