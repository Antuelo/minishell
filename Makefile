NAME = minishell
INCLUDES = -Iincludes -Ilibft
CC = gcc
CFLAGS = -Wall -Wextra -Werror -g
MAKEFLAGS += --no-print-directory

# Libft setup
LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

# Source files 'src/executions'
EXEC_SRCS = \
	main.c \
	src/executions/execute.c \
	src/executions/utils.c \
	src/executions/utils2.c \
	src/executions/free_exec.c \
	src/executions/execute_pipeline.c \
	src/executions/is_builtin_exec.c \
	src/executions/is_builtin_exec1.c \
	src/executions/is_builtin_exec2.c \
	src/executions/is_builtin_exec3.c \
	src/executions/is_builtin_exec4.c \
	src/executions/mini_envp.c

# Automatically find all .c files inside 'src/parsing'
PARSING_SRCS := $(shell find src/parsing -name '*.c')

SRCS = $(EXEC_SRCS) $(PARSING_SRCS)
OBJS = $(SRCS:.c=.o)

RM = rm -f

all: $(LIBFT) $(NAME)

$(LIBFT):
	@$(MAKE) -C $(LIBFT_DIR)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -lreadline -o $(NAME)
	@echo "✅ MINISHELL completed successfully!"

%.o: %.c
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	@$(MAKE) -C $(LIBFT_DIR) clean
	@$(RM) $(OBJS)
	@echo "🧹 MINISHELL files removed successfully!"

fclean: clean
	@$(MAKE) -C $(LIBFT_DIR) fclean
	@$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
