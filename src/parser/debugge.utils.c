#include "minishell.h"

void	print_cmd(t_cmd *cmd)
{
	if (!cmd)
	{
		printf("Command: NULL\n");
		return ;
	}
	printf("=== Command Info ===\n");
	// Print arguments
	printf("Arguments: ");
	if (cmd->args)
	{
		for (int i = 0; cmd->args[i]; i++)
			printf("'%s' ", cmd->args[i]);
	}
	else
		printf("(none)");
	printf("\n");
	// Print redirections
	if (cmd->input_file)
		printf("Input file: %s\n", cmd->input_file);
	if (cmd->output_file)
		printf("Output file: %s (mode: %s)\n", cmd->output_file,
			cmd->append_mode ? "append" : "overwrite");
	if (cmd->heredoc_delimiter)
		printf("Heredoc delimiter: %s\n", cmd->heredoc_delimiter);
}

// Debug function to print tokens
void	print_tokens(t_token *token)
{
	t_token *current = token;
	char *type_names[] = {"WORD", "PIPE", "REDIRECT_IN", "REDIRECT_OUT",
		"REDIRECT_APPEND", "HEREDOC", "EOF"};
	while (current)
	{
		printf("Token: %s, Value: ", type_names[current->type]);
		if (current->value)
			printf("%s\n", current->value);
		else
			printf("NULL\n");
		current = current->next;
	}
}