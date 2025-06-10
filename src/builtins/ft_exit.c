#include "../../include/minishell.h"

static int	check_number(const char	*str)
{
	int i;

	if (!str ||!*str)
		return (0);
	i = 0;
	if (str[i] == '+' ||str[i] == '-')
		i++;
	while (str[i])
	{
		if (!(str[i] >= '0' && str[i] <= '9'))
			return (0);
		i++;
	}
	return(1);
}

int	ft_exit(char	**cmds)
{
	int	exit_status;

	printf("exit\n");
	if (!cmds[1])
		exit(0);
	if (!check_number(cmds[1]))
	{
		ft_printf("minishell: exit: %s: numeric argument required\n", cmds[1]);
		exit(2);
	}
	if (cmds[2])
	{
		ft_printf("minishell: exit: too many arguments\n");
		return (1);
	}
	exit_status = ft_atoi(cmds[1]);
	exit(exit_status);
}