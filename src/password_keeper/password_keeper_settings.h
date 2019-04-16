#ifndef REST_IN_PASS_PASSWORD_KEEPER_PASSWORD_KEEPER_SETTINGS_H
#define REST_IN_PASS_PASSWORD_KEEPER_PASSWORD_KEEPER_SETTINGS_H

#include <jansson.h> /* json_t */

typedef struct {
    char *master_password;
    char *encryption_key;
    char *passwords_file;
} password_keeper_settings_t;

int password_keeper_settings_load(password_keeper_settings_t *settings,
                                  json_t *j_settings);
void password_keeper_settings_unload(password_keeper_settings_t *settings);

#endif /* REST_IN_PASS_PASSWORD_KEEPER_PASSWORD_KEEPER_SETTINGS_H */
