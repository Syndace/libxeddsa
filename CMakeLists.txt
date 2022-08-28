cmake_minimum_required(VERSION 3.5)
project(libxeddsa)

include(CTest)

add_subdirectory(ref10)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

foreach (OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${PROJECT_SOURCE_DIR}/bin)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${PROJECT_SOURCE_DIR}/bin)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} ${PROJECT_SOURCE_DIR}/bin)
endforeach ()

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

set(sodium_USE_STATIC_LIBS YES)
find_package(Sodium REQUIRED)
set(sodium_static_INCLUDE_DIR ${sodium_INCLUDE_DIR})

if (NOT DEFINED EMSCRIPTEN)
    set(sodium_USE_STATIC_LIBS NO)
    find_package(Sodium REQUIRED)
    set(sodium_dynamic_INCLUDE_DIR ${sodium_INCLUDE_DIR})

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
include_directories(${sodium_static_INCLUDE_DIR})
if (NOT DEFINED EMSCRIPTEN)
    include_directories(${sodium_dynamic_INCLUDE_DIR})
endif ()

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

if (NOT DEFINED EMSCRIPTEN)
    add_library(xeddsa_dynamic SHARED $<TARGET_OBJECTS:xeddsa_objects>)

    if (WIN32)
        # Correct the import library.
        set_target_properties(xeddsa_dynamic PROPERTIES IMPORT_PREFIX "")
        set_target_properties(xeddsa_dynamic PROPERTIES IMPORT_SUFFIX ".lib")
    endif (WIN32)
endif ()

####################
# combined library #
####################

add_library(combined_static STATIC CMakeLists.txt)
set_target_properties(combined_static PROPERTIES LINKER_LANGUAGE C)
target_link_libraries(combined_static PUBLIC
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
    sodium_static
)

if (NOT DEFINED EMSCRIPTEN)
    add_library(combined_dynamic SHARED CMakeLists.txt)
    set_target_properties(combined_dynamic PROPERTIES LINKER_LANGUAGE C)
    target_link_libraries(combined_dynamic PUBLIC
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
        sodium_shared
    )
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
target_link_libraries(xeddsa_test_static combined_static sodium_static)

if (NOT DEFINED EMSCRIPTEN)
    add_executable(xeddsa_test_dynamic $<TARGET_OBJECTS:xeddsa_test_objects>)
    target_include_directories(xeddsa_test_dynamic PRIVATE test)
    target_link_libraries(xeddsa_test_dynamic combined_dynamic sodium_shared)
endif ()

################
# DEPENDENCIES #
################

target_link_libraries(xeddsa_static crypto_sign_static sodium_static)

if (NOT DEFINED EMSCRIPTEN)
    target_link_libraries(xeddsa_dynamic crypto_sign_dynamic sodium_shared)
endif ()

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
