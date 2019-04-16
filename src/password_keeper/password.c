#include <string.h> /* strdup */
#include <stdlib.h> /* malloc, free */

#include "password.h"

int password_json_validate(json_t *j_password)
{
    const char *name, *secret;

    if (!json_is_object(j_password))
        return -1;

    name = json_string_value(json_object_get(j_password, "name"));
    secret = json_string_value(json_object_get(j_password, "secret"));

    if ((name == NULL)
        || (secret == NULL))
    {
        return -1;
    }

    return 0;
}

password_t *password_from_json(json_t *j_password)
{
    password_t *password;

    if (password_json_validate(j_password) != 0)
        return NULL;

    password = malloc(sizeof(password_t));
    if (password == NULL)
        return NULL;

    password->name = strdup(
        json_string_value(json_object_get(j_password, "name")));
    password->secret = strdup(
        json_string_value(json_object_get(j_password, "secret")));

    if ((password->name == NULL)
        || (password->secret == NULL))
    {
        password_destroy(password);
        return NULL;
    }

    return password;
}

json_t *password_to_json(password_t *password)
{
    json_t *j_password;

    if (password == NULL)
        return NULL;

    j_password = json_pack("{s:s, s:s}",
                           "name", password->name,
                           "secret", password->secret);

    return j_password;
}

void password_destroy(password_t *password)
{
    if (password == NULL)
        return;

    if (password->name != NULL)
        free(password->name);

    if (password->secret != NULL)
        free(password->secret);

    free(password);
}

const char *password_get_name(password_t *password)
{
     if (password == NULL)
         return NULL;

     return password->name;
}

const char *password_get_secret(password_t *password)
{
     if (password == NULL)
         return NULL;

     return password->secret;
}

int password_set_name(password_t *password, const char *name)
{
     if (password == NULL)
         return -1;

     if (name == NULL)
         return -1;

     free(password->name);
     password->name = strdup(name);
     return 0;
}

int password_set_secret(password_t *password, const char *secret)
{
     if (password == NULL)
         return -1;

     if (secret == NULL)
         return -1;

     free(password->secret);
     password->secret = strdup(secret);
     return 0;
}
