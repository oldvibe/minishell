#include "../include/minishell.h"

int	execute_builtin(t_cmd *cmd, t_env *env_list)
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
	if (ft_strcmp(cmd->args[0], "export") == 0)
	{
		ft_export(cmd->args, &env_list);
		return (1);
	}
	if (ft_strcmp(cmd->args[0], "unset") == 0)
	{
		ft_unset(cmd->args, &env_list);
		return (1);
	}
	return (0);
}

static void	execute_command(t_cmd *cmd, char **envp, t_env *env_li)
{
	pid_t	pid;
	int tmpin;
	int tmpout;

	
	if(!cmd->next && is_built(cmd->args)) // check if it buitlin 
	{
		if(!cmd->input_file)
		{
			tmpin = dup(0);
			tmpout = dup(1);
			// handle_redir(cmd,OPEN_RED, tmpin, tmpout);

		}
		cmd->exit_status =  execute_builtin(cmd,env_li);
		// handle_redir(cmd,CLOSE_RED, tmpin, tmpout);

	}
	else if(!cmd->next)
	{
		pid = fork();
		
		if(!pid)
		{
			exec_non_builtin(cmd, envp);
		}
		else
		{
			waitpid(pid,&cmd->exit_status,0);
		}

	}
	else
		execute_multi(cmd,envp, env_li);
	
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
	t_gc	gc_instance = {0};

	(void)ac;
	(void)av;
	env = dup_env(envp);
	if (!env)
		return (1);
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
		// gc_free_all(&gc_instance); // Free all allocations after each input
	}
	free_env(env);
	return (0);
}
