#ifndef REST_IN_PASS_REST_REST_SETTINGS_H
#define REST_IN_PASS_REST_REST_SETTINGS_H

#include <stdint.h> /* uint16_t */

#include <jansson.h> /* json_t, json_integer_value, json_object_foreach */

#include "../linked_list.h"

typedef struct {
    uint16_t port;
    linked_list_t *realms;
} rest_settings_t;

int rest_settings_load(rest_settings_t *settings, json_t *j_settings);
int rest_settings_unload(rest_settings_t *settings);

#endif /* REST_IN_PASS_REST_REST_SETTINGS_H */
