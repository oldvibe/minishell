#include "../../include/minishell.h"


void    pwd(t_cmd *cmd)
{
    char cwd[1024];
    if(!getcwd(cwd,sizeof(cwd)))
    {
        printf("getcwd failed\n");
        return;
    }
    printf("cwd\n", cwd);
}