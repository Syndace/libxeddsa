libxeddsa
=========

A toolkit around Curve25519 and Ed25519 key pairs, with a focus on conversion between the two.

Offers:

* Conversion between Curve25519 and Ed25519 public keys
* Extraction of private keys from seeds
* Ed25519 digital signing using seeds or private keys
* Ed25519 digital signature verification
* X25519 Diffie-Hellman key agreement

Some of those features are offered by re-exporting functions from `libsodium <https://download.libsodium.org/doc/>`_, others are implemetations of the `XEdDSA <https://www.signal.org/docs/specifications/xeddsa/>`_ specification, using ref10 by Daniel J. Bernstein.

The conversion functions utilize the birational maps between Curve25519 and Ed25519 defined in `RFC 7748, section 4.1 "Curve25519" <https://www.rfc-editor.org/rfc/rfc7748.html#section-4.1>`_.

Note: XEdDSA is also specified for Curve448/Ed448, which is not covered by this library.

Which Problems does this Library Solve?
---------------------------------------

This library solves two problems regarding the conversion between Curve25519 and Ed25519 key pairs.

The Sign Bit
^^^^^^^^^^^^

The birational maps define unique mappings between both elliptic curves. However, there is a small problem in practice: the serialization format for Curve25519 public keys (specified in `RFC 7748, section 5 "The X25519 and X448 Functions" <https://www.rfc-editor.org/rfc/rfc7748.html#section-5>`_) omits *one bit* of information, which is not required for Diffie-Hellman on Curve25519, but is required for unique conversion into Ed25519 public keys. This one bit of missing information is what becomes the sign of the x coordinate when converted to Ed25519, thus it's called the *sign bit* here.

There are two rather trivial ways to solve this issue:

* Transfer the sign bit somehow (aka "invent your own Curve25519 public key serialization format")
* Introduce some convention that makes the sign bit unique

XEdDSA goes down the second route and defines that only those private keys shall be used whose corresponding public keys have a sign bit of zero. This solves the ambiguity, because during conversion, the sign bit can simply be fixed to zero.

Both solutions are supported by this library.

Seeds vs. Private Keys
^^^^^^^^^^^^^^^^^^^^^^

The private keys of both Curve25519 and Ed25519 are technically scalars, which are multiplied by the base point of each curve to derive the respective public keys. The same private key works for both Curve25519 and Ed25519, thanks to the birational maps that are defined between the public keys. However, the Ed25519 specification doesn't give the private key to the user directly, instead they hand out a "seed", which the private key is derived from using a single application of SHA-512. Operations that require the private key, notably digital signing, always start by deriving the private key from the seed in the first step. This also means that digital signatures can't be generated using the APIs of libraries like libsodium, if you only have access the private key itself instead of a seed to derive it from.

This library solves this issue by offering a digital signing function that accepts the private key itself and skips the initial derivation step.

.. toctree::
   :maxdepth: 2
   :caption: Contents:

   Installation <installation>
   API Documentation <xeddsa>
