#include <string.h> /* strcasecmp */

#include "rest_settings.h"

#include "rest_authorization_settings.h"

static const char *logging_section = "[SETTINGS:REST]";

int rest_settings_load(rest_settings_t *settings, json_t *j_settings)
{
    const char *key;
    json_t *j_value;

    json_object_foreach(j_settings, key, j_value)
    {
        if (strcasecmp(key, "port") == 0)
        {
            settings->port = (uint16_t) json_integer_value(j_value);
        }
        else if (strcasecmp(key, "authorization") == 0)
        {
            rest_authorization_settings_load(settings->realms,
                                             j_value);
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

int rest_settings_unload(rest_settings_t *settings)
{
    rest_authorization_settings_unload(settings->realms);
    return 0;
}
