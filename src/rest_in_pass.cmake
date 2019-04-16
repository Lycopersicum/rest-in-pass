# Provides REST_IN_PASS_SOURCES, REST_IN_PASS_SOURCES_DIR variables

include(${CMAKE_CURRENT_LIST_DIR}/password_keeper/password_keeper.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/rest/rest.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/utils/utils.cmake)

set(REST_IN_PASS_SOURCES_DIR ${CMAKE_CURRENT_LIST_DIR})

set(REST_IN_PASS_SOURCES
    ${REST_IN_PASS_SOURCES}
    ${REST_IN_PASS_SOURCES_DIR}/linked_list.c
    ${REST_IN_PASS_SOURCES_DIR}/rest_in_pass.c
    ${REST_IN_PASS_SOURCES_DIR}/settings.c
    )

set(REST_IN_PASS_SOURCES
    ${REST_IN_PASS_SOURCES}
    ${PASSWORD_KEEPER_SOURCES}
    ${REST_SOURCES}
    ${UTILS_SOURCES}
    )
    
