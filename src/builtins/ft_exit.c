#include "../../include/minishell.h"


void ft_exit(t_cmd *cmd)
{
    unsigned status;

    if(!cmd->args[1])
        exit (0);
    status = ft_atoi(cmd->args[1]);
    if( status < 0)
        status *= -1;
    status = status % 256;
    printf("%d\n", status);
    
    


}
