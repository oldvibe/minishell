#include "../../include/minishell.h"

int	handle_redirection(t_cmd *cmd, t_token **myToken)
{
	t_token	*token;
	char	*filename;

	token = *myToken;
	if (!token->next || token->next->type != TOKEN_WORD)
	{
		printf("Error: Expected filename after redirection\n");
		return (0);
	}
	filename = token->next->value;
	if (token->type == TOKEN_REDIRECT_IN)
	{
		printf("Setting input file: %s\n", filename);
		if (cmd->input_file)
			free(cmd->input_file);
		cmd->input_file = ft_strdup(filename);
	}
	else if (token->type == TOKEN_REDIRECT_OUT)
	{
		printf("Setting output file (overwrite): %s\n", filename);
		if (cmd->output_file)
			free(cmd->output_file);
		cmd->output_file = ft_strdup(filename);
		cmd->append_mode = 0;
	}
	else if (token->type == TOKEN_REDIRECT_APPEND)
	{
		printf("Setting output file (append): %s\n", filename);
		if (cmd->output_file)
			free(cmd->output_file);
		cmd->output_file = ft_strdup(filename);
		cmd->append_mode = 1;
	}
	else if (token->type == TOKEN_HEREDOC)
	{
		printf("Setting heredoc delimiter: %s\n", filename);
		if (cmd->heredoc_delimiter)
			free(cmd->heredoc_delimiter);
		cmd->heredoc_delimiter = ft_strdup(filename);
	}
	else
	{
		printf("Error: Unknown redirection type\n");
		return (0);
	}
	// skip filename [>>>>>>>]
	*myToken = token->next;
	return (1);
}
