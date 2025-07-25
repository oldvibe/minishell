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
		cmd->status = ft_pwd(cmd->args);
		return (1);
	}
	if (ft_strcmp(cmd->args[0], "cd") == 0)
	{
		cmd->status = ft_cd(cmd);
		return (1);
	}
	if (ft_strcmp(cmd->args[0], "echo") == 0)
	{
		 cmd->status = ft_echo(cmd);
		return (1);
	}
	if (ft_strcmp(cmd->args[0], "env") == 0)
	{
		cmd->status = ft_env(cmd->args, env_list);
		return (1);
	}
	if (ft_strcmp(cmd->args[0], "export") == 0)
	{
		cmd->status = ft_export(cmd->args, &env_list);
		return (1);
	}
	return (0);
}



// single execute:

// fork for child process:
	// handle_redire < > >> if faild exit with 1 and print error using perror(file_name);
	// check if is NULL exit normal.
	// empty string (print command not found) and exit with 127;
	// get_path but check if command have / that mean user give you absolute path
	// check if absolute path is exist printf no such file or directory exit with 127.
	// check if is a directory (print is a directory) exit with 126.
	// give it to exceve if faild print permission denied exit with 126.
	// if user give you just relative path like (ls,cat...).
	// go find it absolute path in environment exectly varible call PATH.
	// if it not exist or empty string print command not found and exit 127.
	// split it with : and join /command to check if exist and make sure is executable if not just save one path that are exist.
	// if not exist print command not found and exit with 127;
	// return (path) execve and if faild print permission denied. 

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
			handle_redir(cmd,OPEN_RED, tmpin, tmpout);

		}
		execute_builtin(cmd,env_li);
		handle_redir(cmd,OPEN_RED, tmpin, tmpout);
		// save stdin --> dup(0);
		// save stdout ---> dup (1);
		// handle_redir ---> < > >>
		// execute_builtin;
		// return stdin dup2(old fd , new fd(0));
		// return stdout dup2(old fd, new fd (1));
		// close(tmp-fd--> stdin);
		// close (tmp-fd--> stdout);
	}
	else if(!cmd->next)
	{
		pid = fork();
		if(!pid)
		{
			exec_non_builtin(cmd, envp);
		}
		else
		wait();
	}
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
	execute_command(cmds, envp, env);
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
