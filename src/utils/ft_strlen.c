#include "../../include/utils.h"

int ft_strlen(const char *s)
{
    int count;

    count = 0;
    while(s[count])
    {
        count++;
    }
    return (count);
}
