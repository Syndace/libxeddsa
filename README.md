[![Documentation Status](https://readthedocs.org/projects/libxeddsa/badge/?version=latest)](https://libxeddsa.readthedocs.io/)
[![Build Status](https://github.com/Syndace/libxeddsa/actions/workflows/build-and-test.yml/badge.svg)](https://github.com/Syndace/libxeddsa/actions/workflows/build-and-test.yml)

**_This repository is actively maintained._**

Activity is low at times because this project is already feature-complete, documented and tested.

# libxeddsa #

A toolkit around Curve25519 and Ed25519 key pairs, with a focus on conversion between the two.

Offers:
- Conversion between Curve25519 and Ed25519 public keys
- Extraction of private keys from seeds
- Ed25519 digital signing using seeds or private keys
- Ed25519 digital signature verification
- X25519 Diffie-Hellman key agreement

Some of those features are offered by re-exporting functions from [libsodium](https://download.libsodium.org/doc/), others are implemetations of the [XEdDSA](https://www.signal.org/docs/specifications/xeddsa/) specification, using ref10 by Daniel J. Bernstein.

The conversion functions utilize the birational maps between Curve25519 and Ed25519 defined in [RFC 7748, section 4.1 "Curve25519"](https://www.rfc-editor.org/rfc/rfc7748.html#section-4.1) (on page 5).

Note: XEdDSA is also specified for Curve448/Ed448, which is not covered by this library.

## Which Problems does this Library Solve? ##

This library solves two problems regarding the conversion between Curve25519 and Ed25519 key pairs.

### The Sign Bit ###

The birational maps define unique mappings between both elliptic curves. However, there is a small problem in practice: the serialization format for Curve25519 public keys (specified in [RFC 7748, section 5 "The X25519 and X448 Functions"](https://www.rfc-editor.org/rfc/rfc7748.html#section-5)) omits _one bit_ of information, which is not required for Diffie-Hellman on Curve25519, but is required for unique conversion into Ed25519 public keys. This one bit of missing information is what becomes the sign of the x coordinate when converted to Ed25519, thus it's called the _sign bit_ here.

There are two rather trivial ways to solve this issue:
- Transfer the sign bit somehow (aka "invent your own Curve25519 public key serialization format")
- Introduce some convention that makes the sign bit unique

XEdDSA goes down the second route and defines that only those private keys shall be used whose corresponding public keys have a sign bit of zero. This solves the ambiguity, because during conversion, the sign bit can simply be fixed to zero.

Both solutions are supported by this library.

### Seeds vs. Private Keys ###

The private keys of both Curve25519 and Ed25519 are technically scalars, which are multiplied by the base point of each curve to derive the respective public keys. The same private key works for both Curve25519 and Ed25519, thanks to the birational maps that are defined between the public keys. However, the Ed25519 specification doesn't give the private key to the user directly, instead they hand out a "seed", which the private key is derived from using a single application of SHA-512. Operations that require the private key, notably digital signing, always start by deriving the private key from the seed in the first step. This also means that digital signatures can't be generated using the APIs of libraries like libsodium, if you only have access the private key itself instead of a seed to derive it from.

This library solves this issue by offering a digital signing function that accepts the private key itself and skips the initial derivation step.

## Installation ##

Prebuilt binaries are available for Linux (amd64 & arm64), MacOS (amd64 & arm64) and Windows (amd64) and attached to [the releases](https://github.com/Syndace/libxeddsa/releases). They can also be found in the build artifacts of the [Build & Test workflow](https://github.com/Syndace/libxeddsa/actions/workflows/build-and-test.yml). To use libxeddsa, [libsodium](https://download.libsodium.org/doc/) has to be installed on the system.

Instructions for building from source, including compilation to WebAssembly using Emscripten, can be found in [the documentation](https://libxeddsa.readthedocs.io/).

## Bindings to other Programming Languages ##

| Language | Bindings | Notes |
|----------|----------|-------|
| Python   | [python-xeddsa](https://github.com/Syndace/python-xeddsa) | Provides simple statically typed bindings. |

## Documentation ##

View the documentation on [readthedocs.io](https://libxeddsa.readthedocs.io/) or build it locally, which requires [Doxygen](https://www.doxygen.nl/index.html) and the Python packages listed in `docs/requirements.txt`. With all dependencies installed, run `make html` in the `docs/` directory. You can find the generated documentation in `docs/_build/html/`.
