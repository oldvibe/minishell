#ifndef EXECUTION_H
# define EXECUTION_H

# include "minishell.h"
#include "env.h"

void	execute_command(t_cmd	*cmds, char	**envp, t_env *env_list);

#endif
