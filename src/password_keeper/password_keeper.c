#include <string.h> /* strcmp */

#include "password_keeper.h"

static int encrypt_file(const char *path,
                        const char *encryption_key)
{
    return 0;
}

static int password_keeper_save_passwords(password_keeper_t *keeper)
{
    json_t *j_passwords, *j_password;
    linked_list_t *passwords = keeper->passwords;
    linked_list_entry_t *list_entry;
    password_t *password;
    const char *path, *encryption_key;
    int status;

    path = keeper->settings->passwords_file;
    encryption_key = keeper->settings->encryption_key;

    if ((path == NULL)
        || (passwords == NULL))
    {
        return -1; 
    }

    j_passwords = json_array();
    linked_list_lock(passwords);
    linked_list_foreach(passwords, list_entry)
    {
        password = list_entry->data;
        j_password = password_to_json(password);
        /* Don't do anything if j_password is NULL. */
        if (j_password != NULL)
            json_array_append_new(j_passwords, j_password);
    }
    linked_list_unlock(passwords);

    status = json_dump_file(j_passwords, path, JSON_INDENT(2));
    json_decref(j_passwords);

    if (status != 0)
        return -1;

    if ((encryption_key != NULL)
        && (encrypt_file(path, encryption_key) != 0))
        return -1; 

    return 0; 
}

int password_keeper_initialize(password_keeper_t *password_keeper,
                               password_keeper_settings_t *settings)
{
    if (password_keeper == NULL)
        return -1;

    password_keeper->settings = settings;

    password_keeper->passwords = linked_list_create();

    return 0;
}

void password_keeper_terminate(password_keeper_t *password_keeper)
{
    if (password_keeper == NULL)
        return;

    linked_list_destroy(password_keeper->passwords);
}

int password_keeper_create_password(password_keeper_t *password_keeper, 
                                    json_t *j_password)
{
    password_t *password;

    if (password_keeper == NULL)
        return -1;

    password = password_from_json(j_password);
    if (password == NULL)
        return -1;

    if (linked_list_add(password_keeper->passwords, password) != 0)
        return -1;

    if (password_keeper_save_passwords(password_keeper) != 0)
        return -1;

    return 0;
}

static password_t *password_keeper_find_password(password_keeper_t *keeper,
                                                 const char *name)
{
    linked_list_t *passwords;
    linked_list_entry_t *list_entry;
    password_t *password;

    if (keeper == NULL)
        return NULL;

    passwords = keeper->passwords;
    if (passwords == NULL)
        return NULL;

    linked_list_lock(passwords);
    linked_list_foreach(passwords, list_entry)
    {
        password = list_entry->data;

        if (strcmp(password->name, name) == 0) 
        {
            linked_list_unlock(passwords);
            return password;
        }
    }
    linked_list_unlock(passwords);

    return NULL;
}

const char *password_keeper_read_secret(password_keeper_t *password_keeper,
                                       const char *name)
{
    password_t *password;
    password = password_keeper_find_password(password_keeper, name);

    if (password == NULL)
        return NULL;

    return password_get_secret(password);
}

int password_keeper_update_secret(password_keeper_t *password_keeper, 
                               const char *name,
                               const char *secret)
{
    password_t *password;
    int status;

    password = password_keeper_find_password(password_keeper, name);
    if (password == NULL)
        return -1;

    linked_list_lock(password_keeper->passwords);
    status = password_set_secret(password, secret);
    linked_list_unlock(password_keeper->passwords);

    if (status != 0)
        return -1;

    if (password_keeper_save_passwords(password_keeper) != 0)
        return -1;

    return 0;
}

int password_keeper_delete_password(password_keeper_t *password_keeper,
                                    const char *name)
{
    password_t *password;

    password = password_keeper_find_password(password_keeper, name);
    if (password == NULL)
        return -1;

    linked_list_remove(password_keeper->passwords, password);
    password_destroy(password);

    if (password_keeper_save_passwords(password_keeper) != 0)
        return -1;

    return 0;
}
