#include <string.h> /* strcasecmp */
// TODO: remove following line after testing
#include <stdio.h>

#include "password_keeper_callbacks.h"

static int create_password_cb(const struct _u_request *u_request,
                              struct _u_response *u_response,
                              void *context)
{
    password_keeper_t *password_keeper = context;
    json_t *j_password;
    const char *name, *content_type;

    content_type = u_map_get_case(u_request->map_header, "Content-Type");
    if ((content_type == NULL)
        || (strcasecmp(content_type, "application/json") != 0))
    {
        ulfius_set_empty_body_response(u_response, 415);
        return U_CALLBACK_COMPLETE;
    }

    j_password = json_loadb(u_request->binary_body,
                            u_request->binary_body_length,
                            0, NULL);

    if (password_json_validate(j_password) != 0)
    {
        ulfius_set_empty_body_response(u_response, 400);
        json_decref(j_password);
        return U_CALLBACK_COMPLETE;
    }

    name = json_string_value(json_object_get(j_password, "name"));

    if (password_keeper_read_secret(password_keeper, name) != NULL)
    {
        ulfius_set_empty_body_response(u_response, 409);
    }
    else if (password_keeper_create_password(password_keeper,
                                             j_password) == 0)
    {
        ulfius_set_empty_body_response(u_response, 201);
    }
    else
    {
        json_decref(j_password);
        return U_CALLBACK_ERROR;
    }

    json_decref(j_password);
    return U_CALLBACK_COMPLETE;
}

static int read_password_cb(const struct _u_request *u_request,
                            struct _u_response *u_response,
                            void *context)
{
    password_keeper_t *password_keeper = context;
    const char *secret, *name;

    name = u_map_get(u_request->map_url, "name");
    if (name == NULL)
    {
        ulfius_set_empty_body_response(u_response, 400);
        return U_CALLBACK_COMPLETE;
    } 

    secret = password_keeper_read_secret(password_keeper, name);
    if (secret == NULL)
        ulfius_set_empty_body_response(u_response, 404);
    else
    {
        ulfius_add_header_to_response(u_response,
                                      "Content-Type", "text/plain");
        ulfius_set_string_body_response(u_response, 200, secret);
    }

    return U_CALLBACK_COMPLETE;
}

static int update_password_cb(const struct _u_request *u_request,
                              struct _u_response *u_response,
                              void *context)
{
    password_keeper_t *password_keeper = context;
    const char *content_type, *name;
    char secret[u_request->binary_body_length + 1];

    name = u_map_get(u_request->map_url, "name");
    if (name == NULL)
    {
        ulfius_set_empty_body_response(u_response, 400);
        return U_CALLBACK_COMPLETE;
    } 

    content_type = u_map_get_case(u_request->map_header, "Content-Type");
    if ((content_type == NULL)
        || (strcasecmp(content_type, "text/plain") != 0))
    {
        ulfius_set_empty_body_response(u_response, 415);
        return U_CALLBACK_COMPLETE;
    }

    memcpy(secret, u_request->binary_body, u_request->binary_body_length);
    secret[u_request->binary_body_length] = 0;

    if (secret == NULL)
        ulfius_set_empty_body_response(u_response, 400);
    else if (password_keeper_read_secret(password_keeper, name) == NULL)
        ulfius_set_empty_body_response(u_response, 404);
    else if (password_keeper_update_secret(password_keeper,
                                           name, secret) == 0)
        ulfius_set_empty_body_response(u_response, 204);
    else
        return U_CALLBACK_ERROR;

    return U_CALLBACK_COMPLETE;
}

static int delete_password_cb(const struct _u_request *u_request,
                              struct _u_response *u_response,
                              void *context)
{
    password_keeper_t *password_keeper = context;
    const char *name = u_map_get(u_request->map_url, "name");
    if (name == NULL)
    {
        ulfius_set_empty_body_response(u_response, 400);
        return U_CALLBACK_COMPLETE;
    } 

    if (password_keeper_read_secret(password_keeper, name) == NULL)
    {
        ulfius_set_empty_body_response(u_response, 404);
        return U_CALLBACK_COMPLETE;
    }

    if (password_keeper_delete_password(password_keeper, name) == 0)
        ulfius_set_empty_body_response(u_response, 204);
    else
        return U_CALLBACK_ERROR;

    return U_CALLBACK_COMPLETE;
}

int add_password_keeper_callbacks(rest_core_t *core,
                                  password_keeper_t *password_keeper)
{
    ulfius_add_endpoint_by_val(&core->ulfius, "POST", "/keeper", NULL, 10,
                               create_password_cb, password_keeper);
    ulfius_add_endpoint_by_val(&core->ulfius, "GET", "/keeper", ":name", 10,
                               read_password_cb, password_keeper);
    ulfius_add_endpoint_by_val(&core->ulfius, "PUT", "/keeper", ":name", 10,
                               update_password_cb, password_keeper);
    ulfius_add_endpoint_by_val(&core->ulfius, "DELETE", "/keeper", ":name",
                               10, delete_password_cb, password_keeper);

    return 0;
}
