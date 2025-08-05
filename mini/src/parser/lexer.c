#include "../../include/minishell.h"

static char	*append_char(char *str, char c, int *len, t_gc *gc)
{
	char	*new_str = gc_malloc(*len + 2, gc);
	int		i;

	if (!new_str)
		return (NULL);
	i = 0;
	while (i < *len)
	{
		new_str[i] = str[i];
		i++;
	}
	new_str[*len] = c;
	new_str[*len + 1] = '\0';
	free(str);
	(*len)++;
	return (new_str);
}

char	*read_word(t_lexer *lexer, t_gc *gc)
{
	char	*result;
	int		len;
	char	quote;
	char	c;

	result = gc_malloc(1, gc);
	if (!result)
		return (NULL);
	result[0] = '\0';
	len = 0;
	quote = 0;
	while (lexer->pos < lexer->len)
	{
		c = lexer->input[lexer->pos];
		if ((c == '\'' || c == '"') && !quote)
		{
			quote = c;
			lexer->pos++;
			continue ;
		}
		if (c == quote)
		{
			quote = 0;
			lexer->pos++;
			continue ;
		}
		if (!quote && is_special_char(c))
			break ;
		result = append_char(result, c, &len, gc);
		if (!result)
			return (NULL);
		lexer->pos++;
	}
	if (quote != 0)
		return (free(result), NULL);
	return (result);
}

t_token	*create_token(t_token_type type, char *value, t_gc *gc)
{
	t_token	*token;

	token = gc_malloc(sizeof(t_token), gc);
	if (!token)
		return (NULL);
	token->type = type;
	if (value)
		token->value = ft_strdup(value);
	else
		token->value = NULL;
	token->next = NULL;
	return (token);
}

static t_token	*handle_redirect_tokens(t_lexer *lexer, t_gc *gc)
{
	char	c;

	c = lexer->input[lexer->pos];
	if (c == '<')
	{
		if (lexer->pos + 1 < lexer->len && lexer->input[lexer->pos + 1] == '<')
		{
			lexer->pos += 2;
			return (create_token(TOKEN_HEREDOC, "<<", gc));
		}
		lexer->pos++;
		return (create_token(TOKEN_REDIRECT_IN, "<", gc));
	}
	if (c == '>')
	{
		if (lexer->pos + 1 < lexer->len && lexer->input[lexer->pos + 1] == '>')
		{
			lexer->pos += 2;
			return (create_token(TOKEN_REDIRECT_APPEND, ">>", gc));
		}
		lexer->pos++;
		return (create_token(TOKEN_REDIRECT_OUT, ">", gc));
	}
	return (NULL);
}

t_token	*get_next_token(t_lexer *lexer, t_gc *gc)
{
	char	c;
	char	*word;
	t_token	*token;

	skip_whitespace(lexer);
	if (lexer->pos >= lexer->len)
		return (create_token(TOKEN_EOF, NULL, gc));
	c = lexer->input[lexer->pos];
	if (c == '|')
	{
		lexer->pos++;
		return (create_token(TOKEN_PIPE, "|", gc));
	}
	token = handle_redirect_tokens(lexer, gc);
	if (token)
		return (token);
	word = read_word(lexer, gc);
	if (!word)
		return (NULL);
	if (ft_strlen(word) == 0)
	{
		free(word);
		return (get_next_token(lexer, gc));
	}
	return (create_token(TOKEN_WORD, word, gc));
}

t_lexer	*init_lexer(char *input, t_gc *gc)
{
	t_lexer	*lexer;
	lexer = gc_malloc(sizeof(t_lexer), gc);
	// lexer = malloc(sizeof(t_lexer));
	if (!lexer)
		return (NULL);
	lexer->input = input;
	lexer->pos = 0;
	lexer->len = ft_strlen(input);
	return (lexer);
}

t_token	*tokenize(char *input, t_gc *gc)
{
	t_lexer	*lexer;
	t_token	*tokens;
	t_token	*current;
	t_token	*token;

	lexer = init_lexer(input, gc);
	if (!lexer)
		return (NULL);
	tokens = NULL;
	current = NULL;
	while (1)
	{
		token = get_next_token(lexer, gc);
		if (!token)
			break ;
		if (!tokens)
		{
			tokens = token;
			current = token;
		}
		else
		{
			current->next = token;
			current = token;
		}
		if (token->type == TOKEN_EOF)
			break ;
	}
	free(lexer);
	return (tokens);
}

