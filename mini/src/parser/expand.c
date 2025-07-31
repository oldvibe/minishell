#include "../../include/minishell.h"


char	*get_env_value(t_env *env, char *key)
{
	t_env	*current;

	current = env;
	while (current)
	{
		if (ft_strcmp(current->key, key) == 0)
			return (current->value);
		current = current->next;
	}
	return (NULL);
}


static int	is_valid_var_char(char c)
{
    return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')
        || (c >= '0' && c <= '9') || c == '_');
}

static char	*expand_one(char *str, int *i, t_env *env, int last_status, t_gc *gc)
{
    char	*var_name;
    char	*var_value;
    int		start;

    (*i)++; // skip '$'
    if (str[*i] == '?')
    {
        (*i)++;
        return ft_itoa(last_status);
    }
    if (str[*i] == '$')
    {
        (*i)++;
        return ft_strdup(""); // or getpid if you want bash-like
    }
    start = *i;
    while (str[*i] && is_valid_var_char(str[*i]))
        (*i)++;
    if (*i == start)
        return ft_strdup("$");
    var_name = gc_malloc(*i - start + 1, gc);
    if (!var_name)
        return NULL;
    ft_strncpy(var_name, str + start, *i - start);
    var_name[*i - start] = '\0';
    var_value = get_env_value(env, var_name);
    free(var_name);
    return var_value ? ft_strdup(var_value) : gc_malloc(1, gc);
}

char	*expand_variable(char *str, t_env *env, t_gc *gc)
{
    int		i = 0, j = 0;
    char	*result = gc_malloc(1, gc);
    int		last_status = 0; //
    result[0] = '\0';

    while (str[i])
    {
        if (str[i] == '$')
        {
            char *exp = expand_one(str, &i, env, last_status, gc);
            char *tmp = ft_strjoin(result, exp);
            result = tmp;
        }
        else
        {
            char buf[2] = {str[i], 0};
            char *tmp = ft_strjoin(result, buf);
            free(result);
            result = tmp;
            i++;
        }
    }
    return result;
}
