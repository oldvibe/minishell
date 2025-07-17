#ifndef PARSER_H
# define PARSER_H

// # include "minishell.h"
struct s_env;
typedef struct s_env t_env;

typedef enum
{
	TOKEN_WORD,
	TOKEN_PIPE,
	TOKEN_REDIRECT_IN,
	TOKEN_REDIRECT_OUT,
	TOKEN_REDIRECT_APPEND,
	TOKEN_HEREDOC,         
	TOKEN_EOF
}					t_token_type;

typedef struct s_token
{
	t_token_type	type;
	char			*value;
	struct s_token	*next;
}					t_token;

typedef struct s_lexer
{
	char			*input;
	int				pos;
	int				len;
}					t_lexer;

// Command structure for parsed commands
typedef struct s_cmd
{
	char **args;             
	char *input_file;        
	char *output_file;      
	int append_mode;         // for >> (1 = append, 0 = overwrite)
	char *heredoc_delimiter; // for << redirection
	struct s_cmd *next;      // for pipes
}					t_cmd;

// Lexer functions
t_lexer		*init_lexer(char *input);
void		skip_whitespace(t_lexer *lexer);
t_token		*create_token(t_token_type type, char *value);
int			is_special_char(char c);
char		*read_word(t_lexer *lexer);
t_token		*get_next_token(t_lexer *lexer);
t_token		*tokenize(char *input);
void		free_tokens(t_token *tokens);
void		print_tokens(t_token *tokens);

// Parser functions (to be implemented)
t_cmd	*init_cmd(void);
int    	add_arg_to_cmd(t_cmd *cmd, char *arg, t_env *env);
t_cmd  	*parse_command(t_token **tokens, t_env *env);
int 	validate_tokens(t_token *tokens);
t_cmd  	*parse_tokens(t_token *tokens, t_env *env);
char 	*expand_variable(char *str, t_env *env);
char 	*get_env_value(t_env *env, char *key);
int		handle_redirection(t_cmd *cmd, t_token **myToken);
void	print_prompt(void);
void	print_cmd(t_cmd *cmd);
#endif
