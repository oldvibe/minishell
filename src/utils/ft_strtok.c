#include "../../include/utils.h"

char *ft_strtok(char *str, const char *delim, int *index)
{

    char *token;
    int i;
    int start;
    int end;

    if(!str || !delim || !index)
        return NULL;
    while(str[*index] && ft_strchr(delim, str[*index]))
        (*index)++;
    if(str[*index] == '\0')
        return NULL;
    start = *index;
    while(str[*index] && !ft_strchr(delim, str[*index]))
        (*index)++;
    end = *index;
    token = malloc(end - start + 1);
    if(!token)
        return NULL;
    i = 0;
    while(i < end - start)
    {
        token[i] = str[start + i];
        i++;
    }
    token[i] = '\0';
    return token;
}
