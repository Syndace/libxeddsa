cmake_minimum_required(VERSION 3.5)
project(ref10)

include(CTest)

include(CheckSymbolExists)

if (UNIX)
    CHECK_SYMBOL_EXISTS(getentropy    "sys/random.h"            RANDOM_SOURCE_getentropy)
    CHECK_SYMBOL_EXISTS(getentropy    "unistd.h"                RANDOM_SOURCE_getentropy2)
    CHECK_SYMBOL_EXISTS(getrandom     "unistd.h;sys/random.h"   RANDOM_SOURCE_getrandom)
    CHECK_SYMBOL_EXISTS(getrandom     "unistd.h;linux/random.h" RANDOM_SOURCE_getrandom2)
    CHECK_SYMBOL_EXISTS(SYS_getrandom "unistd.h;syscall.h"      RANDOM_SOURCE_getrandom3)

    if (RANDOM_SOURCE_getentropy OR RANDOM_SOURCE_getentropy2 OR RANDOM_SOURCE_getrandom OR RANDOM_SOURCE_getrandom2 OR RANDOM_SOURCE_getrandom3)
        set(RANDOM_SOURCE_urandom FALSE)
    else ()
        set(RANDOM_SOURCE_urandom TRUE)
    endif ()
else (UNIX)
    set(RANDOM_SOURCE_getentropy  FALSE)
    set(RANDOM_SOURCE_getentropy2 FALSE)
    set(RANDOM_SOURCE_getrandom   FALSE)
    set(RANDOM_SOURCE_getrandom2  FALSE)
    set(RANDOM_SOURCE_getrandom3  FALSE)
    set(RANDOM_SOURCE_urandom     FALSE)
endif (UNIX)

if (WIN32)
    set(RANDOM_SOURCE_rtlgenrandom TRUE)
else (WIN32)
    set(RANDOM_SOURCE_rtlgenrandom FALSE)
endif (WIN32)

if (NOT (UNIX OR WIN32))
    message(FATAL_ERROR "Unsupported operating system (neither UNIX nor Windows). CMake will exit.")
endif ()

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

include_directories(include)

###############
# crypto_core #
###############

set(crypto_core_sources
    crypto_core/core.c
)

add_library(crypto_core_objects OBJECT ${crypto_core_sources})

set_property(TARGET crypto_core_objects PROPERTY POSITION_INDEPENDENT_CODE TRUE)
target_compile_definitions(crypto_core_objects PRIVATE BUILD)
target_include_directories(crypto_core_objects PUBLIC  crypto_core)

add_library(crypto_core_static  STATIC $<TARGET_OBJECTS:crypto_core_objects>)
add_library(crypto_core_dynamic SHARED $<TARGET_OBJECTS:crypto_core_objects>)

if (WIN32)
    # Correct the import library.
    set_target_properties(crypto_core_dynamic PROPERTIES IMPORT_PREFIX "")
    set_target_properties(crypto_core_dynamic PROPERTIES IMPORT_SUFFIX ".lib")
endif (WIN32)

###############
# crypto_hash #
###############

set(crypto_hash_sources
    crypto_hash/hash.c
)

add_library(crypto_hash_objects OBJECT ${crypto_hash_sources})

set_property(TARGET crypto_hash_objects PROPERTY POSITION_INDEPENDENT_CODE TRUE)
target_compile_definitions(crypto_hash_objects PRIVATE BUILD)
target_include_directories(crypto_hash_objects PUBLIC  crypto_hash)

add_library(crypto_hash_static  STATIC $<TARGET_OBJECTS:crypto_hash_objects>)
add_library(crypto_hash_dynamic SHARED $<TARGET_OBJECTS:crypto_hash_objects>)

if (WIN32)
    # Correct the import library.
    set_target_properties(crypto_hash_dynamic PROPERTIES IMPORT_PREFIX "")
    set_target_properties(crypto_hash_dynamic PROPERTIES IMPORT_SUFFIX ".lib")
