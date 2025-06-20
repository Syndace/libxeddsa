# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/), and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

## [2.0.1] - 20th of June, 2025

### Changed
- Nothing about the functionality of libxeddsa was changed, however adjustments to the build system were made and libsodium has been updated.

## [2.0.0] - 2nd of April, 2022

### Added
- Added CHANGELOG.
- Added versioning.
- Added a set of functions surrounding Curve25519 and Ed25519 key pairs, to make this library a toolset around both curves instead of just an implementation of XEdDSA.
- Expanded documentation and tests to cover the new functionality.

### Changed
- Rewrote the XEdDSA APIs for greater flexibility.

## [1.0.0] - 29th of March, 2020

### Added
- An implementation of XEdDSA for Curve25519 keys, consisting of a function for signing using a Curve25519 private key and public key conversion from Curve25519 to Ed25519.

[Unreleased]: https://github.com/Syndace/libxeddsa/compare/v2.0.1...HEAD
[2.0.1]: https://github.com/Syndace/libxeddsa/compare/v2.0.0...v2.0.1
[2.0.0]: https://github.com/Syndace/libxeddsa/compare/v1.0.0...v2.0.0
[1.0.0]: https://github.com/Syndace/libxeddsa/releases/tag/v1.0.0
