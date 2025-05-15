
NAME		= minishell
CC			= cc
CFLAGS		= -Wall -Wextra -Werror
READLINE	= -lreadline
INCLUDES	= -Iincludes

SRC			=	src/parsing/main.c \
				src/parsing/input.c \
				src/parsing/tokenizer.c \
				src/parsing/token_utils.c \
				src/parsing/parser.c \
				src/parsing/redirections.c \

OBJ			= $(SRC:.c=.o)


all: $(NAME)

$(NAME): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME) $(READLINE)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@ $(INCLUDES)

clean:
	rm -f $(OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
