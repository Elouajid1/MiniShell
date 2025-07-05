#include <unistd.h>
# include "./libft/ft_strdup.c"

int main(int ac, char **av, char **env)
{
    char    *tmp = ft_strdup("dodo");
    char *str = tmp;
    char *argv[] = {"./ls", 
            NULL};
    execve("./ls", argv, env);
}