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

char *get_env_value(t_env *env, char *key)
{
    t_env *current = env;
    
    while (current)
    {
        if (ft_strcmp(current->key, key) == 0)
            return current->value;
        current = current->next;
    }
    return NULL;
}

char *expand_variable(char *str, t_env *env)
{
    char *result;
    char *var_name;
    char *var_value;
    int i;
    
    if (!str || !ft_strchr(str, '$'))
        return ft_strdup(str);
    
    if (str[0] == '$')
    {
        // ft_isalnum nzidoha
        i = 1;
        // while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		while (str[i] && ((str[i] >= 'a' && str[i] <= 'z') || 
                  (str[i] >= 'A' && str[i] <= 'Z') || 
                  (str[i] >= '0' && str[i] <= '9') || 
                  str[i] == '_'))
            i++;
        
        var_name = malloc(i);
        ft_strncpy(var_name, str + 1, i - 1);
        var_name[i - 1] = '\0';
        
        // Get value from environment
        var_value = get_env_value(env, var_name);
        free(var_name);
        
        if (!var_value)
            return ft_strdup(""); // Return empty if var not found
        
        return ft_strdup(var_value);
    }
    
    return ft_strdup(str);
}

// all good
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
    t_cmd *current_cmd = NULL;   // last command zdnaha
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


