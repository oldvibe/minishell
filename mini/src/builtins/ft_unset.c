#include "../../include/minishell.h"


int    remove_env(char *name, t_env **envi)
{
    t_env *current;
    t_env *prev;
    current = *envi;
    if(!ft_strcmp(name,current->key))
    {
        current = current->next;
        *envi = current;
        return 0;
    }
    prev = current;
    current = current->next;

    while (current)
    {
        if(!ft_strcmp(name,current->key))
        {
            prev->next =current->next;
            return 0;
        }
        prev = current;
        current = current->next;
    }
    return 1;
    
}

int ft_unset(char **args, t_env **envi)
{
    int i;

    int l;
    i = 1;
    while(args[i])
    {
        l = remove_env(args[i], envi);
        i++;
    }
    return l;
}