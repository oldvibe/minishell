#ifndef BUILTINS_H
# define BUILTINS_H
#include "../include/parser.h"

int		ft_pwd();
int		ft_exit(char **cmds);
void    ft_echo(t_cmd *cmds);
void    ft_cd(t_cmd *cmd);
int	    ft_env(char **cmds, t_env *env_list);

#endif
