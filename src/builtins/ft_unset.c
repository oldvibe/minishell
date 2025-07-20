#include "../../include/minishell.h"


void    remove_env(char *name, t_env **envi)
{
    t_env *current;
    t_env *prev;
    current = *envi;
    if(!ft_strcmp(name,current->key))
    {
        current = current->next;
        *envi = current;
        return ;
    }
    prev = current;
    current = current->next;

    while (current)
    {
        if(!ft_strcmp(name,current->key))
        {
            prev->next =current->next;
            return;
        }
        prev = current;
        current = current->next;
    }
    
}

int ft_unset(char **args, t_env **envi)
{
    int i;

    i = 1;
    while(args[i])
    {
        remove_env(args[i], envi);
        i++;
    }  
}