#include "../../include/minishell.h"

char	*read_word(t_lexer *lexer)
{
	int		start;
	char	quote;
	int		i;
	char	c;
	int		len;
	char	*word;

	start = lexer->pos;
	quote = 0;
	i = 0;
	// calculate the len excluding the quotes
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
		lexer->pos++;
	}
	len = lexer->pos - start;
	word = malloc(len + 1);
	if (!word)
		return (NULL);
	ft_strncpy(word, lexer->input + start, len);
	word[len] = '\0';
	return (word);
}

void	skip_whitespace(t_lexer *lexer)
{
	while (lexer->pos < lexer->len && (lexer->input[lexer->pos] == ' '
			|| lexer->input[lexer->pos] == '\t'))
		lexer->pos++;
}

t_token	*create_token(t_token_type type, char *value)
{
	t_token	*token;

	token = malloc(sizeof(t_token));
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

int	is_special_char(char c)
{
	return (c == '|' || c == '<' || c == '>' || c == ' ' || c == '\t');
}

t_token	*get_next_token(t_lexer *lexer)
{
	char	c;
	char	*word;

	skip_whitespace(lexer);
	if (lexer->pos >= lexer->len)
		return (create_token(TOKEN_EOF, NULL));
	c = lexer->input[lexer->pos];
	// Handle pipes
	if (c == '|')
	{
		lexer->pos++;
		return (create_token(TOKEN_PIPE, "|"));
	}
	// Handle redirections
	if (c == '<')
	{
		if (lexer->pos + 1 < lexer->len && lexer->input[lexer->pos + 1] == '<')
		{
			lexer->pos += 2;
			return (create_token(TOKEN_HEREDOC, "<<"));
		}
		lexer->pos++;
		return (create_token(TOKEN_REDIRECT_IN, "<"));
	}
	if (c == '>')
	{
		if (lexer->pos + 1 < lexer->len && lexer->input[lexer->pos + 1] == '>')
		{
			lexer->pos += 2;
			return (create_token(TOKEN_REDIRECT_APPEND, ">>"));
		}
		lexer->pos++;
		return (create_token(TOKEN_REDIRECT_OUT, ">"));
	}
	// Handle words (qoutes dakhlin)
	word = read_word(lexer);
	if (!word)
		return (NULL);
	return (create_token(TOKEN_WORD, word));
}

t_lexer	*init_lexer(char *input)
{
	t_lexer	*lexer;

	lexer = malloc(sizeof(t_lexer));
	if (!lexer)
		return (NULL);
	lexer->input = input;
	lexer->pos = 0;
	lexer->len = ft_strlen(input);
	return (lexer);
}

// Tokenize from dakchi lijay mn linput
t_token	*tokenize(char *input)
{
	t_lexer	*lexer;
	t_token	*tokens;
	t_token	*current;
	t_token	*token;

	lexer = init_lexer(input);
	if (!lexer)
		return (NULL);
	tokens = NULL;
	current = NULL;
	while ((token = get_next_token(lexer)) && token->type != TOKEN_EOF)
	{
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
	}
	free(lexer);
	return (tokens);
}
