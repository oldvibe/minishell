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
	cmd->append_mode = 0;
	cmd->heredoc_delimiter = NULL;
	cmd->next = NULL;
	return (cmd);
}

int	add_arg_to_cmd(t_cmd *cmd, char *arg,  t_env *env)
{
	char *expanded_arg;
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
	expanded_arg = expand_variable(arg, env);
	cmd->args[count] = ft_strdup(arg);
	cmd->args[count + 1] = NULL;
	return (1);
}

t_cmd	*parse_command(t_token **tokens,  t_env *env)
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
		printf("-----------> Processing token: %s (type:%d) <---------------\n", current->value, current->type);
		if (current->type == TOKEN_WORD)
		{
			printf("---> Adding argument: '%s'\n", current->value);
			if (!add_arg_to_cmd(cmd, current->value, env))
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
			printf("\nError: Unexpected token type %d\n", current->type);
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

t_cmd *parse_tokens(t_token *tokens, t_env *env)
{
    if (!tokens)
        return (NULL);
    
    t_cmd *first_cmd = NULL; 
    t_cmd *current_cmd = NULL;
    t_token *current_token = tokens;
    
    while (current_token && current_token->type != TOKEN_EOF)
    {
        printf("\n=== Parsing new command ===\n");
        
        t_cmd *new_cmd = parse_command(&current_token, env);
        if (!new_cmd)
        {
            printf("Error: Failed to parse command\n");
            free_cmd_list(first_cmd);
            return (NULL);
        }
        
        if (!first_cmd)
        {
            printf("Setting as first command\n");
            first_cmd = new_cmd;
            current_cmd = new_cmd;
        }
        else
        {
            printf("Linking to previous command\n");
            current_cmd->next = new_cmd;
            current_cmd = new_cmd;
        }
        
        if (current_token && current_token->type == TOKEN_PIPE)
        {
            printf("Found pipe, continuing to next command\n");
            current_token = current_token->next;  // Skip khfif lpipe
        }
    }
    
    return (first_cmd);
}

int validate_tokens(t_token *tokens)
{
    t_token *current = tokens;
    
    if (!current || current->type == TOKEN_EOF)
    {
        printf("Error: Empty command\n");
        return 0;
    }
    
    // Check if starts with pipe
    if (current->type == TOKEN_PIPE)
    {
        printf("Error: Command cannot start with pipe\n");
        return 0;
    }
    
    while (current && current->type != TOKEN_EOF)
    {
        // Check pipe followed by pipe or EOF
        if (current->type == TOKEN_PIPE)
        {
            if (!current->next || current->next->type == TOKEN_EOF)
            {
                printf("Error: Pipe without command\n");
                return 0;
            }
            if (current->next->type == TOKEN_PIPE)
            {
                printf("Error: Invalid double pipe\n");
                return 0;
            }
        }
        
        // Check redirections followed by non-word
        if (current->type == TOKEN_REDIRECT_IN || 
            current->type == TOKEN_REDIRECT_OUT ||
            current->type == TOKEN_REDIRECT_APPEND ||
            current->type == TOKEN_HEREDOC)
        {
            if (!current->next || current->next->type != TOKEN_WORD)
            {
                printf("Error: Redirection without filename\n");
                return 0;
            }
        }
        
        current = current->next;
    }
    return 1;
}


