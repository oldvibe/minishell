#include "../include/minishell.h"

int main(int ac, char **av, char **envp)
{
	t_env	*env;
	t_token	*token;
	t_cmd   *cmds;
	char	*input;
	pid_t	pid;

	(void)ac;
	(void)av;
	env = dup_env(envp);

	if (!env)
		return (1);
	while(1)
	{
		input = readline("minishell:~$ ");
		if (!input)
		{
			printf("exit\n");
			break;
		}
		if (*input)
			add_history(input);
		token = tokenize(input);
		cmds = parse_command(&token);
		if(cmds->args[0])
		{
			if (ft_strcmp(cmds->args[0], "exit") == 0)
			{
				ft_exit(cmds->args);
			}
			pid = fork();
			if (pid == 0)
			{
				execve(cmds->args[0], cmds->args, envp);
				perror(cmds->args[0]);
				exit(EXIT_FAILURE);
			}
			else
				waitpid(pid, NULL, 0);
		}
		free(input);
	}
	free_env(env);
	return (0);
}