endif (WIN32)

#####################
# crypto_hashblocks #
#####################

set(crypto_hashblocks_sources
    crypto_hashblocks/blocks.c
)

add_library(crypto_hashblocks_objects OBJECT ${crypto_hashblocks_sources})

set_property(TARGET crypto_hashblocks_objects PROPERTY POSITION_INDEPENDENT_CODE TRUE)
target_compile_definitions(crypto_hashblocks_objects PRIVATE BUILD)
target_include_directories(crypto_hashblocks_objects PUBLIC  crypto_hashblocks)

add_library(crypto_hashblocks_static  STATIC $<TARGET_OBJECTS:crypto_hashblocks_objects>)
add_library(crypto_hashblocks_dynamic SHARED $<TARGET_OBJECTS:crypto_hashblocks_objects>)

if (WIN32)
    # Correct the import library.
    set_target_properties(crypto_hashblocks_dynamic PROPERTIES IMPORT_PREFIX "")
    set_target_properties(crypto_hashblocks_dynamic PROPERTIES IMPORT_SUFFIX ".lib")
endif (WIN32)

##############
# crypto_rng #
##############

set(crypto_rng_sources
    crypto_rng/rng.c
)

add_library(crypto_rng_objects OBJECT ${crypto_rng_sources})

set_property(TARGET crypto_rng_objects PROPERTY POSITION_INDEPENDENT_CODE TRUE)
target_compile_definitions(crypto_rng_objects PRIVATE BUILD)
target_include_directories(crypto_rng_objects PUBLIC  crypto_rng)

add_library(crypto_rng_static  STATIC $<TARGET_OBJECTS:crypto_rng_objects>)
add_library(crypto_rng_dynamic SHARED $<TARGET_OBJECTS:crypto_rng_objects>)

if (WIN32)
    # Correct the import library.
    set_target_properties(crypto_rng_dynamic PROPERTIES IMPORT_PREFIX "")
    set_target_properties(crypto_rng_dynamic PROPERTIES IMPORT_SUFFIX ".lib")
endif (WIN32)

#####################
# crypto_scalarmult #
#####################

set(crypto_scalarmult_sources
    crypto_scalarmult/base.c
    crypto_scalarmult/fe_0.c
    crypto_scalarmult/fe_1.c
    crypto_scalarmult/fe_add.c
    crypto_scalarmult/fe_copy.c
    crypto_scalarmult/fe_cswap.c
    crypto_scalarmult/fe_frombytes.c
    crypto_scalarmult/fe_invert.c
    crypto_scalarmult/fe_mul121666.c
    crypto_scalarmult/fe_mul.c
    crypto_scalarmult/fe_sq.c
    crypto_scalarmult/fe_sub.c
    crypto_scalarmult/fe_tobytes.c
    crypto_scalarmult/scalarmult.c
)

add_library(crypto_scalarmult_objects OBJECT ${crypto_scalarmult_sources})

set_property(TARGET crypto_scalarmult_objects PROPERTY POSITION_INDEPENDENT_CODE TRUE)
target_compile_definitions(crypto_scalarmult_objects PRIVATE BUILD)
target_include_directories(crypto_scalarmult_objects PUBLIC  crypto_scalarmult)

add_library(crypto_scalarmult_static  STATIC $<TARGET_OBJECTS:crypto_scalarmult_objects>)
add_library(crypto_scalarmult_dynamic SHARED $<TARGET_OBJECTS:crypto_scalarmult_objects>)

if (WIN32)
    # Correct the import library.
    set_target_properties(crypto_scalarmult_dynamic PROPERTIES IMPORT_PREFIX "")
    set_target_properties(crypto_scalarmult_dynamic PROPERTIES IMPORT_SUFFIX ".lib")
endif (WIN32)

###############
# crypto_sign #
###############

