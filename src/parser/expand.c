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

static char	*extract_var_name(char *str, int *i)
{
	char	*var_name;
	int		start;
	int		len;

	start = *i + 1;
	(*i)++;
	while (str[*i] && is_valid_var_char(str[*i]))
		(*i)++;
	len = *i - start;
	if (len == 0)
		return (NULL);
	var_name = malloc(len + 1);
	if (!var_name)
		return (NULL);
	ft_strncpy(var_name, str + start, len);
	var_name[len] = '\0';
	return (var_name);
}

char	*expand_variable(char *str, t_env *env)
{
	char	*var_name;
	char	*var_value;
	int		i;

	if (!str || !ft_strchr(str, '$'))
		return (ft_strdup(str));
	i = 0;
	if (str[i] == '$')
	{
		var_name = extract_var_name(str, &i);
		if (!var_name)
			return (ft_strdup(""));
		var_value = get_env_value(env, var_name);
		free(var_name);
		if (!var_value)
			return (ft_strdup(""));
		return (ft_strdup(var_value));
	}
	return (ft_strdup(str));
}
