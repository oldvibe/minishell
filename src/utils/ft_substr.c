#include "../../include/utils.h"

char *ft_substr(const char *s, size_t start, size_t len)
{
	char	*sub;
	size_t	str_len;
	size_t	i;

	if(!s)
		return (NULL);
	i = 0;
	str_len = ft_strlen(s);
	if (start >= str_len)
		return (ft_strdup(""));
	if (len > str_len - start)
		len = str_len - start;
	sub = malloc(len + 1);
	if (!sub)
		return (NULL);
	while (i < len)
	{
		sub[i] = s[start + i];
		i++;
	}
	sub[i] = '\0';
	return (sub);
}

// char *ft_substr(const char *s, size_t start, size_t len)
// {
//     char *sub = malloc(len + 1);
//     if (!sub)
//         return NULL;
//     size_t i = 0;
//     while (i < len && s[start + i])
//     {
//         sub[i] = s[start + i];
//         i++;
//     }
//     sub[i] = '\0';
//     return sub;
// }
