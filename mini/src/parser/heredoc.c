#include "../../include/minishell.h"
#include "../../include/gc.h"

char *read_heredoc(const char *delimiter, int expand, t_env *env, t_gc *gc)
{
    char	*line;
    char	*content = gc_malloc(1, gc);
    size_t	len = 0;

    if (!content)
        return NULL;
    content[0] = '\0';

    while (1)
    {
        line = readline("> ");
        if (!line || strcmp(line, delimiter) == 0)
        {
            free(line);
            break;
        }
        if (expand)
        {
            char *expanded = expand_variable(line, env, gc);
            free(line);
            line = expanded;
        }
        size_t line_len = strlen(line);
        char *new_content = gc_malloc(len + line_len + 2, gc);
        if (!new_content)
        {
            free(line);
            return NULL;
        }
        memcpy(new_content, content, len);
        memcpy(new_content + len, line, line_len);
        new_content[len + line_len] = '\n';
        new_content[len + line_len + 1] = '\0';
        content = new_content;
        free(line);
        len += line_len + 1;
    }
    gc_free_all(&gc);
    return content;
}