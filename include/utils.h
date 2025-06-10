#ifndef UTILS_H
#define UTILS_H

#include "minishell.h"

char	**ft_split(const char *s, char c);
char	*ft_strchr(const char *s, int c);
int	    ft_strcmp(const char *s1, const char *s2);
char	*ft_strdup(const char *s);
char	*ft_strjoin(const char *s1, const char *s2);
int	    ft_strncmp(const char *s1, const char *s2, size_t n);
int     ft_strlen(const char *s);
char    *ft_strtok(char *str, const char *delim, int *index);
char    *ft_substr(const char *s, size_t start, size_t len);
char	*ft_strncpy(char *dest, const char *src, size_t n);
int		ft_atoi(const char	*str);


#endif
