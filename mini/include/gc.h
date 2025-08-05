#ifndef GC_H
#define GC_H

#include "minishell.h"
typedef struct s_gc_node
{
    void *ptr;
    struct s_gc_node *next;
} t_gc_node;

typedef struct s_gc
{
    t_gc_node *head;
} t_gc;

// GC functions
void *gc_malloc(size_t size, t_gc *gc);
void gc_add(void *ptr, t_gc *gc);
void gc_free_all(t_gc *gc);

#endif