#include "../../include/minishell.h"

void update_env(char *arg, t_env **envi)
{
    char *name;
    char *value;
    int i;
    i = 0;
    t_env *current;
    current = *envi;

    while(arg[i] && arg[i]!= '=')
        i++;
    name = ft_substr(arg, 0, i);
    if (arg[i] == '=' && arg[i + 1])
    {
        value = ft_strdup(arg + i + 1);

    }
    else if(arg[i] == '=' && !arg[i+ 1])
        value = ft_strdup("");
    else
        value = NULL;
    while (current)
    {
        if(!strcmp(current->key, name))
        {
            current->value;
            return;
        }
        current = current->next;
    }

    t_env *new = malloc(sizeof(t_env));
    new->key = name;
    new->value = value;
    new->next = *envi;
    *envi =new;
    return;


}

void    check_arg(char *arg)
{
    int i;
    i=1;
    if(arg[0] != '_' && !ft_isalpha(arg[0]))
    {
        printf("'%s': not a valid identifier", arg);
        exit(1);
    }
    while(arg[i] && arg[i]!= '=')
    {
        if(arg[i] != '_' && !ft_isdigit(arg[i]) && !ft_isalpha(arg[i]))
        {
            printf("'%s': not a valid identifier", arg);
            exit(1);
        }
        i++; 
    }

}


void    ft_export(char **args, t_env **envi)
{
    t_env *current = *envi;
    int i;
    i = 2;
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
            check_arg(args[i]);
            update_env(args[i], envi);
            i++;
        }
        
        
    }
}


int main(int ac, char **av,char **envp)
{
    t_env *env;
    env = dup_env(envp);
    ft_export(av, &env);
}