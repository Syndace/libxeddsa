#include <xeddsa.h>

#include <fe.h>

/**
 * Convert a Curve25519 public key into an Ed25519 public key, which validates signatures created by using
 * `curve25519_sign` with the corresponding Curve25519 private key.
 *
 * @param ed25519_pub (out): The Ed25519 public key corresponding to the Curve25519 public key. The
 *     little-endian encoding of the y coordinate (32 bytes) with the sign bit of the x coordinate stored in
 *     the most significant bit as per RFC 8032 (on page 7).
 * @param curve25519_pub (in): The Curve25519 public key to convert into its Ed25519 equivalent. The
 *     little-endian encoding of the u coordinate as per RFC 7748 (on page 4).
 */
void curve25519_pub_to_ed25519_pub(uint8_t* ed25519_pub, const uint8_t* curve25519_pub) {
    // Apply the birational map defined in RFC 7748 to calculate an Ed25519 public key from a Curve25519
    // public key. Because the serialization format of Curve25519 public keys only contains the u coordinate,
    // the x coordinate of the corresponding Ed25519 public key can't be uniquely calculated as defined by the
    // birational map. The x coordinate is represented in the serialization format of Ed25519 public keys only
    // in a single sign bit. This function assumes that the sign bit has to be zero, the curve25519_sign
    // function makes sure that this assumption is correct for all signatures it generates.
    fe u, y;
    fe one;
    fe u_minus_one, u_plus_one, u_plus_one_inv;

    // Parse the Curve25519 public key input as a field element containing the u coordinate
    fe_frombytes(u, curve25519_pub);

    // Calculate the parameters (u - 1) and (u + 1)
    fe_1(one);
    fe_sub(u_minus_one, u, one);
    fe_add(u_plus_one,  u, one);

    // Invert u + 1
    fe_invert(u_plus_one_inv, u_plus_one);

    // Calculate y = (u - 1) * inv(u + 1) (mod p)
    fe_mul(y, u_minus_one, u_plus_one_inv);

    // Serialize the field element containing the y coordinate to the Ed25519 public key output
    fe_tobytes(ed25519_pub, y);

    // Set the sign bit to zero
    ed25519_pub[31] &= 0x7f;
}
