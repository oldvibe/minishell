#include "../../include/minishell.h"

// initialization
t_cmd	*init_cmd(void)
{
	t_cmd	*cmd;

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->args = NULL;
	cmd->input_file = NULL;
	cmd->output_file = NULL;
	cmd->append_mode = 0;
	cmd->heredoc_delimiter = NULL;
	cmd->next = NULL;
	return (cmd);
}

int	add_arg_to_cmd(t_cmd *cmd, char *arg)
{
	int	count;

	count = 0;
	if (cmd->args)
	{
		while (cmd->args[count])
			count++;
	}
	cmd->args = ft_realloc(cmd->args, sizeof(char *) * (count + 2));
	if (!cmd->args)
		return (0);
	cmd->args[count] = ft_strdup(arg);
	cmd->args[count + 1] = NULL;
	return (1);
}

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
	if (token->type == TOKEN_REDIRECT_IN) // <
	{
		printf("Setting input file: %s\n", filename);
		if (cmd->input_file)
			free(cmd->input_file);
		cmd->input_file = ft_strdup(filename);
	}
	else if (token->type == TOKEN_REDIRECT_OUT) // >
	{
		printf("Setting output file (overwrite): %s\n", filename);
		if (cmd->output_file)
			free(cmd->output_file);
		cmd->output_file = ft_strdup(filename);
		cmd->append_mode = 0;
	}
	else if (token->type == TOKEN_REDIRECT_APPEND) // >>
	{
		printf("Setting output file (append): %s\n", filename);
		if (cmd->output_file)
			free(cmd->output_file);
		cmd->output_file = ft_strdup(filename);
		cmd->append_mode = 1;
	}
	else if (token->type == TOKEN_HEREDOC) // <<
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

t_cmd	*parse_command(t_token **tokens)
{
	t_cmd	*cmd;
	t_token	*current;

	cmd = init_cmd();
	if (!cmd)
		return (NULL);
	current = *tokens;
	while (current && current->type != TOKEN_PIPE && current->type != TOKEN_EOF)
	{
		if (!current->value)
			current->value = "NULL";
		printf("-----------> Processing token: %s (type:%d) <---------------\n",
				current->value, current->type);
		if (current->type == TOKEN_WORD)
		{
			// Add argument to command
			printf("  -> Adding argument: '%s'\n", current->value);
			if (!add_arg_to_cmd(cmd, current->value))
			{
				printf("Error: Failed to add argument\n");
				free_cmd(cmd);
				return (NULL);
			}
		}
		else if (current->type == TOKEN_REDIRECT_IN
			|| current->type == TOKEN_REDIRECT_OUT
			|| current->type == TOKEN_REDIRECT_APPEND
			|| current->type == TOKEN_HEREDOC)
		{
			// hna printf khassna nbdloha
			//??
			printf("  --------> Handling redirection <------------- \n");
			if (!handle_redirection(cmd, &current))
			{
				printf("Error: Failed to handle redirection\n");
				free_cmd(cmd);
				return (NULL);
			}
		}
		else
		{
			printf("Error: Unexpected token type %d\n", current->type);
			free_cmd(cmd);
			return (NULL);
		}
		// skip to next token
		current = current->next;
	}
	// Update tokens pointer to last position hbsna fiha
	*tokens = current;
	return (cmd);
}

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
	t_token	*current;
	char	*type_names[] = {"WORD", "PIPE", "REDIRECT_IN", "REDIRECT_OUT",
			"REDIRECT_APPEND", "HEREDOC", "EOF"};

	current = token;
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



// int	main(void)
// {
// 	t_token	*tokens1;
// 	t_token	*current1;
// 	t_cmd	*cmd1;
// 	t_token	*tokens2;
// 	t_token	*current2;
// 	t_cmd	*cmd2;
// 	t_token	*tokens3;
// 	t_token	*current3;
// 	t_cmd	*cmd3;

// 	printf("=== Testing Single Command Parsing ===\n\n");



// 	// Test 1: Simple command with arguments
// 	printf("Test 1: ls -la /home\n\n");
// 	tokens1 = tokenize("ls -la /home");
// 	current1 = tokens1;
// 	cmd1 = parse_command(&current1);
// 	print_cmd(cmd1);
// 	free_tokens(tokens1);
// 	free_cmd(cmd1);
// 	//printf("\n" "=" * 40 "\n\n");
// 	printf("\n\n");


// 	// Test 2: Command with output redirection
// 	printf("Test 2: echo hello > output.txt\n\n");
// 	tokens2 = tokenize("echo hello > output.txt");
// 	current2 = tokens2;
// 	cmd2 = parse_command(&current2);
// 	print_cmd(cmd2);
// 	free_tokens(tokens2);
// 	free_cmd(cmd2);
// 	printf("\n\n");
// 	//printf("\n" "=" * 40 "\n\n");


// 	// Test 3: Complex command with multiple redirections
// 	printf("Test 3: cat < input.txt > output.txt\n\n");
// 	tokens3 = tokenize("cat < input.txt > output.txt");
// 	current3 = tokens3;
// 	cmd3 = parse_command(&current3);
// 	print_cmd(cmd3);
// 	free_tokens(tokens3);
// 	free_cmd(cmd3);
// 	return (0);
// }
