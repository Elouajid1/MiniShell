NAME    = minishell
CC      = cc
CFLAGS	= -Wall -Wextra -Werror
LIBFT   = ./libft/libft.a
SRCS    = main.c utils.c tools.c signals.c free.c executor.c redirections.c env.c\
		setup.c pipes.c builtin1.c builtin2.c builtin3.c single_command_signal.c\
		error_message.c heredoc.c\
		parsing/expand.c parsing/free.c parsing/parse.c parsing/tokenize.c parsing/ft_split.c parsing/minishell.c
OBJS    = $(SRCS:.c=.o)
INCLUDE = -lreadline -lncurses #-g3 -fsanitize=address

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(INCLUDE) $(CFLAGS) $(OBJS) $(LIBFT) -o $(NAME)

%.o: %.c
	$(CC) $(CFLAGS) -I ./libft -I ./src -c $< -o $@

$(LIBFT):
	make -C ./libft

clean:
	rm -f $(OBJS)
	make -C ./libft clean

fclean: clean
	rm -f $(NAME)
	make -C ./libft fclean

re: fclean all

.PHONY: all clean fclean re

