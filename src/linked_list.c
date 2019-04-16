#include "linked_list.h"

#include <stdlib.h> /* malloc, free */

void linked_list_lock(linked_list_t *list)
{
    if (list != NULL)
        pthread_mutex_lock(&list->mutex);
}

void linked_list_unlock(linked_list_t *list)
{
    if (list != NULL)
        pthread_mutex_unlock(&list->mutex);
}

linked_list_t *linked_list_create(void)
{
    linked_list_t *list = malloc(sizeof(linked_list_t));

    if (list == NULL)
        return NULL;

    pthread_mutex_init(&list->mutex, NULL);
    list->head = NULL;

    return list;
}

void linked_list_destroy(linked_list_t *list)
{
    linked_list_entry_t *entry;

    linked_list_lock(list);

    while (list->head != NULL)
    {
        entry = list->head;
        list->head = entry->next;
        entry->next = NULL;
        free(entry);
    }

    linked_list_unlock(list);

    pthread_mutex_destroy(&list->mutex);

    free(list);
}

int linked_list_add(linked_list_t *list, void *data)
{
    linked_list_entry_t *entry;

    entry = malloc(sizeof(linked_list_entry_t));
    if (entry == NULL)
        return -1;

    linked_list_lock(list);

    entry->next = list->head;
    entry->data = data;
    list->head = entry;

    linked_list_unlock(list);
    return 0;
}

int linked_list_remove(linked_list_t *list, void *data)
{
    linked_list_entry_t *entry, *previous;

    linked_list_lock(list);

    linked_list_foreach(list, entry)
    // for (entry = list->head; entry != NULL; entry = entry->next)
    {
        if (entry->data == data)
        {
            if (entry == list->head)
            {
                list->head = entry->next;
                entry->next = NULL;
                free(entry);
            }
            else
            {
                previous->next = entry->next;
                entry->next = NULL;
                free(entry);
            }

            linked_list_unlock(list);
            return 0;
        }
        previous = entry;
    }

    return -1;
}
