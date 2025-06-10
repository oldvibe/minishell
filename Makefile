CC = cc
FLAGS = -Wall -Wextra -Werror

NAME =  minishell

SRCS =  src/main.c \
		src/parser/lexer.c \
		src/utils/ft_split.c \
		src/utils/ft_strchr.c \
		src/utils/ft_strcmp.c \
		src/utils/ft_strdup.c \
		src/utils/ft_strjoin.c \
		src/utils/ft_strlen.c \
		src/utils/ft_strncmp.c \
		src/utils/ft_strtok.c \
		src/utils/ft_substr.c \
		src/utils/ft_strncpy.c \
		src/dup_env.c

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -lreadline -o $(NAME)

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
