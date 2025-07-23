#include "../../include/minishell.h"

void update_env(char *arg, t_env **envi)
{
    char *name;
    char *valuee;
    int i;
    i = 0;
    t_env *current;
    current = *envi;

    while(arg[i] && arg[i]!= '=')
        i++;
    name = ft_substr(arg, 0, i);
    if(!name)
        exit(1);
    if (arg[i] == '=' && arg[i + 1])
    {
        valuee = ft_strdup(arg + i + 1);
    }
    else if(arg[i] == '=' && !arg[i+ 1])
        valuee = ft_strdup("");
    else
        valuee = NULL;
    while (current->next)
    {
        if(!strcmp(current->key, name))
        {
            current->value = valuee;
            return;
        }
        current = current->next;
    }
    if(!strcmp(current->key, name))
    {
        current->value = valuee;
        return;
    }
    else{

        t_env *new = malloc(sizeof(t_env));
        new->key = name;
        new->value = valuee;
        current->next = new;
        new->next = NULL;
    }
    return;


}

int    check_arg(char *arg)
{
    int i;
    i=1;
    if(arg[0] != '_' && !isalpha(arg[0]))
    {
        printf("'%s': not a valid identifier", arg);
        return(1);
    }
    while(arg[i] && arg[i]!= '=')
    {
        if(arg[i] != '_' && !isdigit(arg[i]) && !isalpha(arg[i]))
        {
            printf("'%s': not a valid identifier", arg);
            return(1);
        }
        i++; 
    }
    return 0; 

}


int    ft_export(char **args, t_env **envi)
{
    t_env *current = *envi;
    int i;
    i = 1;
    if(!args[1])
    {
        while (current)
        {
            printf("declare -x %s=%s\n", current->key, current->value);
            current= current->next;
        }
    }
    else
    {
        while (args[i])
        { 
            if(check_arg(args[i]))
                return 1;
            update_env(args[i], envi);
            i++;
        }
        
        
    }
    return 0;
}