set(crypto_sign_sources
    crypto_sign/fe_0.c
    crypto_sign/fe_1.c
    crypto_sign/fe_add.c
    crypto_sign/fe_cmov.c
    crypto_sign/fe_copy.c
    crypto_sign/fe_frombytes.c
    crypto_sign/fe_invert.c
    crypto_sign/fe_isnegative.c
    crypto_sign/fe_isnonzero.c
    crypto_sign/fe_mul.c
    crypto_sign/fe_neg.c
    crypto_sign/fe_pow22523.c
    crypto_sign/fe_sq2.c
    crypto_sign/fe_sq.c
    crypto_sign/fe_sub.c
    crypto_sign/fe_tobytes.c
    crypto_sign/ge_add.c
    crypto_sign/ge_double_scalarmult.c
    crypto_sign/ge_frombytes.c
    crypto_sign/ge_madd.c
    crypto_sign/ge_msub.c
    crypto_sign/ge_p1p1_to_p2.c
    crypto_sign/ge_p1p1_to_p3.c
    crypto_sign/ge_p2_0.c
    crypto_sign/ge_p2_dbl.c
    crypto_sign/ge_p3_0.c
    crypto_sign/ge_p3_dbl.c
    crypto_sign/ge_p3_tobytes.c
    crypto_sign/ge_p3_to_cached.c
    crypto_sign/ge_p3_to_p2.c
    crypto_sign/ge_precomp_0.c
    crypto_sign/ge_scalarmult_base.c
    crypto_sign/ge_sub.c
    crypto_sign/ge_tobytes.c
    crypto_sign/keypair.c
    crypto_sign/open.c
    crypto_sign/sc_muladd.c
    crypto_sign/sc_reduce.c
    crypto_sign/sign.c
)

add_library(crypto_sign_objects OBJECT ${crypto_sign_sources})

set_property(TARGET crypto_sign_objects PROPERTY POSITION_INDEPENDENT_CODE TRUE)
target_compile_definitions(crypto_sign_objects PRIVATE BUILD)
target_include_directories(crypto_sign_objects PUBLIC  crypto_sign)

add_library(crypto_sign_static  STATIC $<TARGET_OBJECTS:crypto_sign_objects>)
add_library(crypto_sign_dynamic SHARED $<TARGET_OBJECTS:crypto_sign_objects>)

if (WIN32)
    # Correct the import library.
    set_target_properties(crypto_sign_dynamic PROPERTIES IMPORT_PREFIX "")
    set_target_properties(crypto_sign_dynamic PROPERTIES IMPORT_SUFFIX ".lib")
endif (WIN32)

#################
# crypto_stream #
#################

set(crypto_stream_sources
    crypto_stream/stream.c
    crypto_stream/xor.c
)

add_library(crypto_stream_objects OBJECT ${crypto_stream_sources})

set_property(TARGET crypto_stream_objects PROPERTY POSITION_INDEPENDENT_CODE TRUE)
target_compile_definitions(crypto_stream_objects PRIVATE BUILD)
target_include_directories(crypto_stream_objects PUBLIC  crypto_stream)

add_library(crypto_stream_static  STATIC $<TARGET_OBJECTS:crypto_stream_objects>)
add_library(crypto_stream_dynamic SHARED $<TARGET_OBJECTS:crypto_stream_objects>)

if (WIN32)
    # Correct the import library.
    set_target_properties(crypto_stream_dynamic PROPERTIES IMPORT_PREFIX "")
    set_target_properties(crypto_stream_dynamic PROPERTIES IMPORT_SUFFIX ".lib")
endif (WIN32)

#################
# crypto_verify #
#################

set(crypto_verify_sources
    crypto_verify/verify.c
)

add_library(crypto_verify_objects OBJECT ${crypto_verify_sources})

set_property(TARGET crypto_verify_objects PROPERTY POSITION_INDEPENDENT_CODE TRUE)
target_compile_definitions(crypto_verify_objects PRIVATE BUILD)
target_include_directories(crypto_verify_objects PUBLIC  crypto_verify)

