#include "../../include/minishell.h"

int check_flag(char *arg)
{
    int i;
    i = 0;
    if(arg[i] != '-')
        return(0);
    while (arg[i + 1])
    {
        if(arg[i + 1] != 'n')
            return 0; 
        i++;      
    }
    return(1);
    
}
static void check_write(char *str, int exit_status)
{
    int i;

    i = 0;
    while(str[i])
    {
        if(str[i] == '$' && str[i + 1] == '?')
        {
            printf("%d", exit_status);
            i++;
        }
        else
            printf("%c", str[i]);
        i++;
    }
}
int ft_echo(t_cmd *cmds)
{
    int i;
    int new_line;

    i = 1;
    new_line = 0;
    if(check_flag(cmds->args[i]))
    {
        new_line = 1;
        i++;
    }
    while (cmds->args[i])
    {
        check_write(cmds->args[i], cmds->exit_status);
        if(cmds->args[i + 1])
            printf(" ");
        i++;
    }
    if(new_line == 0)
        printf("\n");
    
    return 0;

}
