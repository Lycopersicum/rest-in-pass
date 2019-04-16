#include <string.h> /* strcasecmp */

#include <argp.h> /* error_t, argp_state, argp*/
#include <jansson.h> /* json_error_t, json_t, json_load_file,
                        json_object_foreach, json_decref */

#include "settings.h"

#include "linked_list.h"

static const char *logging_section = "[SETTINGS]";

static char doc[] =
    "REST In Pass - Password storage with REST API";
static struct argp_option options[] =
{
    { "config", 'c', "FILE", 0, "Specify configuration file" },
    { 0 }
};

static int load_config_file(settings_t *settings, char *config_file)
{
    json_error_t error;
    json_t *j_settings, *j_value;
    const char *section;

    j_settings = json_load_file(config_file, 0, &error);

    if (j_settings == NULL)
    {
        fprintf(stderr, "%s:%d:%d error:%s \n",
                config_file, error.line, error.column, error.text);
        return 1;
    }

    json_object_foreach(j_settings, section, j_value)
    {
        if (strcasecmp(section, "rest") == 0)
        {
            rest_settings_load(&settings->rest, j_value);
        }
        else if (strcasecmp(section, "keeper") == 0)
        {
            password_keeper_settings_load(&settings->keeper, j_value);
        }
        else
        {
            fprintf(stdout,
                    "%s Unrecognised configuration file section: \"%s\"!\n",
                    logging_section, section);
        }
    }

    json_decref(j_settings);
    return 0;
}

static error_t parse_opt(int key, char *arg, struct argp_state *state)
{
    settings_t *settings = state->input;
    
    switch (key)
    {
    case 'c':
        if (load_config_file(settings, arg) != 0)
            argp_usage(state);
        break;
    default:
        return ARGP_ERR_UNKNOWN;
    }
    return 0;
}

static struct argp argp = { options, parse_opt, NULL, doc};

int settings_initialize(settings_t *settings, int argc, char *argv[])
{
    settings->rest.realms = linked_list_create();
    return argp_parse(&argp, argc, argv, 0, 0, settings);
}

void settings_terminate(settings_t *settings)
{
	rest_settings_unload(&settings->rest);
    password_keeper_settings_unload(&settings->keeper);
    linked_list_destroy(settings->rest.realms);
}
