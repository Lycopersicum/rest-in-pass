#include <stdlib.h> /* malloc, free */
#include <string.h> /* strdup */

#include "password_keeper_settings.h"

static const char *logging_section = "[SETTINGS:KEEPER]";

void password_keeper_settings_unload(password_keeper_settings_t *settings)
{
    if (settings == NULL)
        return;

    if (settings->master_password != NULL)
        free(settings->master_password);

    if (settings->passwords_file != NULL)
        free(settings->passwords_file);
}

int password_keeper_settings_load(password_keeper_settings_t *settings,
                                  json_t *j_settings)
{
    const char *key;
    json_t *j_value;

    json_object_foreach(j_settings, key, j_value)
    {
        if (strcasecmp(key, "encryption_key") == 0)
        {
            settings->encryption_key = strdup(json_string_value(j_value));
        }
        else if (strcasecmp(key, "master_password") == 0)
        {
            settings->master_password = strdup(json_string_value(j_value));
        }
        else if (strcasecmp(key, "passwords_file") == 0)
        {
            settings->passwords_file = strdup(json_string_value(j_value));
        }
        else
        {
            fprintf(stdout,
                    "%s Unrecognised configuration file key: \"%s\"!\n",
                    logging_section, key);
        }
    }

    return 0;
}
