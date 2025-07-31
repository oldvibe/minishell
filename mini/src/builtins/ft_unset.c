#include "../../include/minishell.h"


void    remove_env(char *name, t_env **envi)
{
    t_env *current;
    t_env *prev;

    if (!envi || !*envi)
        return;
    current = *envi;

    // Remove head node if it matches
    while (current && !ft_strcmp(name, current->key))
    {
        t_env *to_free = current;
        current = current->next;
        free(to_free->key);
        free(to_free->value);
        free(to_free);
        *envi = current;
    }

    prev = current;
    if (!current)
        return;
    current = current->next;

    while (current)
    {
        if (!ft_strcmp(name, current->key))
        {
            prev->next = current->next;
            free(current->key);
            free(current->value);
            free(current);
            return;
        }
        prev = current;
        current = current->next;
    }
}

int ft_unset(char **args, t_env **envi)
{
    int i = 1;
    while (args[i])
    {
        remove_env(args[i], envi);
        i++;
    }
    return 0;
}