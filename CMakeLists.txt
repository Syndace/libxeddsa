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
find_package(Sodium REQUIRED)

include_directories(include)
include_directories(${sodium_INCLUDE_DIR})

##########
# xeddsa #
##########

set(xeddsa_sources
    src/xeddsa.c
    src/curve25519_sign.c
    src/curve25519_pub_to_ed25519_pub.c
)

add_library(xeddsa_objects OBJECT ${xeddsa_sources})

set_property(TARGET xeddsa_objects PROPERTY POSITION_INDEPENDENT_CODE TRUE)
target_compile_definitions(xeddsa_objects PRIVATE BUILD)

add_library(xeddsa_static  STATIC $<TARGET_OBJECTS:xeddsa_objects>)
add_library(xeddsa_dynamic SHARED $<TARGET_OBJECTS:xeddsa_objects>)

if (WIN32)
    # Correct the import library.
    set_target_properties(xeddsa_dynamic PROPERTIES IMPORT_PREFIX "")
    set_target_properties(xeddsa_dynamic PROPERTIES IMPORT_SUFFIX ".lib")
endif (WIN32)

##########################
# xeddsa test executable #
##########################

set(xeddsa_test_sources
    test/test.c
    test/test_libsodium_compatibility.c
    test/test_conversion_uniqueness.c
)

add_library(xeddsa_test_objects OBJECT ${xeddsa_test_sources})

target_compile_definitions(xeddsa_test_objects PRIVATE BUILD)
target_include_directories(xeddsa_test_objects PRIVATE test)

add_executable(xeddsa_test_static  $<TARGET_OBJECTS:xeddsa_test_objects>)
add_executable(xeddsa_test_dynamic $<TARGET_OBJECTS:xeddsa_test_objects>)

target_include_directories(xeddsa_test_static  PRIVATE test)
target_include_directories(xeddsa_test_dynamic PRIVATE test)

target_link_libraries(xeddsa_test_static  xeddsa_static)
target_link_libraries(xeddsa_test_dynamic xeddsa_dynamic)

################
# DEPENDENCIES #
################

target_link_libraries(xeddsa_static  crypto_sign_static  sodium)
target_link_libraries(xeddsa_dynamic crypto_sign_dynamic sodium)
target_include_directories(xeddsa_objects PUBLIC
    ref10/include
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
    add_test(NAME xeddsa_test_static  COMMAND xeddsa_test_static)
    add_test(NAME xeddsa_test_dynamic COMMAND xeddsa_test_dynamic)
endif ()
