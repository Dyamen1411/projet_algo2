#include "linked_list.h"

typedef struct element_t {
  const void *value;
  struct element_t *next;
} element_t;

struct linked_list_t {
  element_t *head;
  element_t *tail;
  size_t size;
};

linked_list_t *linked_list_empty() {
  linked_list_t *linked_list = (linked_list_t *) malloc(sizeof(linked_list_t));
  if (linked_list == NULL) {
    return NULL;
  }
  linked_list->head = NULL;
  linked_list->tail = NULL;
  linked_list->size = 0;
  return linked_list;
}

size_t linked_list_size(linked_list_t *linked_list) {
  return linked_list->size;
}

bool linked_list_is_empty(linked_list_t *linked_list) {
  return linked_list->size == 0;
}

const void *linked_list_add_head(linked_list_t *linked_list, const void *xptr) {
  if (xptr == NULL) {
    return NULL;
  }
  element_t *element = (element_t *) malloc(sizeof(element_t));
  if (element == NULL) {
    return NULL;
  }
  element->value = xptr;
  ++linked_list->size;
  if (linked_list->head == NULL) {
    element->next = NULL;
    linked_list->head = element;
    linked_list->tail = element;
    return xptr;
  }
  element->next = linked_list->head;
  linked_list->head = element;
  return xptr;
}

const void *linked_list_add_tail(linked_list_t *linked_list, const void *xptr) {
  if (xptr == NULL) {
    return NULL;
  }
  element_t *element = (element_t *) malloc(sizeof(element_t));
  if (element == NULL) {
    return NULL;
  }
  element->value = xptr;
  element->next = NULL;
  ++linked_list->size;
  if (linked_list->head == NULL) {
    linked_list->head = element;
    linked_list->tail = element;
    return xptr;
  }
  linked_list->tail->next = element;
  linked_list->tail = element;
  return xptr;
}

const void *linked_list_remove_head(linked_list_t *linked_list) {
  if (linked_list->head == NULL) {
    return NULL;
  }
  const void *xptr = linked_list->head->value;
  element_t *new_head = linked_list->head->next;
  free(linked_list->head);
  linked_list->head = new_head;
  --linked_list->size;
  if (new_head == NULL) {
    linked_list->tail = NULL;
  }
  return xptr;
}

int linked_list_apply(linked_list_t *linked_list, int (*f) (const void *)) {
  element_t *element = linked_list->head;
  while (element != NULL) {
    if (f(element->value) != 0) {
      return 1;
    }
    element = element->next;
  }
  return 0;
}

void linked_list_dispose(linked_list_t **linked_list_ptr) {
  if (*linked_list_ptr == NULL) {
    return;
  }
  element_t *element = (*linked_list_ptr)->head;
  while (element != NULL) {
    element_t *next = element->next;
    free(element);
    element = next;
  }
  free(*linked_list_ptr);
  *linked_list_ptr = NULL;
}
