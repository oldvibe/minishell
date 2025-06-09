/* parser.h - Parser header file */

#ifndef PARSER_H
# define PARSER_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>

// Token types
typedef enum {
    TOKEN_WORD, // normal string 
    TOKEN_PIPE,  // |
    TOKEN_REDIRECT_IN,    // <
    TOKEN_REDIRECT_OUT,   // >
    TOKEN_REDIRECT_APPEND, // >>
    TOKEN_HEREDOC,       // <<
    TOKEN_EOF
} t_token_type;

// Token structure
typedef struct s_token {
    t_token_type type;
    char *value;
    struct s_token *next;
} t_token;

// Lexer structure
typedef struct s_lexer {
    char *input;
    int pos;
    int len;
} t_lexer;

// Command structure for parsed commands
typedef struct s_cmd {
    char **args;              // command + arguments
    char *input_file;         // for < redirection
    char *output_file;        // for > redirection
    int append_mode;          // for >> (1 = append, 0 = overwrite)
    char *heredoc_delimiter;  // for << redirection
    struct s_cmd *next;       // for pipes
} t_cmd;

// Lexer functions
// t_lexer *init_lexer(char *input);
// void skip_whitespace(t_lexer *lexer);
// t_token *create_token(t_token_type type, char *value);
// int is_special_char(char c);
// char *read_word(t_lexer *lexer);
// t_token *get_next_token(t_lexer *lexer);
// t_token *tokenize(char *input);
// void free_tokens(t_token *tokens);
// void print_tokens(t_token *tokens);

// Parser functions (to be implemented)
// t_cmd *parse_tokens(t_token *tokens);
// t_cmd *parse_simple_command(t_token **tokens);
// void handle_redirections(t_cmd *cmd, t_token **tokens);
// t_cmd *create_command(void);
// void free_commands(t_cmd *cmd);

#endif