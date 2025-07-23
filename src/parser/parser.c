#include "../../include/minishell.h"

t_cmd	*init_cmd(void)
{
	t_cmd	*cmd;

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->args = NULL;
	cmd->input_file = NULL;
	cmd->output_file = NULL;
	cmd->status = 0;
	cmd->append_mode = 0;
	cmd->heredoc_delimiter = NULL;
	cmd->next = NULL;
	return (cmd);
}

static char	**realloc_args(char **args, int count)
{
	char	**new_args;
	int		i;

	new_args = malloc(sizeof(char *) * (count + 2));
	if (!new_args)
		return (NULL);
	i = 0;
	if (args)
	{
		while (i < count)
		{
			new_args[i] = args[i];
			i++;
		}
		free(args);
	}
	new_args[count] = NULL;
	new_args[count + 1] = NULL;
	return (new_args);
}

int	add_arg_to_cmd(t_cmd *cmd, char *arg, t_env *env)
{
	char	*expanded_arg;
	int		count;

	count = 0;
	if (cmd->args)
	{
		while (cmd->args[count])
			count++;
	}
	cmd->args = realloc_args(cmd->args, count);
	if (!cmd->args)
		return (0);
	expanded_arg = expand_variable(arg, env);
	if (!expanded_arg)
		expanded_arg = ft_strdup("");
	cmd->args[count] = expanded_arg;
	return (1);
}

static int	process_word_token(t_cmd *cmd, t_token *current, t_env *env)
{
	if (!add_arg_to_cmd(cmd, current->value, env))
		return (0);
	return (1);
}

static int	process_redirection_token(t_cmd *cmd, t_token **current)
{
	if (!handle_redirection(cmd, current))
		return (0);
	return (1);
}

t_cmd	*parse_single_command(t_token **tokens, t_env *env)
{
	t_cmd	*cmd;
	t_token	*current;

	cmd = init_cmd();
	if (!cmd)
		return (NULL);
	current = *tokens;
	while (current && current->type != TOKEN_PIPE && current->type != TOKEN_EOF)
	{
		if (current->type == TOKEN_WORD)
		{
			if (!process_word_token(cmd, current, env))
				return (free_cmd(cmd), NULL);
		}
		else if (current->type >= TOKEN_REDIRECT_IN 
			&& current->type <= TOKEN_HEREDOC)
		{
			if (!process_redirection_token(cmd, &current))
				return (free_cmd(cmd), NULL);
		}
		else
			return (free_cmd(cmd), NULL);
		current = current->next;
	}
	*tokens = current;
	return (cmd);
}

static t_cmd	*link_commands(t_cmd *first_cmd, t_cmd *current_cmd, t_cmd *new_cmd)
{
	if (!first_cmd)
	{
		first_cmd = new_cmd;
		current_cmd = new_cmd;
	}
	else
	{
		current_cmd->next = new_cmd;
		current_cmd = new_cmd;
	}
	return (first_cmd);
}

t_cmd	*parse_tokens(t_token *tokens, t_env *env)
{
	t_cmd	*first_cmd;
	t_cmd	*current_cmd;
	t_token	*current_token;
	t_cmd	*new_cmd;

	if (!tokens)
		return (NULL);
	first_cmd = NULL;
	current_cmd = NULL;
	current_token = tokens;
	while (current_token && current_token->type != TOKEN_EOF)
	{
		new_cmd = parse_single_command(&current_token, env);
		if (!new_cmd)
			return (free_cmd_list(first_cmd), NULL);
		first_cmd = link_commands(first_cmd, current_cmd, new_cmd);
		if (!current_cmd)
			current_cmd = new_cmd;
		else
			current_cmd = current_cmd->next;
		if (current_token && current_token->type == TOKEN_PIPE)
			current_token = current_token->next;
	}
	return (first_cmd);
}

static int	validate_pipe_syntax(t_token *current)
{
	if (current->type == TOKEN_PIPE)
	{
		if (!current->next || current->next->type == TOKEN_EOF)
			return (0);
		if (current->next->type == TOKEN_PIPE)
			return (0);
	}
	return (1);
}

static int	validate_redirection_syntax(t_token *current)
{
	if (current->type >= TOKEN_REDIRECT_IN && current->type <= TOKEN_HEREDOC)
	{
		if (!current->next || current->next->type != TOKEN_WORD)
			return (0);
	}
	return (1);
}

int	validate_tokens(t_token *tokens)
{
	t_token	*current;

	current = tokens;
	if (!current || current->type == TOKEN_EOF)
		return (0);
	if (current->type == TOKEN_PIPE)
		return (0);
	while (current && current->type != TOKEN_EOF)
	{
		if (!validate_pipe_syntax(current))
			return (0);
		if (!validate_redirection_syntax(current))
			return (0);
		current = current->next;
	}
	return (1);
}
