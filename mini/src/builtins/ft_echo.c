#include "../../include/minishell.h"

int check_flag(char *arg)
{
    int i = 0;
    if (!arg || arg[i] != '-')
        return 0;
    i++;
    while (arg[i] == 'n')
        i++;
    return arg[i] == '\0';
}

void ft_echo(t_cmd *cmds)
{
    int i = 1;
    int new_line = 1;

    if (ft_strcmp(cmds->args[0], "echo") != 0)
        return;

    // Handle multiple -n flags
    while (cmds->args[i] && check_flag(cmds->args[i]))
    {
        new_line = 0;
        i++;
    }
    while (cmds->args[i])
    {
        printf("%s", cmds->args[i]);
        if (cmds->args[i + 1])
            printf(" ");
        i++;
    }
    if (new_line)
        printf("\n");
}