#include "../../include/execution.h"

int	is_builtin(t_cmd *cmds)
{
	if (!cmds || !cmds->args || !cmds->args[0])
		return (0);
	if (ft_strcmp(cmds->args[0], "exit") == 0)
		return (1);
	if (ft_strcmp(cmds->args[0], "pwd") == 0)
		return (1);
	if (ft_strcmp(cmds->args[0], "env") == 0)
		return (1);
	if (ft_strcmp(cmds->args[0], "cd") == 0)
		return (1);
	if (ft_strcmp(cmds->args[0], "export") == 0)
		return (1);
	if (ft_strcmp(cmds->args[0], "unset") == 0)
		return (1);
	if (ft_strcmp(cmds->args[0], "echo") == 0)
		return (1);
	return (0);
}

void	execute_builtin(t_cmd *cmds, t_env *env_list)
{
	if (ft_strcmp(cmds->args[0], "exti") == 0)
		ft_exit(cmds->args);
	else if (ft_strcmp(cmds->args[0], "pwd") == 0)
		ft_pwd();
	else if (ft_strcmp(cmds->args[0], "env") == 0)
		ft_env(cmds->args, env_list);
}

void	execute_command(t_cmd	*cmds, char	**envp, t_env *env_list)
{
	pid_t	pid;

	if (!cmds ||!cmds->args || !cmds->args[0])
		return ;
	if (is_builtin(cmds))
	{
		execute_builtin(cmds, env_list);
		return;
	}
	pid = fork();
	if (pid == 0)
	{
		execve(cmds->args[0], cmds->args, envp);
		perror(cmds->args[0]);
		exit(EXIT_FAILURE);
	}
	else if (pid > 0)
		waitpid(pid, NULL, 0);
	else
		perror("fork");
}


