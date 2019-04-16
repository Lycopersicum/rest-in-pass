#include "settings.h"

#include "password_keeper/password_keeper.h"
#include "rest/rest_core.h"
#include "rest/authorization_callbacks.h"
#include "rest/password_keeper_callbacks.h"

#include <ulfius.h>

int main(int argc, char *argv[])
{
    static settings_t settings = {
        .keeper = {
            .encryption_key = NULL,
            .master_password = NULL,
            .passwords_file = NULL,
        },
        .rest = {
            .port = 80,
            .realms = NULL,
        },
    };
    static rest_core_t rest_core;
    static password_keeper_t password_keeper;
    printf("created REST realms: %p\n", settings.rest.realms);

    if (settings_initialize(&settings, argc, argv) != 0)
        return -1;

    if (password_keeper_initialize(&password_keeper,
                                   &settings.keeper) != 0)
        return -1;

    if (rest_core_initialize(&rest_core, &settings.rest) != 0)
        return -1;

    if ((add_authorization_callbacks(&rest_core) != 0)
        || (add_password_keeper_callbacks(&rest_core,
                                      &password_keeper) != 0))
        return -1;

    if (rest_core_start(&rest_core) != 0)
        return -1;

    /* Wait for user keyboard input to kill server*/
    printf("%s Press <enter> to quit server\n", "[MAIN]");
    getchar();

    rest_core_terminate(&rest_core);
    password_keeper_terminate(&password_keeper);

    settings_terminate(&settings);

    return 0;
}
