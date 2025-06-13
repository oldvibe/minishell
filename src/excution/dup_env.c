#include "../../include/minishell.h"

t_env *create_env(const char *env)
{
    t_env *new;
    size_t i;

    new = malloc(sizeof(t_env));
    if(!new)
        return NULL;
    i = 0;
    while (env[i] && env[i] != '=')
        i++;
    new->key = ft_substr(env, 0, i);
    if (!new->key)
    {
        free(new);
        return (NULL);
    }
    if (env[i] == '=' && env[i + 1])
    {
        new->value = ft_strdup(env + i + 1);
        if(!new->value)
        {
            free(new->key);
            free(new);
            return (NULL);
        }
    }
    else
        new->value = NULL;
    new->next = NULL;
    return (new);
}

t_env   *dup_env(char **env)
{
    t_env   *head;
    t_env   *last;
    t_env   *list;

    head = NULL;
    last = NULL;
    while (*env)
    {
        list = create_env(*env);
        if (!list)
            return (NULL);
        if(!head)
            head = list;
        else
            last->next = list;
        last = list;
        env++;
    }
    return (head);
}

void free_env(t_env *env)
{
    t_env *tmp;

    while (env)
    {
        tmp = env;
        env = env->next;
        free(tmp->key);
        free(tmp->value);
        free(tmp);
    }
}

// print for debug
void print_env(t_env *env)
{
    while (env)
    {
        if (env->key)
        {
            printf("%s", env->key);
            if (env->value)
                printf("=%s", env->value);
            printf("\n");
        }
        env = env->next;
    }
}
