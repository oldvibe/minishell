#ifndef BUILTINS_H
# define BUILTINS_H
#include "../include/parser.h"

int		ft_pwd();
int		ft_exit(char **cmds);
int     ft_echo(t_cmd *cmds);
int    ft_export(char **args, t_env **envi);
int    ft_cd(t_cmd *cmd);
int	ft_env(char **cmds, t_env *env_list);
int	ft_exit(char	**cmds);
int ft_unset(char **args, t_env **envi);

#endif
