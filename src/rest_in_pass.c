#include <signal.h> /* struct sigaction, sigemptyset, sigaction,
                       SA_RESTART, SIGINT, SIGTERM */
#include <errno.h> /* errno */
#include <string.h> /* memset, strerror */

#include <ulfius.h>

#include "password_keeper/password_keeper.h"
#include "rest/rest_core.h"
#include "rest/authorization_callbacks.h"
#include "rest/password_keeper_callbacks.h"
#include "settings.h"

static volatile int quit;

static void sigint_handler(int signo)
{
    quit = 1;
}

static void sigpipe_handler(int sig)
{
    static volatile int sigpipe_count;
    sigpipe_count++;
}

static void init_signals(void)
{
    struct sigaction oldsig, sig;

    memset(&sig, 0, sizeof(sig));
    sig.sa_handler = &sigint_handler;
    sigemptyset(&sig.sa_mask);
    sig.sa_flags = 0;
    if (sigaction(SIGINT, &sig, &oldsig) != 0)
        fprintf(stderr, "Failed to install SIGINT handler: %s\n",
                strerror(errno));

    //to stop valgrind
    if (sigaction(SIGTERM, &sig, &oldsig) != 0)
        fprintf(stderr, "Failed to install SIGTERM handler: %s\n",
                strerror(errno));

    memset(&sig, 0, sizeof(sig));
    sig.sa_handler = &sigpipe_handler;
    sigemptyset(&sig.sa_mask);
    sig.sa_flags = SA_RESTART;
    if (sigaction(SIGPIPE, &sig, &oldsig) != 0)
        fprintf(stderr, "Failed to install SIGPIPE handler: %s\n",
                strerror(errno));
}

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

    init_signals();

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

    while(!quit) 
    {
    }

    rest_core_terminate(&rest_core);
    password_keeper_terminate(&password_keeper);

    settings_terminate(&settings);

    return 0;
}
