#include "rest_core.h"

#include "rest_settings.h"

const static char *logging_section = "[REST]";

int rest_core_initialize(rest_core_t *core, rest_settings_t *settings)
{
    int status;

    core->realms = settings->realms;
    fprintf(stdout, "%s Setting up REST core on port %d...\n",
            logging_section, settings->port);

    status = ulfius_init_instance(&core->ulfius, settings->port,
                                  NULL, NULL);
    if (status != U_OK)
    {
        fprintf(stderr, "%s Failed to initialize ulfius!\n",
                logging_section);
        return -1;
    }

    return 0;
}

int rest_core_start(rest_core_t *core)
{
    int status;

    status = ulfius_start_framework(&core->ulfius);

    if (status != U_OK)
    {
        fprintf(stderr, "%s Failed to start ulfius!\n",
                logging_section);
        return -1;
    }

    return 0;
}
int rest_core_terminate(rest_core_t *core)
{
    fprintf(stdout, "%s Terminating REST core...\n",
            logging_section);

    ulfius_stop_framework(&core->ulfius);
    ulfius_clean_instance(&core->ulfius);

    return 0;
}
