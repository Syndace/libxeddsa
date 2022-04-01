#include <stdbool.h>
#include <stdint.h>

#include <sc.h>
#include <sodium.h>

// ge_scalarmult_base, sc_reduce, sc_muladd all store private data on the stack without overwriting it before
// returning. The sodium_stackzero function solves that by allocating a configurable amount of space on the
// stack and overwriting it in the hope to catch all leftovers of stack variables allocated by previously
// called functions. ge_scalarmult_base allocates around 470 bytes of stack, sc_reduce around 330 and
// sc_muladd around 660 bytes which makes it the biggest stack consumer. Together with things like parameters
// and the return addresses lying on the stack, at least 700 bytes have to be cleared. To be sure (and to have
// a power of two!), STACKZERO_SIZE is set to 1024.
#define STACKZERO_SIZE 1024

// Byte-representation of the scalar value of 0 on the Ed25519 curve. Needed by `sc_neg`.
static const uint8_t ZERO[32] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

// Byte-representation of the scalar value of -1 on the Ed25519 curve. Needed by `sc_neg`.
static const uint8_t MINUS_ONE[32] = {
    0xec, 0xd3, 0xf5, 0x5c, 0x1a, 0x63, 0x12, 0x58,
    0xd6, 0x9c, 0xf7, 0xa2, 0xde, 0xf9, 0xde, 0x14,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10
};

/**
 * Clamp a scalar. This operation is idempotent.
 * 
 * @param scalar_out (out): The clamped output scalar.
 * @param scalar_in (in): The input scalar.
 */
static void sc_clamp(uint8_t* scalar_out, const uint8_t* scalar_in) {
    // Mandated by RFC 7748: https://tools.ietf.org/html/rfc7748#section-5
    // Referred to as "clamping": https://moderncrypto.org/mail-archive/curves/2017/000858.html
    for (uint8_t i = 0; i < 32; i++)
        scalar_out[i] = scalar_in[i];

    scalar_out[ 0] = scalar_in[ 0] & 248;
    scalar_out[31] = scalar_in[31] & 127 | 64;
}

/**
 * Constant-time implementation of a conditional move without conditional branching.
 * 
 * @param scalar_inout (inout): The first input scalar as well as the output.
 * @param scalar_in (in): The second input scalar.
 * @param move: Whether to move the second scalar into the first or not.
 * 
 * @warning The value of `move` is not cleared from the stack before returning. Thus, if the value of `move`
 *     contains private data, make sure to clean the stack after the call returns.
 */
static void sc_cmov(uint8_t* scalar_inout, const uint8_t* scalar_in, const bool move) {
    // The goal is a bitmask with either all ones or all zeros, based on the boolean value of move and in
    // constant time without conditional branches.
    // The code first negates `move` twice logically to get either a one or a zero.
    // Then it subtracts one from `move`, which either results in 0x00 (if `move` is true) or in 0xff due to
    // an 8 bit underflow (if move is false).
    uint8_t mask = ((uint8_t) !!move) - 1;

    // First, XOR both possible results
    for (uint8_t i = 0; i < 32; i++)
        scalar_inout[i] ^= scalar_in[i];

    // Second, and the condition mask to the mixture, which results in either of:
    // - out = (out ^ in) & 0x00 = 0x00,     if `move` is true
    // - out = (out ^ in) & 0xff = out ^ in, if `move` is false
    for (uint8_t i = 0; i < 32; i++)
        scalar_inout[i] &= mask;

    // Third, XOR out with in again, which results in either of:
    // - out ^ in = (0       ) ^ in = in,  if `move` is true
    // - out ^ in = (out ^ in) ^ in = out, if `move` is false
    for (uint8_t i = 0; i < 32; i++)
        scalar_inout[i] ^= scalar_in[i];
}

/**
 * Negate a scalar on the Ed25519 curve.
 * 
 * @param scalar_out (out): The negated scalar.
 * @param scalar_in (in): The scalar to negate.
 */
static void sc_neg(uint8_t* scalar_out, const uint8_t* scalar_in) {
    // ref10 only provides sc_muladd to calculate (ab+c) mod l.
    // This can be used to negate a scalar by choosing a = -1 mod l, b = scalar, c = 0.
    // The "difficult" part is calculating -1 mod l, where l = 2^252 + 27742317777372353535851937790883648493.
    // Python to the rescue!
    // Running (-1 % l).to_bytes(32, "little") yields the byte sequence stored in MINUS_ONE.
    sc_muladd(scalar_out, MINUS_ONE, scalar_in, ZERO);

    sodium_stackzero(STACKZERO_SIZE);
}