add_library(crypto_verify_static  STATIC $<TARGET_OBJECTS:crypto_verify_objects>)
add_library(crypto_verify_dynamic SHARED $<TARGET_OBJECTS:crypto_verify_objects>)

if (WIN32)
    # Correct the import library.
    set_target_properties(crypto_verify_dynamic PROPERTIES IMPORT_PREFIX "")
    set_target_properties(crypto_verify_dynamic PROPERTIES IMPORT_SUFFIX ".lib")
endif (WIN32)

###################
# fastrandombytes #
###################

set(fastrandombytes_sources
    fastrandombytes/fastrandombytes.c
)

add_library(fastrandombytes_objects OBJECT ${fastrandombytes_sources})

set_property(TARGET fastrandombytes_objects PROPERTY POSITION_INDEPENDENT_CODE TRUE)
target_compile_definitions(fastrandombytes_objects PRIVATE BUILD)
target_include_directories(fastrandombytes_objects PUBLIC  fastrandombytes)

add_library(fastrandombytes_static  STATIC $<TARGET_OBJECTS:fastrandombytes_objects>)
add_library(fastrandombytes_dynamic SHARED $<TARGET_OBJECTS:fastrandombytes_objects>)

if (WIN32)
    # Correct the import library.
    set_target_properties(fastrandombytes_dynamic PROPERTIES IMPORT_PREFIX "")
    set_target_properties(fastrandombytes_dynamic PROPERTIES IMPORT_SUFFIX ".lib")
endif (WIN32)

#####################
# kernelrandombytes #
#####################

set(RANDOM_SOURCE_ADDED FALSE)

if (NOT RANDOM_SOURCE_ADDED)
    if (RANDOM_SOURCE_getentropy)
        set(kernelrandombytes_sources kernelrandombytes/getentropy.c)
        set(RANDOM_SOURCE_ADDED TRUE)
    endif ()
endif ()

if (NOT RANDOM_SOURCE_ADDED)
    if (RANDOM_SOURCE_getentropy2)
        set(kernelrandombytes_sources kernelrandombytes/getentropy2.c)
        set(RANDOM_SOURCE_ADDED TRUE)
    endif ()
endif ()

if (NOT RANDOM_SOURCE_ADDED)
    if (RANDOM_SOURCE_getrandom)
        set(kernelrandombytes_sources kernelrandombytes/getrandom.c)
        set(RANDOM_SOURCE_ADDED TRUE)
    endif ()
endif ()

if (NOT RANDOM_SOURCE_ADDED)
    if (RANDOM_SOURCE_getrandom2)
        set(kernelrandombytes_sources kernelrandombytes/getrandom2.c)
        set(RANDOM_SOURCE_ADDED TRUE)
    endif ()
endif ()

if (NOT RANDOM_SOURCE_ADDED)
    if (RANDOM_SOURCE_getrandom3)
        set(kernelrandombytes_sources kernelrandombytes/getrandom3.c)
        set(RANDOM_SOURCE_ADDED TRUE)
    endif ()
endif ()

if (NOT RANDOM_SOURCE_ADDED)
    if (RANDOM_SOURCE_rtlgenrandom)
        set(kernelrandombytes_sources kernelrandombytes/rtlgenrandom.c)
        set(RANDOM_SOURCE_ADDED TRUE)
    endif ()
endif ()

if (NOT RANDOM_SOURCE_ADDED)
    if (RANDOM_SOURCE_urandom)
        set(kernelrandombytes_sources kernelrandombytes/urandom.c)
        set(RANDOM_SOURCE_ADDED TRUE)
    endif ()
endif ()

if (NOT RANDOM_SOURCE_ADDED)
    message(FATAL_ERROR "No secure random source found. CMake will exit.")
endif ()

add_library(kernelrandombytes_objects OBJECT ${kernelrandombytes_sources})

set_property(TARGET kernelrandombytes_objects PROPERTY POSITION_INDEPENDENT_CODE TRUE)
target_compile_definitions(kernelrandombytes_objects PRIVATE BUILD)
target_include_directories(kernelrandombytes_objects PUBLIC  kernelrandombytes)

