#ifndef BUILTINS_H
# define BUILTINS_H

#include "minishell.h"
#include "env.h"

int		ft_pwd();
int		ft_exit(char **cmds);
int	ft_env(char **cmds, t_env *env_list);
#endif
