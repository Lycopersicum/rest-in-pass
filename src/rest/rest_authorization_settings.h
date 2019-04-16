#ifndef REST_IN_PASS_REST_REST_AUTHORIZATION_SETTINGS_H
#define REST_IN_PASS_REST_REST_AUTHORIZATION_SETTINGS_H

#include <jansson.h> /* json_t, json_string_value, json_object_get,
                        json_array_foreach */

#include "../linked_list.h"

typedef struct {
    char *name;
    char *secret;
} rest_realm_t;

int rest_authorization_settings_load(linked_list_t *realms,
                                     json_t *j_settings);
int rest_authorization_settings_unload(linked_list_t *realms);

#endif /* REST_IN_PASS_REST_REST_AUTHORIZATION_SETTINGS_H */
