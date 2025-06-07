NAME = minishell
INCLUDES = -Iincludes -Ilibft
CC = gcc
CFLAGS = -Wall -Wextra -Werror -g
MAKEFLAGS += --no-print-directory

# Libft setup
LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

# Source files 'src/executions'

SRCS := ./main.c $(shell find src/parsing -name '*.c')
OBJS := $(SRCS:.c=.o)

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
