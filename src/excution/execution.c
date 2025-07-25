#include "../../include/minishell.h"

void exec_non_builtin(t_cmd *cmd, char **env)
{
    if(!cmd->args || !*cmd->args)
        exit(1);
    int tmpin;
    int tmpout;
    struct stat sb;
    
    char *cmd_n;
    int i;
    char **path;
    i = 0;
    tmpin = dup(0);
	tmpout = dup(1);
    handle_redir(cmd, OPEN_RED, tmpin,tmpout);
    if(cmd->args[0][0] == '/')
        check_absolu_path(cmd->args, env);
    else
    {
        cmd_n = ft_strjoin("/", cmd->args[0]);
        path = get_path(env);
        while(path[i++])
        {
            stat(ft_strjoin(path[i], cmd_n), &sb);
            if(!access(ft_strjoin(path[i], cmd_n), F_OK))
            {
                if (S_ISDIR(sb.st_mode))
                    continue;
                if(execve(ft_strjoin(path[i], cmd_n),cmd->args,env))
                {
                    perror("Permission denied\n");
                    exit(127);
                }
                
                return;
            }
        }
    }
    handle_redir(cmd, CLOSE_RED, tmpin,tmpout);

}
