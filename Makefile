NAME = minishell

SRCDIR = src
OBJDIR = obj
LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

SOURCES = 	main/main.c \
			main/main_loop.c \
			environment/environment.c \
			execution/arg_processor.c \
			src/execution/args_expansion.c \
			src/execution/args_filter.c \
			src/execution/args_rebuild.c \
			execution/executor.c \
			execution/executor_external.c \
			execution/executor_utils.c \
			execution/executor_path.c \
			execution/executor_pipeline.c \
			execution/executor_pipeline_utils.c \
			execution/redirections.c \
			execution/redirections_utils.c \
			expansion/expand.c \
			expansion/expand_utils.c \
			init/shell_init.c \
			init/signals.c \
			parsing/tokenizer.c \
			parsing/tokenizer_process.c \
			parsing/tokenizer_utils.c \
			parsing/tokenizer_parse.c \
			parsing/parser.c \
			parsing/parser_command.c \
			parsing/parser_utils.c \
			parsing/parser_memory.c \
			builtins/builtins.c \
			builtins/builtin_utils.c \
			builtins/builtin_echo.c \
			builtins/builtin_cd.c \
			builtins/builtin_pwd.c \
			builtins/builtin_export.c \
			builtins/builtin_export_utils.c \
			builtins/builtin_unset.c \
			builtins/builtin_env.c \
			builtins/builtin_exit.c

SRCS = $(addprefix $(SRCDIR)/, $(SOURCES))
OBJS = $(addprefix $(OBJDIR)/, $(notdir $(SOURCES:.c=.o)))

CC = cc
CFLAGS = -Wall -Wextra -Werror -g -I$(LIBFT_DIR)
LDFLAGS = -lreadline

all: $(NAME)

$(LIBFT):
	$(MAKE) -C $(LIBFT_DIR)

$(NAME): $(LIBFT) $(OBJS)
	$(CC) $(OBJS) $(LIBFT) $(LDFLAGS) -o $(NAME)

$(OBJDIR)/%.o: $(SRCDIR)/*/%.c minishell.h | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.c minishell.h | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)

clean:
	rm -f $(OBJS)
	$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	rm -f $(NAME)
	$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re