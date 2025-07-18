#include "../../include/minishell.h"

static int	set_input_file(t_cmd *cmd, char *filename)
{
	if (cmd->input_file)
		free(cmd->input_file);
	cmd->input_file = ft_strdup(filename);
	return (cmd->input_file != NULL);
}

static int	set_output_file(t_cmd *cmd, char *filename, int append)
{
	if (cmd->output_file)
		free(cmd->output_file);
	cmd->output_file = ft_strdup(filename);
	cmd->append_mode = append;
	return (cmd->output_file != NULL);
}

static int	set_heredoc_delimiter(t_cmd *cmd, char *delimiter)
{
	if (cmd->heredoc_delimiter)
		free(cmd->heredoc_delimiter);
	cmd->heredoc_delimiter = ft_strdup(delimiter);
	return (cmd->heredoc_delimiter != NULL);
}

int	handle_redirection(t_cmd *cmd, t_token **my_token)
{
	t_token	*token;
	char	*filename;

	token = *my_token;
	if (!token->next || token->next->type != TOKEN_WORD)
		return (0);
	filename = token->next->value;
	if (token->type == TOKEN_REDIRECT_IN)
	{
		if (!set_input_file(cmd, filename))
			return (0);
	}
	else if (token->type == TOKEN_REDIRECT_OUT)
	{
		if (!set_output_file(cmd, filename, 0))
			return (0);
	}
	else if (token->type == TOKEN_REDIRECT_APPEND)
	{
		if (!set_output_file(cmd, filename, 1))
			return (0);
	}
	else if (token->type == TOKEN_HEREDOC)
	{
		if (!set_heredoc_delimiter(cmd, filename))
			return (0);
	}
	*my_token = token->next;
	return (1);
}