#include <test.h>

#include <stdio.h>
#include <stdbool.h>

#include <xeddsa.h>

#include <sodium.h>

static void check(int result, bool* failed, const char* name) {
    if (result == 0) {
        fprintf(stdout, "Test passed: %s\n", name);
    }
    else {
        fprintf(stderr, "Test failed: %s\n", name);
        *failed = true;
    }
}

int main() {
    bool failed = false;

    if (xeddsa_init() < 0) {
        fprintf(stderr, "Couldn't initialize the library.\n");
        return EXIT_FAILURE;
    }

    check(test_libsodium_compatibility(), &failed, "libsodium compatibility");
    check(test_conversion_uniqueness(),   &failed, "public key conversion uniqueness");

    return failed ? EXIT_FAILURE : EXIT_SUCCESS;
}
