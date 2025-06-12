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
		cmds= parse_command(&token);
		if (cmds)
			execute_command(cmds, envp, env);
		free(input);
	}
	free_env(env);
	return (0);
}
