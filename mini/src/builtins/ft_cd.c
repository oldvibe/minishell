#include "../../include/minishell.h"

int    ft_cd(t_cmd *cmd)
{
    char *path;
    char cwd[1024];
    path = cmd->args[1];
    getcwd(cwd, sizeof(cwd));
    
    if(!path)
    {
        path = getenv("HOME");
    }

    if(chdir(path) != 0)
    {
        printf("CHANGING DIRECTORY FAILED!!");
        return 1;
    }
    return 0;
    // if (getcwd(cwd, sizeof(cwd)))
    // {
    //     printf("CURRENT IS : %s\n",cwd);
    //     setenv("PWD", cwd, 1);

    // }
    

}