#include "../../include/minishell.h"

void    ft_cd(t_cmd *cmd)
{
    char *path;
    char cwd[1024];
    path = cmd->args[1];
    if (getcwd(cwd, sizeof(cwd)))
        printf("CURRENT IS : %s\n",cwd);
    
    if(!path)
    {
        path = getenv("HOME");
    }

    if(chdir(path) != 0)
    {
        printf("CHANGING DIRECTORY FAILED!!");
        return;
    }
    // if (getcwd(cwd, sizeof(cwd)))
    // {
    //     printf("CURRENT IS : %s\n",cwd);
    //     setenv("PWD", cwd, 1);

    // }
    

}