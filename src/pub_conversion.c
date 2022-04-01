#include <fe.h>
#include <sodium.h>

#include <xeddsa.h>

/**
 * Convert a Curve25519 public key into an Ed25519 public key.
 *
 * @param ed25519_pub (out): The Ed25519 public key corresponding to the Curve25519 public key.
 * @param curve25519_pub (in): The Curve25519 public key to convert into its Ed25519 equivalent.
 * @param set_sign_bit: Whether to set the sign bit of the output Ed25519 public key.
 */
void curve25519_pub_to_ed25519_pub(uint8_t* ed25519_pub,
                                   const uint8_t* curve25519_pub,
                                   const bool set_sign_bit) {
    // Apply the birational map defined in RFC 7748, section 4.1 "Curve25519" to calculate an Ed25519 public
    // key from a Curve25519 public key. Because the serialization format of Curve25519 public keys only
    // contains the u coordinate, the x coordinate of the corresponding Ed25519 public key can't be uniquely
    // calculated as defined by the birational map. The x coordinate is represented in the serialization
    // format of Ed25519 public keys only in a single sign bit. This function assumes that the sign bit is
    // known to the user and is passed accordingly.
    fe u, y;
    fe one;
    fe u_minus_one, u_plus_one, u_plus_one_inv;

    // Parse the Curve25519 public key input as a field element containing the u coordinate. RFC 7748,
    // section 5 "The X25519 and X448 Functions", mandates that the most significant bit of the Curve25519
    // public key has to be zeroized. This is handled by fe_frombytes internally.
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

    // Set the sign bit to the requested value
    ed25519_pub[31] |= !!set_sign_bit << 7;
}

/**
 * Convert an Ed25519 public key into a Curve25519 public key. Re-export of libsodiums/ref10s
 * `crypto_sign_ed25519_pk_to_curve25519` function for convenience.
 *
 * @param curve25519_pub (out): The Curve25519 public key corresponding to the Ed25519 public key.
 * @param ed25519_pub (in): The Ed25519 public key to convert into its Curve25519 equivalent.
 * @return 0 on success, -1 if the public key was rejected due to suboptimal security propierties.
 */
int ed25519_pub_to_curve25519_pub(uint8_t* curve25519_pub, const uint8_t* ed25519_pub) {
    return crypto_sign_ed25519_pk_to_curve25519(curve25519_pub, ed25519_pub);
}
