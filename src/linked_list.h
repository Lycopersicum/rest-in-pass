#ifndef REST_IN_PASS_LINKED_LIST_H
#define REST_IN_PASS_LINKED_LIST_H

#include <pthread.h>

typedef struct linked_list_entry_t
{
    struct linked_list_entry_t *next;
    void *data;
} linked_list_entry_t;

typedef struct
{
    pthread_mutex_t mutex;
    linked_list_entry_t *head;
} linked_list_t;

#define linked_list_foreach(list, entry) \
    for(entry = list->head; entry != NULL; entry = entry->next)

void linked_list_lock(linked_list_t *list);
void linked_list_unlock(linked_list_t *list);

linked_list_t *linked_list_create();
void linked_list_destroy(linked_list_t *list);

int linked_list_add(linked_list_t *list, void *data);
int linked_list_remove(linked_list_t *list, void *data);

#endif /* REST_IN_PASS_LINKED_LIST_H */
