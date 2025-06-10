#ifndef MINISHELL_H
#define MINISHELL_H

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <signal.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <string.h>
#include <limits.h>
#include "utils.h"
#include "parser.h"
#include "builtins.h"

typedef struct s_env
{
    char *key;
    char *value;
    struct s_env *next;
}   t_env;

// for duplicate the env :
t_env *create_env(const char *env);
t_env   *dup_env(char **env);
void free_env(t_env *env);
void print_env(t_env *env);

//


#endif
