NAME = minishell

SRCDIR = src
OBJDIR = obj
LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

SOURCES = 	main/main.c \
			main/main_utils.c \
			environment/environment.c \
			execution/executor.c \
			execution/executor_cmd.c \
			execution/executor_external.c \
			execution/executor_utils.c \
			execution/executor_pipeline.c \
			execution/executor_pipeline_utils.c \
			execution/redirections.c \
			execution/redirections_utils.c \
			expansion/expand.c \
			expansion/expand_utils.c \
			init/shell_init.c \
			init/signals.c \
			parsing/tokenizer.c \
			parsing/tokenizer_main.c \
			parsing/tokenizer_utils.c \
			parsing/tokenizer_helper.c \
			parsing/parser.c \
			parsing/parser_main.c \
			parsing/parser_utils.c \
			parsing/parser_validation.c \
			parsing/parser_cleanup.c \
			utils/process_args.c \
			utils/word_split.c \
			utils/word_split_utils.c \
			utils/filter_args.c \
			utils/utils.c \
			builtins/builtins.c \
			builtins/execute_builtin_helpers.c \
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