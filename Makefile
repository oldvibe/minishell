CC = cc
FLAGS = -Wall -Wextra -Werror

NAME =  minishell
PRINTF = src/utils/ft_printf/printf.a

SRCS =  src/main.c \
		src/ft_parser/lexer.c \
		src/ft_parser/parser.c \
		src/execution/execution.c \
		src/builtins/builtins.c \
		src/builtins/ft_cd.c \
		src/builtins/ft_echo.c \
		src/builtins/ft_exit.c \
		src/builtins/ft_pwd.c \
		src/builtins/ft_env.c \
		src/utils/ft_atoi.c \
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
		src/utils/ft_memcpy.c \
		src/utils/ft_memset.c \
		src/utils/ft_bzero.c \
		src/utils/ft_calloc.c \
		src/utils/ft_realloc.c \
		src/dup_env.c

OBJS = $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	make -C src/utils/ft_printf
	$(CC) $(CFLAGS) $(OBJS) $(PRINTF) -lreadline -o $(NAME)

clean:
	make -C src/utils/ft_printf clean
	rm -f $(OBJS)

fclean: clean
	make -C src/utils/ft_printf fclean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
