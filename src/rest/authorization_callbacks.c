#include <string.h> /* strdup, strcmp, strlen */
#include <stdio.h> /* fprintf, stdout */

#include "authorization_callbacks.h"

#include "../linked_list.h"
#include "rest_authorization_settings.h"

const static char *HEADER_AUTHORIZATION = "Authorization";
const static char *HEADER_UNAUTHORIZED = "WWW-Authenticate";
const static char *HEADER_AUTHORIZATION_PREFIX = "Basic ";

static char *get_request_authorization(const struct _u_request *u_request)
{
    char *token;
    const char *authorization_header;
    authorization_header = u_map_get(u_request->map_header,
                                     HEADER_AUTHORIZATION);

    if (authorization_header == NULL)
    {
        return NULL;
    }

    if (strncmp(authorization_header,
                HEADER_AUTHORIZATION_PREFIX,
                strlen(HEADER_AUTHORIZATION_PREFIX)) != 0)
    {
        return NULL;
    }

    token = (char *) authorization_header + strlen(HEADER_AUTHORIZATION_PREFIX);

    return token;
}

static char *get_request_realm(const struct _u_request *u_request)
{
    char *url, *url_to_free, *realm = NULL;

    url = strdup(u_request->http_url + 1);
    if (url != NULL)
    {
        url_to_free = url;
        realm = strdup(strsep(&url, "/"));
        free(url_to_free);
        return realm;
    }

    return NULL;
}

static int rest_realms_check_access(linked_list_t *realms,
                                    const char *name,
                                    const char *secret)
{
    rest_realm_t *realm;
    linked_list_entry_t *list_entry;
    int status = 0;

    linked_list_lock(realms);
    linked_list_foreach(realms, list_entry)
    {
        realm = list_entry->data;
        if (strcmp(realm->name, name) == 0)
        {
            if (strcmp(realm->secret, secret) == 0)
            {
                linked_list_unlock(realms);
                return 0;
            }
            /* Do not give access if realm is listed, otherwise, pass */
            status = -1;
        }
    }
    linked_list_unlock(realms);

    return status;
}

static void add_authorization_hint(struct _u_response *u_response,
                                   const char *realm)
{
    char authorization_hint[strlen(HEADER_AUTHORIZATION_PREFIX)
                            + strlen(realm) + strlen("realm=\"\"") + 2];
    snprintf(authorization_hint, sizeof(authorization_hint),
             "%s realm=\"%s\"", HEADER_AUTHORIZATION_PREFIX, realm);

    u_map_put(u_response->map_header, HEADER_UNAUTHORIZED,
              authorization_hint);
}

static int authorization_callback(const struct _u_request *u_request,
                                  struct _u_response *u_response,
                                  void *context)
{
    linked_list_t *realms = context;
    char *authorization_key, *realm;
    int status;

    realm = get_request_realm(u_request);
    if (realm == NULL)
    {
        return U_CALLBACK_ERROR;
    }

    authorization_key = get_request_authorization(u_request);
    if (authorization_key == NULL)
    {
        add_authorization_hint(u_response, realm);
        free(realm);
        return U_CALLBACK_UNAUTHORIZED;
    }

    status = rest_realms_check_access(realms,
                                      realm, authorization_key);
    free(realm);
    if (status == 0)
        return U_CALLBACK_CONTINUE;

    ulfius_set_empty_body_response(u_response, 403);
    return U_CALLBACK_COMPLETE;
}

int add_authorization_callbacks(rest_core_t *core)
{
    ulfius_add_endpoint_by_val(&core->ulfius, "*", "*", NULL,
                               2, authorization_callback, core->realms);
    return 0;
}
