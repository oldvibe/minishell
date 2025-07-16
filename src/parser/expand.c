#include "../../include/minishell.h"

char *get_env_value(t_env *env, char *key)
{
    t_env *current = env;
    
    while (current)
    {
        if (ft_strcmp(current->key, key) == 0)
            return current->value;
        current = current->next;
    }
    return NULL;
}

char *expand_variable(char *str, t_env *env)
{
    char *result;
    char *var_name;
    char *var_value;
    int i;
    
    if (!str || !ft_strchr(str, '$'))
        return ft_strdup(str);
    
    if (str[0] == '$')
    {
        // ft_isalnum nzidoha
        i = 1;
        // while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
		while (str[i] && ((str[i] >= 'a' && str[i] <= 'z') || 
                  (str[i] >= 'A' && str[i] <= 'Z') || 
                  (str[i] >= '0' && str[i] <= '9') || 
                  str[i] == '_'))
            i++;
        
        var_name = malloc(i);
        ft_strncpy(var_name, str + 1, i - 1);
        var_name[i - 1] = '\0';
        
        // Get value from environment
        var_value = get_env_value(env, var_name);
        free(var_name);
        
        if (!var_value)
            return ft_strdup(""); // Return empty if var not found
        
        return ft_strdup(var_value);
    }
    
    return ft_strdup(str);
}