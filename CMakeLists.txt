cmake_minimum_required(VERSION 3.10)
project(libxeddsa VERSION 2.0.1 LANGUAGES C)

include(CTest)

add_subdirectory(ref10)

if (WIN32)
    # On Windows, don't even try to organize the outputs logically
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

    foreach (OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
        string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG)
        set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${PROJECT_SOURCE_DIR}/bin)
        set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${PROJECT_SOURCE_DIR}/bin)
        set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${PROJECT_SOURCE_DIR}/bin)
    endforeach ()
else (WIN32)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/static)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin/dynamic)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

    foreach (OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
        string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG)
        set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${PROJECT_SOURCE_DIR}/bin/static)
        set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${PROJECT_SOURCE_DIR}/bin/dynamic)
        set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${PROJECT_SOURCE_DIR}/bin)
    endforeach ()
endif (WIN32)

if (WIN32)
    # When using the MinGW generator on Windows, the generated files are prefixed with
    # "lib" and use the extension ".a" instead of ".lib". This is not Windows convention.
    # Using following global settings it is possible to correct most of these issues.
    # Sadly, these global settings don't apply to import libraries. These have to be
    # corrected for each single target.
    set(CMAKE_STATIC_LIBRARY_PREFIX "")
    set(CMAKE_SHARED_LIBRARY_PREFIX "")

    set(CMAKE_STATIC_LIBRARY_SUFFIX ".lib")
endif (WIN32)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}")

if (DEFINED EMSCRIPTEN)
    set(sodium_USE_STATIC_LIBS YES)
    find_package(Sodium REQUIRED)
else ()
    set(sodium_USE_STATIC_LIBS NO)
    find_package(Sodium REQUIRED)

    if (WIN32)
        file(COPY "${sodium_DLL_DEBUG}" DESTINATION "${PROJECT_SOURCE_DIR}/bin/")
        get_filename_component(sodium_DLL_DEBUG_NAME "${sodium_DLL_DEBUG}" NAME)
        file(RENAME "${PROJECT_SOURCE_DIR}/bin/${sodium_DLL_DEBUG_NAME}" "${PROJECT_SOURCE_DIR}/bin/libsodium_debug.dll")

        file(COPY "${sodium_DLL_RELEASE}" DESTINATION "${PROJECT_SOURCE_DIR}/bin/")
        get_filename_component(sodium_DLL_RELEASE_NAME "${sodium_DLL_RELEASE}" NAME)
        file(RENAME "${PROJECT_SOURCE_DIR}/bin/${sodium_DLL_RELEASE_NAME}" "${PROJECT_SOURCE_DIR}/bin/libsodium_release.dll")
    endif ()
endif ()

include_directories(include)
include_directories(ref10/include)
include_directories(${sodium_INCLUDE_DIR})

##########
# xeddsa #
##########

set(xeddsa_sources
    src/ed25519.c
    src/pub_conversion.c
    src/pub_derivation.c
    src/seed_priv_utils.c
    src/utils.c
    src/version.c
    src/x25519.c
    src/xeddsa.c
)

add_library(xeddsa_objects OBJECT ${xeddsa_sources})

set_property(TARGET xeddsa_objects PROPERTY POSITION_INDEPENDENT_CODE TRUE)
target_compile_definitions(xeddsa_objects PRIVATE SODIUM_DLL_EXPORT)
target_compile_definitions(xeddsa_objects PRIVATE BUILD)

add_library(xeddsa_static STATIC $<TARGET_OBJECTS:xeddsa_objects>)
if (NOT WIN32)
    set_target_properties(xeddsa_static PROPERTIES OUTPUT_NAME xeddsa)
endif ()
target_link_libraries(xeddsa_static PUBLIC
    xeddsa_objects
    crypto_sign_objects
    crypto_hash_objects
    crypto_hashblocks_objects
    crypto_verify_objects
    fastrandombytes_objects
    crypto_rng_objects
    crypto_stream_objects
    crypto_core_objects
    kernelrandombytes_objects
    sodium
)

install(TARGETS xeddsa_static)

if (NOT DEFINED EMSCRIPTEN)
    add_library(xeddsa_dynamic SHARED $<TARGET_OBJECTS:xeddsa_objects>)
    set_target_properties(xeddsa_dynamic PROPERTIES OUTPUT_NAME xeddsa)
    set_target_properties(xeddsa_dynamic PROPERTIES
       VERSION ${CMAKE_PROJECT_VERSION}
       SOVERSION ${CMAKE_PROJECT_VERSION_MAJOR}
    )

    target_link_libraries(xeddsa_dynamic PUBLIC
        xeddsa_objects
        crypto_sign_objects
        crypto_hash_objects
        crypto_hashblocks_objects
        crypto_verify_objects
        fastrandombytes_objects
        crypto_rng_objects
        crypto_stream_objects
        crypto_core_objects
        kernelrandombytes_objects
        sodium
    )

    if (WIN32)
        # Correct the import library.
        set_target_properties(xeddsa_dynamic PROPERTIES IMPORT_PREFIX "")
        set_target_properties(xeddsa_dynamic PROPERTIES IMPORT_SUFFIX ".lib")
    endif (WIN32)

    install(TARGETS xeddsa_dynamic)
endif ()

##########################
# xeddsa test executable #
##########################

set(xeddsa_test_sources
    test/test_conversion_uniqueness.c
    test/test_pub_derivation.c
    test/test_signing.c
    test/test.c
)

add_library(xeddsa_test_objects OBJECT ${xeddsa_test_sources})

target_compile_definitions(xeddsa_test_objects PRIVATE SODIUM_DLL_EXPORT)
target_compile_definitions(xeddsa_test_objects PRIVATE BUILD)
target_include_directories(xeddsa_test_objects PRIVATE test)

add_executable(xeddsa_test_static $<TARGET_OBJECTS:xeddsa_test_objects>)
target_include_directories(xeddsa_test_static PRIVATE test)
target_link_libraries(xeddsa_test_static xeddsa_static sodium)

if (NOT DEFINED EMSCRIPTEN)
    add_executable(xeddsa_test_dynamic $<TARGET_OBJECTS:xeddsa_test_objects>)
    target_include_directories(xeddsa_test_dynamic PRIVATE test)
    target_link_libraries(xeddsa_test_dynamic xeddsa_dynamic sodium)
endif ()

################
# DEPENDENCIES #
################

target_include_directories(xeddsa_objects PUBLIC
    ref10/crypto_sign
    ref10/crypto_hash
    ref10/crypto_hashblocks
    ref10/crypto_verify
    ref10/fastrandombytes
    ref10/crypto_rng
    ref10/crypto_stream
    ref10/crypto_core
    ref10/kernelrandombytes
)

#########
# TESTS #
#########

if (BUILD_TESTING)
    add_test(NAME xeddsa_test_static COMMAND xeddsa_test_static)

    if (NOT DEFINED EMSCRIPTEN)
        add_test(NAME xeddsa_test_dynamic COMMAND xeddsa_test_dynamic)
    endif ()
endif ()
