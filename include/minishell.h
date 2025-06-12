#ifndef MINISHELL_H
# define MINISHELL_H

# include "../src/utils/ft_printf/printf.h"
# include "builtins.h"
# include "parser.h"
# include "execution.h"
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
# include <unistd.h>
# include "env.h"

// for duplicate the env :
t_env				*create_env(const char *env);
t_env				*dup_env(char **env);
void				free_env(t_env *env);
void				print_env(t_env *env);

//

#endif
