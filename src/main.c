#include "../include/minishell.h"

int	ft_builtin_exit(t_cmd *cmds)
{
	if (ft_strcmp(cmds->args[0], "exit") == 0)
	{
		write_history(".minishell_history");
		ft_exit(cmds->args);
		return (1);
	}
	return (0);
}

int	main(int ac, char **av, char **envp)
{
	t_env	*env;
	t_token	*token;
	t_cmd	*cmds;
	char	*input;
	pid_t	pid;

	(void)ac;
	(void)av;
	env = dup_env(envp);
	if (!env)
		return (1);
	read_history(".minishell_history");
	while (1)
	{
		input = readline("minishell:~$ ");
		if (!input)
		{
			printf("exit\n");
			break ;
		}
		if (*input)
			add_history(input);
		token = tokenize(input);
		if (!validate_tokens(token))
		{
			free_tokens(token);
			free(input);
			continue ;
		}
		cmds = parse_tokens(token, env);
		// hadi gha kantcheki wach command null bach mantihich fmachakil
		if (!cmds)
		{
			printf("Error: Failed to parse command\n");
			free_tokens(token);
			free(input);
			continue ;
		}
		if (cmds->args[0])
		{
			if (ft_builtin_exit(cmds))
				break;
			pid = fork();
			if (pid == 0)
			{
				// execve(cmds->args[0], cmds->args, envp);
				// perror(cmds->args[0]);
				// exit(EXIT_FAILURE);
			}
			else
				waitpid(pid, NULL, 0);
		}
		free_tokens(token);
		free(input);
	}
	write_history(".minishell_history");
	free_env(env);
	return (0);
}
