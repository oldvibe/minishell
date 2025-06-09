#include "../../include/parser.h"

t_lexer *init_lexer(char *input)
{
    t_lexer *lexer;

    lexer = malloc(sizeof(t_lexer));
    if (!lexer)
        return (NULL);
    lexer->input = input;
    lexer->pos = 0;
    lexer->len = strlen(input);
    return (lexer);
}

// Skip whitespace
void skip_whitespace(t_lexer *lexer)
{
    while (lexer->pos < lexer->len &&
           (lexer->input[lexer->pos] == ' ' ||
            lexer->input[lexer->pos] == '\t'))
        lexer->pos++;
}

// Create new token
t_token *create_token(t_token_type type, char *value)
{
    t_token *token;

    token = malloc(sizeof(t_token));
    if (!token)
        return (NULL);
    token->type = type;
    token->value = value ? strdup(value) : NULL;
    token->next = NULL;
    return (token);
}

int is_special_char(char c)
{
    return (c == '|' || c == '<' || c == '>' || c == ' ' || c == '\t');
}

// Read word token (handles quotes)
char *read_word(t_lexer *lexer)
{
    int start = lexer->pos;
    char quote = 0;

    while (lexer->pos < lexer->len)
    {
        char c = lexer->input[lexer->pos];

        // Handle quotes
        if ((c == '\'' || c == '"') && !quote)
        {
            quote = c;
            lexer->pos++;
            continue;
        }
        if (c == quote)
        {
            quote = 0;
            lexer->pos++;
            continue;
        }

        // If not in quotes and hit special char, stop
        if (!quote && is_special_char(c))
            break;

        lexer->pos++;
    }

    // Extract the word
    int len = lexer->pos - start;
    char *word = malloc(len + 1);
    if (!word)
        return (NULL);
    ft_strncpy(word, lexer->input + start, len);
    word[len] = '\0';

    return (word);
}

// Get next token
t_token *get_next_token(t_lexer *lexer)
{
    skip_whitespace(lexer);

    if (lexer->pos >= lexer->len)
        return (create_token(TOKEN_EOF, NULL));

    char c = lexer->input[lexer->pos];

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

    // Handle words (including quoted strings)
    char *word = read_word(lexer);
    if (!word)
        return (NULL);

    return (create_token(TOKEN_WORD, word));
}

// Tokenize entire input
t_token *tokenize(char *input)
{
    t_lexer *lexer = init_lexer(input);
    if (!lexer)
        return (NULL);

    t_token *tokens = NULL;
    t_token *current = NULL;
    t_token *token;

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

// Free token list
void free_tokens(t_token *tokens)
{
    t_token *current = tokens;
    t_token *next;

    while (current)
    {
        next = current->next;
        if (current->value)
            free(current->value);
        free(current);
        current = next;
    }
}

// Debug function to print tokens
void print_tokens(t_token *tokens)
{
    t_token *current = tokens;
    char *type_names[] = {"WORD", "PIPE", "REDIRECT_IN", "REDIRECT_OUT",
                          "REDIRECT_APPEND", "HEREDOC", "EOF"};

    while (current)
    {
        printf("Token: %s, Value: %s\n",
               type_names[current->type],
               current->value ? current->value : "NULL");
        current = current->next;
    }
}