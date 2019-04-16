# Provides REST_SOURCES, REST_SOURCES_DIR variables

set(REST_SOURCES_DIR ${CMAKE_CURRENT_LIST_DIR})

set(REST_SOURCES
    ${REST_SOURCES}
    ${REST_SOURCES_DIR}/rest_core.c
    ${REST_SOURCES_DIR}/authorization_callbacks.c
    ${REST_SOURCES_DIR}/password_keeper_callbacks.c
    ${REST_SOURCES_DIR}/rest_settings.c
    ${REST_SOURCES_DIR}/rest_authorization_settings.c
    )
