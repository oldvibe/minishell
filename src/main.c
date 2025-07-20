#include "../include/minishell.h"

static int	execute_builtin(t_cmd *cmd)
{
	if (!cmd->args || !cmd->args[0])
		return (0);
	if (ft_strcmp(cmd->args[0], "exit") == 0)
	{
		ft_exit(cmd->args);
		return (1);
	}
	if (ft_strcmp(cmd->args[0], "pwd") == 0)
	{
		ft_pwd(cmd->args);
		return (1);
	}
	if (ft_strcmp(cmd->args[0], "cd") == 0)
	{
		ft_cd(cmd);
		return (1);
	}
	if (ft_strcmp(cmd->args[0], "echo") == 0)
	{
		ft_echo(cmd);
		return (1);
	}
	return (0);
}

static void	execute_command(t_cmd *cmd, char **envp)
{
	pid_t	pid;

	if (!cmd->args || !cmd->args[0])
		return ;
	if (execute_builtin(cmd))
		return ;
	pid = fork();
	if (pid == 0)
	{
		execve(cmd->args[0], cmd->args, envp);
		perror(cmd->args[0]);
		exit(EXIT_FAILURE);
	}
	else if (pid > 0)
		waitpid(pid, NULL, 0);
	else
		perror("fork");
}

static int	process_input(char *input, t_env *env, char **envp)
{
	t_token	*tokens;
	t_cmd	*cmds;

	tokens = tokenize(input);
	if (!tokens || !check_syntax(tokens))
	{
		free_tokens(tokens);
		return (0);
	}
	if (!validate_tokens(tokens))
	{
		free_tokens(tokens);
		return (0);
	}
	cmds = parse_tokens(tokens, env);
	if (!cmds)
	{
		free_tokens(tokens);
		return (0);
	}
	if (cmds->args && cmds->args[0] && ft_strcmp(cmds->args[0], "exit") == 0)
	{
		free_cmd_list(cmds);
		free_tokens(tokens);
		return (1);
	}
	execute_command(cmds, envp);
	free_cmd_list(cmds);
	free_tokens(tokens);
	return (0);
}

int	main(int ac, char **av, char **envp)
{
	t_env	*env;
	char	*input;

	(void)ac;
	(void)av;
	env = dup_env(envp);
	if (!env)
		return (1);
	read_history(".minishell_history");
	print_prompt();
	while (1)
	{
		input = readline("minishell:~$ ");
		if (!input)
		{
			printf("exit\n");
			break ;
		}
		if (*input)
		{
			add_history(input);
			if (process_input(input, env, envp))
			{
				free(input);
				break ;
			}
		}
		free(input);
	}
	write_history(".minishell_history");
	free_env(env);
	return (0);
}
