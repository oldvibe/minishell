#ifndef PARSER_H
# define PARSER_H

# include "gc.h"
# include "minishell.h"
struct s_env;
typedef struct s_env t_env;
struct s_gc;
typedef struct s_gc t_gc;
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
typedef enum
{
	OPEN_RED,
	CLOSE_RED

} 			redire_status;


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


typedef struct s_cmd
{
	char **args;                // Command and arguments
	int argc;                   // Number of arguments
	char *input_file;           // Input redirection file
	char *output_file;          // Output redirection file
	int append_mode;            // 1 if >>, 0 if >
	char *heredoc_delimiter;    // Heredoc delimiter
	char *heredoc_content;      // Heredoc expanded content
	int heredoc_expand;         // 1 if heredoc should be expanded, 0 if quoted
	int fd_in;                  // Input file descriptor
	int fd_out;                 // Output file descriptor
	int pipe_in;                // Pipe input fd
	int pipe_out;               // Pipe output fd
	int is_builtin;             // 1 if builtin, 0 otherwise
	pid_t pid;                  // Process ID for the command
	int exit_status;            // Exit status of the command
	int error_flag;             // Error flag for parsing/execution
	char **envp;                // Environment for execve
	struct s_cmd *next;         // Next command in pipeline
}					t_cmd;

// Lexer functions
t_lexer		*init_lexer(char *input, t_gc *gc);
void		skip_whitespace(t_lexer *lexer);
t_token		*create_token(t_token_type type, char *value, t_gc *gc);
int			is_special_char(char c);
char		*read_word(t_lexer *lexer, t_gc *gc);
t_token		*get_next_token(t_lexer *lexer, t_gc *gc);
t_token		*tokenize(char *input, t_gc *gc);
void		free_tokens(t_token *tokens);
void		print_tokens(t_token *tokens);

// Parser functions
t_cmd	*init_cmd(t_gc *gc);
int    	add_arg_to_cmd(t_cmd *cmd, char *arg, t_env *env, t_gc *gc);
t_cmd	*parse_single_command(t_token **tokens, t_env *env, t_gc *gc);
int 	validate_tokens(t_token *tokens);
t_cmd  	*parse_tokens(t_token *tokens, t_env *env, t_gc *gc);
char 	*expand_variable(char *str, t_env *env, t_gc *gc);
char 	*get_env_value(t_env *env, char *key);
int		handle_redirection(t_cmd *cmd, t_token **myToken, t_gc *gc);
void	print_prompt(void);
int		check_syntax(t_token *tokens);
// Heredoc reading
char	*read_heredoc(const char *delimiter, int expand, t_env *env, t_gc *gc);
#endif
