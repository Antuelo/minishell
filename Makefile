NAME = minishell
INCLUDES = -Iincludes -Ilibft
CC = gcc
CFLAGS = -Wall -Wextra -Werror -g
MAKEFLAGS += --no-print-directory

# Libft setup
LIBFT_DIR = libft
LIBFT = $(LIBFT_DIR)/libft.a

# Source files 'src/executions'

SRCS := ./main.c $(shell find src/parsing src/execution -name '*.c')
OBJS := $(SRCS:.c=.o)

RM = rm -f

all: $(LIBFT) $(NAME)

$(LIBFT):
	@$(MAKE) -C $(LIBFT_DIR)

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -lreadline -o $(NAME)
	@echo "✅ MINISHELL compiled successfully!"
#	@valgrind --leak-check=full --show-leak-kinds=definite --log-file=valgrind_log.txt ./minishell
#	valgrind --leak-check=full --show-leak-kinds=all --track-fds=yes --track-origins=yes --suppressions=a.supp --log-file="valg.log" ./minishell

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

valgrind: re
	@echo "\n----->executings MINISHELL with Valgrind !!!"
	@echo "----->result in valgrind_log\n"
	valgrind --leak-check=full --show-leak-kinds=all --track-fds=yes --track-origins=yes --suppressions=a.supp --log-file="valg.log" ./minishell


.PHONY: all clean fclean re
