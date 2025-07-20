#include "../../include/minishell.h"

void    ft_env(t_cmd *cmd, t_env *envi)
{
    while(envi)
    {
        printf("%s=%s\n", envi->key, envi->value);
        envi= envi->next;
    }
}