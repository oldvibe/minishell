#ifndef MINISHELL_H
# define MINISHELL_H

# include "../src/utils/ft_printf/printf.h"
# include "builtins.h"
# include "parser.h"
# include "utils.h"
# include <fcntl.h>
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/wait.h>
#include <sys/stat.h>
# include <unistd.h>

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}					t_env;

// for duplicate the env :
t_env				*create_env(const char *env);
t_env				*dup_env(char **env);
void				free_env(t_env *env);
void				print_env(t_env *env);



// handling memory leaks
void	free_cmd(t_cmd *cmd);
void 	free_cmd_list(t_cmd *cmd_list);
void	free_tokens(t_token *token); //hada lexer


//execution

void exec_non_builtin(t_cmd *cmd, char **env);
int is_built(char **args);
void	handle_redir(t_cmd *cmd, redire_status status, int tmpin, int tmpout);
char **et_path(char **envi);
 void check_absolu_path(char **args, char **env);




#endif