add_library(kernelrandombytes_static  STATIC $<TARGET_OBJECTS:kernelrandombytes_objects>)
add_library(kernelrandombytes_dynamic SHARED $<TARGET_OBJECTS:kernelrandombytes_objects>)

if (WIN32)
    # Correct the import library.
    set_target_properties(kernelrandombytes_dynamic PROPERTIES IMPORT_PREFIX "")
    set_target_properties(kernelrandombytes_dynamic PROPERTIES IMPORT_SUFFIX ".lib")
endif (WIN32)

if (RANDOM_SOURCE_rtlgenrandom)
    target_link_libraries(kernelrandombytes_static  ADVAPI32)
    target_link_libraries(kernelrandombytes_dynamic ADVAPI32)
endif ()

#####################################
# kernelrandombytes test executable #
#####################################

add_library(kernelrandombytes_test_objects OBJECT kernelrandombytes/test.c)

target_compile_definitions(kernelrandombytes_test_objects PRIVATE BUILD)
target_include_directories(kernelrandombytes_test_objects PRIVATE kernelrandombytes)

add_executable(kernelrandombytes_test_static  $<TARGET_OBJECTS:kernelrandombytes_test_objects>)
add_executable(kernelrandombytes_test_dynamic $<TARGET_OBJECTS:kernelrandombytes_test_objects>)

target_link_libraries(kernelrandombytes_test_static  kernelrandombytes_static)
target_link_libraries(kernelrandombytes_test_dynamic kernelrandombytes_dynamic)

################
# DEPENDENCIES #
################

target_link_libraries(crypto_sign_static
    crypto_hash_static
    crypto_hashblocks_static
    crypto_verify_static
    fastrandombytes_static
    crypto_rng_static
    crypto_stream_static
    crypto_core_static
    kernelrandombytes_static
)

target_link_libraries(crypto_sign_dynamic
    crypto_hash_dynamic
    crypto_hashblocks_dynamic
    crypto_verify_dynamic
    fastrandombytes_dynamic
    crypto_rng_dynamic
    crypto_stream_dynamic
    crypto_core_dynamic
    kernelrandombytes_dynamic
)

target_include_directories(crypto_sign_objects PUBLIC
    crypto_hash
    crypto_hashblocks
    crypto_verify
    fastrandombytes
    crypto_rng
    crypto_stream
    crypto_core
    kernelrandombytes
)

target_link_libraries(crypto_hash_static  crypto_hashblocks_static)
target_link_libraries(crypto_hash_dynamic crypto_hashblocks_dynamic)
target_include_directories(crypto_hash_objects PUBLIC crypto_hashblocks)

target_link_libraries(fastrandombytes_static  crypto_rng_static  crypto_stream_static  crypto_core_static  kernelrandombytes_static)
target_link_libraries(fastrandombytes_dynamic crypto_rng_dynamic crypto_stream_dynamic crypto_core_dynamic kernelrandombytes_dynamic)
target_include_directories(fastrandombytes_objects PUBLIC crypto_rng crypto_stream crypto_core kernelrandombytes)

target_link_libraries(crypto_rng_static  crypto_stream_static  crypto_core_static)
target_link_libraries(crypto_rng_dynamic crypto_stream_dynamic crypto_core_dynamic)
target_include_directories(crypto_rng_objects PUBLIC crypto_stream crypto_core)

target_link_libraries(crypto_stream_static  crypto_core_static)
target_link_libraries(crypto_stream_dynamic crypto_core_dynamic)
target_include_directories(crypto_stream_objects PUBLIC crypto_core)

#########
# TESTS #
#########

if (BUILD_TESTING)
    add_test(NAME kernelrandombytes_test_static  COMMAND kernelrandombytes_test_static)
    add_test(NAME kernelrandombytes_test_dynamic COMMAND kernelrandombytes_test_dynamic)
endif ()
