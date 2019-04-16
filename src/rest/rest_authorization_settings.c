#include <stdlib.h> /* malloc, free */
#include <string.h> /* strdup */

#include "rest_authorization_settings.h"

#include "../utils/base64.h"

static const char *logging_section = "[SETTINGS:REST:AUTHORIZATION]";
static void rest_realm_destroy(rest_realm_t *realm)
{
    if (realm == NULL)
        return;

    if (realm->name != NULL)
        free(realm->name);

    if (realm->secret != NULL)
        free(realm->secret);

    free(realm);
}

static int rest_realm_validate_json(json_t *j_realm)
{
    size_t decoded_length;
    const char *name, *secret;
    unsigned char *decoded_secret;

    if (!json_is_object(j_realm))
    {
        fprintf(stdout,
                "%s realm settings should be a JSON object!\n",
                logging_section);
        return -1;
    }

    name = json_string_value(json_object_get(j_realm, "realm"));
    if (name == NULL)
    {
        fprintf(stdout,
                "%s realm name should be a string!\n",
                logging_section);
        return -1;
    }

    secret = json_string_value(json_object_get(j_realm, "secret"));
    if (secret == NULL)
    {
        fprintf(stdout,
                "%s realm \"%s\" secret should be a BASE64 string!\n",
                logging_section, name);
        return -1;
    }

    decoded_secret = base64_decode((const unsigned char *) secret,
                                   strlen(secret),
                                   &decoded_length);
    if (decoded_secret == NULL)
    {
        fprintf(stdout,
                "%s realm \"%s\" secret should be a BASE64 string!\n",
                logging_section, name);
        return -1;
    }
    free(decoded_secret);

    return 0;
}

static rest_realm_t *rest_realm_create(json_t *j_realm)
{
    rest_realm_t *realm;
    const char *name, *secret;

    if (rest_realm_validate_json(j_realm) != 0)
        return NULL;

    realm = malloc(sizeof(rest_realm_t));
    if (realm == NULL)
        return NULL;

    name = json_string_value(json_object_get(j_realm, "realm"));
    secret = json_string_value(json_object_get(j_realm, "secret"));
    realm->name = strdup(name);
    realm->secret = strdup(secret);

    if ((realm->name == NULL)
        || (realm->secret == NULL))
    {
        rest_realm_destroy(realm);
    }

    return realm;
}

int rest_authorization_settings_load(linked_list_t *realms,
                                     json_t *j_settings)
{
    rest_realm_t *realm;
    json_t *j_value;
    size_t index;

    json_array_foreach(j_settings, index, j_value)
    {
        realm = rest_realm_create(j_value);
        if (realm != NULL)
            linked_list_add(realms, realm);
    }

    return 0;
}

int rest_authorization_settings_unload(linked_list_t *realms)
{
    linked_list_entry_t *list_entry;
    rest_realm_t *realm;

    linked_list_foreach(realms, list_entry)
    {
        realm = list_entry->data;
        rest_realm_destroy(realm);
    }

    return 0;
}
