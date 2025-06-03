NAME        = minishell
CC          = cc
CFLAGS      = -Wall -Wextra -Werror
READLINE    = -lreadline
INCLUDES    = -Iincludes

SRC         := $(shell find src/parsing -name '*.c')
OBJ         := $(SRC:.c=.o)

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
