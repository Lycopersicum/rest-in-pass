cmake_minimum_required (VERSION 3.5)

project (rest-in-pass)

option(CODE_COVERAGE "Enable code coverage" OFF)
option(DEBUG "Enable debug options" OFF)

if(DTLS)
    message(FATAL_ERROR "DTLS option is not supported." )
endif()

include_directories(${CMAKE_INSTALL_PREFIX}/include)
set(THIRD_PARTY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/third_party")

set(ULFIUS_CMAKE_MODULES_DIR "${THIRD_PARTY_DIR}/ulfius/cmake-modules")

list(APPEND CMAKE_MODULE_PATH ${ULFIUS_CMAKE_MODULES_DIR})

include(${CMAKE_CURRENT_LIST_DIR}/src/rest_in_pass.cmake)

add_executable(${PROJECT_NAME} ${REST_IN_PASS_SOURCES})

# Find SHARED libraries
set(CMAKE_FIND_LIBRARY_SUFFIXES
    ${CMAKE_SHARED_LIBRARY_SUFFIX}
    ${CMAKE_SHARED_MODULE_SUFFIX}
    ${CMAKE_STATIC_LIBRARY_SUFFIX}
    )

# Nettle and Hogweed comes together, it depends on gmp
find_library(GMP_LIBRARY gmp)
find_library(NETTLE_LIBRARY nettle)
find_library(HOGWEED_LIBRARY hogweed)
set(NETTLE_LIBRARIES ${HOGWEED_LIBRARY} ${NETTLE_LIBRARY} ${GMP_LIBRARY})

# GnuTLS depends on nettle, libtasn1, hogweed, z, unistring (p11-kit if not built manually),
# if find_package doesn't add those libraries, it must be done manually.
find_library(ASN1_LIBRARY tasn1)
find_library(P11-KIT_LIBRARY p11-kit)
find_library(Z_LIBRARY z)
find_package(GnuTLS REQUIRED)
set(GNUTLS_LIBRARIES ${GNUTLS_LIBRARIES} ${NETTLE_LIBRARIES} ${UNISTRING_LIBRARY} ${Z_LIBRARY} ${ASN1_LIBRARY} ${P11-KIT_LIBRARY} ${IDN2_LIBRARY})

# Find STATIC libraries
set(CMAKE_FIND_LIBRARY_SUFFIXES
    ${CMAKE_STATIC_LIBRARY_SUFFIX}
    ${CMAKE_SHARED_LIBRARY_SUFFIX}
    ${CMAKE_SHARED_MODULE_SUFFIX}
    )

# Find and link Threads package libraries (pthread on linux)
find_package(Threads REQUIRED)
# Inform about libraries linked to define Threads and its symbols
message("Threads: ${CMAKE_THREAD_LIBS_INIT}")
set(LIBRARIES ${LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})

# Jansson
find_package(Jansson REQUIRED)
set(LIBRARIES ${LIBRARIES} ${JANSSON_LIBRARIES})

# curl depends on gnutls, (idn2, rtmp, ares, ldap if not built manually)
find_package(CURL REQUIRED)
set(CURL_LIBRARIES ${CURL_LIBRARIES} ${GNUTLS_LIBRARIES})

# gcrypt depends on gpg-error
find_library(GPG-ERROR_LIBRARY gpg-error)
find_library(GCRYPT_LIBRARY gcrypt)
set(GCRYPT_LIBRARIES ${GCRYPT_LIBRARY} ${GPG-ERROR_LIBRARY})

# MicroHTTPD depends on gnutls, gcrypt, pthread (libcurl if not built manually)
find_package(MHD REQUIRED)
set(MHD_LIBRARIES ${MHD_LIBRARIES} ${GNUTLS_LIBRARIES} ${CURL_LIBRARIES} ${GCRYPT_LIBRARIES}  ${CMAKE_THREAD_LIBS_INIT})

# Orcania depends on jansson
find_package(Orcania REQUIRED)
set(ORCANIA_LIBRARIES ${ORCANIA_LIBRARIES} ${ORCANIA_LIBRARIES} ${JANSSON_LIBRARIES})
find_library(ULFIUS_LIBRARY ulfius)

# Ulfius depends on liborcania, libcurl, jansson, libmicrohttpd > 0.9.51 (libyder if not built manually)
set(ULFIUS_LIBRARIES ${ULFIUS_LIBRARY} ${ORCANIA_LIBRARIES} ${MHD_LIBRARIES})
set(LIBRARIES ${LIBRARIES} ${ULFIUS_LIBRARIES})

message("ALL libraries: ${LIBRARIES}")
target_link_libraries(${PROJECT_NAME} ${LIBRARIES})

target_compile_options(${PROJECT_NAME} PRIVATE "-Wall")
if(DEBUG)
    target_compile_options(${PROJECT_NAME} PRIVATE "-Wall" "-O" "-g3")
endif()

if(CODE_COVERAGE)
    target_compile_options(${PROJECT_NAME} PRIVATE "-coverage")
    target_link_libraries(${PROJECT_NAME} "gcov")
endif()
