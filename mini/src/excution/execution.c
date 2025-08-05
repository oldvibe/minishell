#include "../../include/minishell.h"

void exec_non_builtin(t_cmd *cmd, char **env)
{
    if(!cmd->args || !*cmd->args)
    exit(1);
    // int tmpin;
    // int tmpout;
    struct stat sb;
    
    char *cmd_n;
    int i;
    char **path;
    i = 0;
    // tmpin = dup(0);
	// tmpout = dup(1);
    // handle_redir(cmd, OPEN_RED, tmpin,tmpout);
    if(cmd->args[0][0] == '/')
    {

        check_absolu_path(cmd->args, env);
    }
    else
    {
        cmd_n = ft_strjoin("/", cmd->args[0]);
        path = get_path(env);
        while(path[i])
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
                
            }
            i++;
        }
        perror(cmd->args[0]);
    }
    // handle_redir(cmd, CLOSE_RED, tmpin,tmpout);

}




void    execute_multi(t_cmd *cmd,char **envp, t_env *env_li)
{
    int status;
    int pipe_fd[2];
    int prev_fd;
    int i = -1;
    int ncmd = 0;
    t_cmd *temp = cmd;
    while (temp)
    {
        ncmd++;
        temp = temp->next;
    }

    int *pid = malloc(ncmd * 4);

    prev_fd = -1;
    while (cmd)
    {
        if(cmd->next)
            if(pipe(pipe_fd) == -1)
                perror("PIPE");
        pid[++i] = fork();
        if (pid[i] == -1)
            perror("FORK");
        else if (pid[i] == 0)
        {
            if(prev_fd != -1)
            {
                dup2(prev_fd, 0);
                close(prev_fd);
            }
            if(cmd->next)
                dup2(pipe_fd[1], 1);
            close(pipe_fd[0]);
            close(pipe_fd[1]);
            if(is_built(cmd->args))
            {
                cmd->exit_status =  execute_builtin(cmd,env_li);
                exit(cmd->exit_status);
            }
            else
            {
                exec_non_builtin(cmd, envp);
                exit (0);
            }
                
            
        }
        else
        {
            if(prev_fd != -1)
                close(prev_fd);
            if(cmd->next)
                prev_fd = pipe_fd[0];
            close(pipe_fd[1]);
            cmd = cmd->next;
        }
        
    }
    
    if(prev_fd != -1)
        close(prev_fd);
    i = 0;
    while (i < ncmd)
    {
        waitpid(pid[i++], &status, 0);
        if (WIFEXITED(status))
        {
            WEXITSTATUS(status);
        }    
    }
}
