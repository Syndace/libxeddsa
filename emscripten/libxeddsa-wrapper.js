ed25519_priv_sign = Module.cwrap("ed25519_priv_sign", null, [ "number", "array", "array", "number", "array" ])
ed25519_seed_sign = Module.cwrap("ed25519_seed_sign", null, [ "number", "array", "array", "number" ])
ed25519_verify = Module.cwrap("ed25519_verify", "number", [ "array", "array", "array", "number" ])

curve25519_pub_to_ed25519_pub = Module.cwrap("curve25519_pub_to_ed25519_pub", null, [ "number", "array", "number" ])
ed25519_pub_to_curve25519_pub = Module.cwrap("ed25519_pub_to_curve25519_pub", "number", [ "number", "array" ])

priv_to_curve25519_pub = Module.cwrap("priv_to_curve25519_pub", null, [ "number", "array" ])
priv_to_ed25519_pub = Module.cwrap("priv_to_ed25519_pub", null, [ "number", "array" ])
seed_to_ed25519_pub = Module.cwrap("seed_to_ed25519_pub", null, [ "number", "array" ])

priv_force_sign = Module.cwrap("priv_force_sign", null, [ "number", "array", "number" ])
seed_to_priv = Module.cwrap("seed_to_priv", null, [ "number", "array" ])

x25519 = Module.cwrap("x25519", "number", [ "number", "array", "array" ])

xeddsa_init = Module.cwrap("xeddsa_init", "number", [])

XEDDSA_VERSION_MAJOR = Module.HEAPU32.subarray(Module._XEDDSA_VERSION_MAJOR/4, Module._XEDDSA_VERSION_MAJOR/4 + 1)[0]
XEDDSA_VERSION_MINOR = Module.HEAPU32.subarray(Module._XEDDSA_VERSION_MINOR/4, Module._XEDDSA_VERSION_MINOR/4 + 1)[0]
XEDDSA_VERSION_REVISION = Module.HEAPU32.subarray(Module._XEDDSA_VERSION_REVISION/4, Module._XEDDSA_VERSION_REVISION/4 + 1)[0]
