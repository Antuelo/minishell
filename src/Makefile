
NAME		= minishell
CC			= cc
CFLAGS		= -Wall -Wextra -Werror
READLINE	= -lreadline
INCLUDES	= -Iincludes

SRC			=	src/main.c \
				src/input.c \
				src/tokenizer.c \
				src/token_utils.c \

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
