NAME    = minishell
CC      = cc
LIBFT   = ./libft/libft.a
CFLAGS	= -Werror -Wextra -Wall
SRCS    = execution/main.c execution/utils.c execution/tools.c execution/signals.c execution/free.c execution/executor.c execution/redirections.c execution/env.c execution/export_vars.c execution/utils1.c execution/error_message2.c\
		execution/setup.c execution/pipes.c execution/builtin1.c execution/builtin2.c execution/builtin3.c execution/single_command_signal.c execution/helper_functions.c execution/path_functions.c\
		execution/error_message.c execution/heredoc.c execution/env_i.c execution/heredoc_signals.c execution/heredoc_utils.c execution/cpy.c execution/error_message1.c\
		parsing/expand.c parsing/free.c parsing/parse.c parsing/tokenize.c parsing/ft_split.c parsing/utils3.c parsing/utils.c execution/utils2.c\
		parsing/init.c parsing/utils2.c parsing/tools.c
OBJS    = $(SRCS:.c=.o)
INCLUDE = -lreadline -lncurses -lhistory

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -o $(NAME) $(INCLUDE)

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

