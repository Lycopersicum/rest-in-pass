#ifndef REST_IN_PASS_PASSWORD_KEEPER_PASSWORD_H
#define REST_IN_PASS_PASSWORD_KEEPER_PASSWORD_H

#include <jansson.h> /* json_t, json_string_value, json_object_get, 
                        json_pack */

typedef struct {
    char *name;
    char *secret;
} password_t;

int password_json_validate(json_t *j_password);
password_t *password_from_json(json_t *j_password);
json_t *password_to_json(password_t *password);
void password_destroy(password_t *password);

const char *password_get_name(password_t *password);
const char *password_get_secret(password_t *password);

int password_set_name(password_t *password, const char *name);
int password_set_secret(password_t *password, const char *secret);

#endif /* REST_IN_PASS_PASSWORD_KEEPER_PASSWORD_H */
