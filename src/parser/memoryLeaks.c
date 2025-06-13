#include "../../include/minishell.h"

void	free_cmd(t_cmd *cmd)
{
	if (!cmd)
		return ;
	if (cmd->args)
	{
		for (int i = 0; cmd->args[i]; i++)
			free(cmd->args[i]);
		free(cmd->args);
	}
	if (cmd->input_file)
		free(cmd->input_file);
	if (cmd->output_file)
		free(cmd->output_file);
	if (cmd->heredoc_delimiter)
		free(cmd->heredoc_delimiter);
	free(cmd);
}

void free_cmd_list(t_cmd *cmd_list)
{
    t_cmd *current = cmd_list;
    t_cmd *next;
    
    while (current)
    {
        next = current->next;
        free_cmd(current);
        current = next;
    }
}

// hada lexer (free list)
void	free_tokens(t_token *token)
{
	t_token *current;
	t_token *next;

	current = token;
	while (current)
	{
		next = current->next;
		if (current->value)
			free(current->value);
		free(current);
		current = next;
	}
}