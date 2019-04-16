#ifndef REST_IN_PASS_SETTINGS_H
#define REST_IN_PASS_SETTINGS_H

#include "password_keeper/password_keeper.h"
#include "rest/rest_settings.h"

typedef struct {
    password_keeper_settings_t keeper;
    rest_settings_t rest;
} settings_t;

int settings_initialize(settings_t *settings, int argc, char *argv[]);
void settings_terminate(settings_t *settings);

#endif /* REST_IN_PASS_SETTINGS_H */
