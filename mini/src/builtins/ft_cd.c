#include "../../include/minishell.h"

void    ft_cd(t_cmd *cmd)
{
    char *path;
    char cwd[1024];
    char *oldpwd;

    path = cmd->args[1];

    // Handle 'cd' with no arguments or 'cd ~'
    if (!path || strcmp(path, "~") == 0)
        path = getenv("HOME");
    // Handle 'cd -'
    else if (strcmp(path, "-") == 0)
    {
        oldpwd = getenv("OLDPWD");
        if (!oldpwd)
        {
            printf("OLDPWD not set\n");
            return;
        }
        path = oldpwd;
        printf("%s\n", path); // Print the new path as bash does
    }

    // Save current directory for OLDPWD
    if (getcwd(cwd, sizeof(cwd)))
        setenv("OLDPWD", cwd, 1);

    if (chdir(path) != 0)
    {
        perror("cd");
        return;
    }

    // Update PWD
    if (getcwd(cwd, sizeof(cwd)))
        setenv("PWD", cwd, 1);
}