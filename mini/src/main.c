#include "../include/minishell.h"

static int	execute_builtin(t_cmd *cmd, t_env *env_list)
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
	if (ft_strcmp(cmd->args[0], "env") == 0)
	{
		ft_env(cmd->args, env_list);
		return (1);
	}
	return (0);
}

static void	execute_command(t_cmd *cmd, char **envp, t_env *env_li)
{
	pid_t	pid;

	if (!cmd->args || !cmd->args[0])
		return ;
	if (execute_builtin(cmd, env_li))
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

static int	process_input(char *input, t_env *env, char **envp, t_gc *gc_instance)
{
	t_token	*tokens;
	t_cmd	*cmds;

	tokens = tokenize(input, gc_instance);
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
	cmds = parse_tokens(tokens, env, gc_instance);
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
	execute_command(cmds, envp, env);
	free_cmd_list(cmds);
	free_tokens(tokens);
	return (0);
}

int	main(int ac, char **av, char **envp)
{
	t_env	*env;
	char	*input;
	t_gc	gc_instance = {0}; // Initialize GC

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
			if (process_input(input, env, envp, &gc_instance))
			{
				free(input);
				break ;
			}
		}
		free(input);
		gc_free_all(&gc_instance); // Free all allocations after each input
	}
	write_history(".minishell_history");
	free_env(env);
	return (0);
}
