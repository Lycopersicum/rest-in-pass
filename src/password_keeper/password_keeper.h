#ifndef REST_IN_PASS_PASSWORD_KEEPER_PASSWORD_KEEPER_H
#define REST_IN_PASS_PASSWORD_KEEPER_PASSWORD_KEEPER_H

#include "../linked_list.h"
#include "password.h"
#include "password_keeper_settings.h"

typedef struct {
    password_keeper_settings_t *settings;
    linked_list_t *passwords;
} password_keeper_t;

int password_keeper_initialize(password_keeper_t *password_keeper,
                               password_keeper_settings_t *settings);
void password_keeper_terminate(password_keeper_t *password_keeper);

int password_keeper_create_password(password_keeper_t *password_keeper, 
                                    json_t *j_password);
const char *password_keeper_read_secret(password_keeper_t *password_keeper,
                                        const char *name);
int password_keeper_update_secret(password_keeper_t *password_keeper,
                                  const char *name,
                                  const char *secret);
int password_keeper_delete_password(password_keeper_t *password_keeper,
                                    const char *name);

#endif /* REST_IN_PASS_PASSWORD_KEEPER_PASSWORD_KEEPER_H */
