#include "../../include/minishell.h"

int check_if_builtin(char *cmd)
{
	return (!ft_strcmp(cmd, "cd") ||
			!ft_strcmp(cmd, "echo") ||
			!ft_strcmp(cmd, "pwd") ||
			!ft_strcmp(cmd, "exit"));
}
