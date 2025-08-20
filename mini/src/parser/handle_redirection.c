#include "../../include/minishell.h"

static int	set_input_file(t_cmd *cmd, char *filename, t_gc *gc)
{
    if (cmd->input_file)
        free(cmd->input_file);
    cmd->input_file = gc_malloc(strlen(filename) + 1, gc);
    strcpy(cmd->input_file, filename);
    return (cmd->input_file != NULL);
}

static int	set_output_file(t_cmd *cmd, char *filename, int append, t_gc *gc)
{
    if (cmd->output_file)
        free(cmd->output_file);
    cmd->output_file = gc_malloc(strlen(filename) + 1, gc);
    strcpy(cmd->output_file, filename);
    cmd->append_mode = append;
    return (cmd->output_file != NULL);
}

static int	set_heredoc_delimiter(t_cmd *cmd, char *delimiter, t_gc *gc)
{
    if (cmd->heredoc_delimiter)
        free(cmd->heredoc_delimiter);
    cmd->heredoc_delimiter = gc_malloc(strlen(delimiter) + 1, gc);
    strcpy(cmd->heredoc_delimiter, delimiter);
    return (cmd->heredoc_delimiter != NULL);
}

// Improved: handles multiple redirections and quoted filenames
int	handle_redirection(t_cmd *cmd, t_token **my_token, t_gc *gc)
{
    t_token	*token = *my_token;
    char	*filename;

    while (token && (token->type == TOKEN_REDIRECT_IN || token->type == TOKEN_REDIRECT_OUT
            || token->type == TOKEN_REDIRECT_APPEND || token->type == TOKEN_HEREDOC))
    {
        if (!token->next || token->next->type != TOKEN_WORD)
        {
            fprintf(stderr, "minishell: syntax error near unexpected token `%s'\n",
                token->next ? token->next->value : "newline");
            return (0);
        }
        filename = token->next->value;

        // Remove quotes from filename if present
        if ((filename[0] == '\'' || filename[0] == '\"') &&
            filename[strlen(filename) - 1] == filename[0])
        {
            char *tmp = ft_substr(filename, 1, strlen(filename) - 2);
            filename = tmp;
        }

        if (token->type == TOKEN_REDIRECT_IN)
        {
            if (!set_input_file(cmd, filename, gc))
                return (0);
        }
        else if (token->type == TOKEN_REDIRECT_OUT)
        {
            if (!set_output_file(cmd, filename, 0, gc))
                return (0);
        }
        else if (token->type == TOKEN_REDIRECT_APPEND)
        {
            if (!set_output_file(cmd, filename, 1, gc))
                return (0);
        }
        else if (token->type == TOKEN_HEREDOC)
        {
            if (!set_heredoc_delimiter(cmd, filename, gc))
                return (0);
        }

        if ((filename[0] == '\'' || filename[0] == '\"') &&
            filename[strlen(filename) - 1] == filename[0])
            free(filename);

        token = token->next->next;
    }
    *my_token = token;
    return (1);
}