#include "../../include/gc.h"

void gc_add(void *ptr, t_gc *gc)
{
    t_gc_node *node = malloc(sizeof(t_gc_node));
    if (!node)
        return;
    node->ptr = ptr;
    node->next = gc->head;
    gc->head = node;
}

void *gc_malloc(size_t size, t_gc *gc)
{
    void *ptr = malloc(size);
    if (ptr)
        gc_add(ptr, gc);
    return ptr;
}

void gc_free_all(t_gc *gc)
{
    t_gc_node *cur = gc->head;
    while (cur)
    {
        t_gc_node *next = cur->next;
        free(cur->ptr);
        free(cur);
        cur = next;
    }
    gc->head = NULL;
}