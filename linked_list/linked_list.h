#ifndef __LINKED_LIST_H__
#define __LINKED_LIST_H__

#include <stdlib.h>
#include <stdbool.h>

typedef struct linked_list_t linked_list_t;

extern linked_list_t *linked_list_empty();
extern size_t linked_list_size(linked_list_t *linked_list);
extern bool linked_list_is_empty(linked_list_t *linked_list);
extern const void *linked_list_add_head(linked_list_t *linked_list,
    const void *xptr);
extern const void *linked_list_add_tail(linked_list_t *linked_list,
    const void *xptr);
extern const void *linked_list_remove_head(linked_list_t *linked_list);
extern int linked_list_apply(linked_list_t *linked_list,
    int (*f) (const void *));
extern void linked_list_dispose(linked_list_t **linked_list_ptr);

#endif
