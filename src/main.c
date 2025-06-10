#include "../include/minishell.h"

int main(int ac, char **av, char **envp)
{
	t_env	*env;
	t_token	*token;
	char	**cmds;
	char	*input;

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
		if (token)
		{
			// cause segfault for now because of cmds 
			// if (cmds && cmds[0] && ft_strcmp(cmds[0], "exit") == 0)
			// {
			// 	ft_exit(cmds);
			// }
			//print_tokens(token);
			free_tokens(token);
		}
		free(input);
	}
	free_env(env);
	return (0);
}
