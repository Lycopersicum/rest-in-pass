#ifndef REST_IN_PASS_REST_REST_CORE_H
#define REST_IN_PASS_REST_REST_CORE_H

#include <ulfius.h> /* struct _u_instance, */

#include "rest_settings.h"

#include "../linked_list.h"

typedef struct {
    struct _u_instance ulfius;
    rest_settings_t *settings;
    linked_list_t *realms;
} rest_core_t;

int rest_core_initialize(rest_core_t *core, rest_settings_t *settings);
int rest_core_terminate(rest_core_t *core);

int rest_core_start(rest_core_t *core);
#endif /* REST_IN_PASS_REST_REST_CORE_H */
