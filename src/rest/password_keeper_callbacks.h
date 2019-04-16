#ifndef REST_IN_PASS_REST_PASSWORDS_KEEPER_CALLBACKS_H
#define REST_IN_PASS_REST_PASSWORDS_KEEPER_CALLBACKS_H

#include "../password_keeper/password_keeper.h" 
#include "rest_core.h" 

int add_password_keeper_callbacks(rest_core_t *core,
                                  password_keeper_t *password_keeper);

#endif /* REST_IN_PASS_REST_PASSWORDS_KEEPER_CALLBACKS_H */
