/*
 *  Copyright The Mbed TLS Contributors
 *  SPDX-License-Identifier: Apache-2.0 OR GPL-2.0-or-later
 */

// TF-PSA-Crypto configuration of Pomdog, which replaces
// tf-psa-crypto/include/psa/crypto_config.h through TF_PSA_CRYPTO_CONFIG_FILE.
// It lists every option that Pomdog enables, so an option that upstream adds
// stays disabled until someone adds it here.
//
// It enables what the TLS client of mbedtls_config.h needs, and what parsing
// the roots of Mozilla's root store needs.

#ifndef PSA_CRYPTO_CONFIG_H
#define PSA_CRYPTO_CONFIG_H

#define TF_PSA_CRYPTO_CONFIG_VERSION 0x01000000

// NOTE: AEAD ciphers of TLS 1.3 and of the TLS 1.2 cipher suites.
#define PSA_WANT_ALG_GCM 1
#define PSA_WANT_ALG_CHACHA20_POLY1305 1
#define PSA_WANT_KEY_TYPE_AES 1
#define PSA_WANT_KEY_TYPE_CHACHA20 1

// NOTE: CTR_DRBG, the random generator of PSA, needs AES in ECB mode.
#define PSA_WANT_ALG_ECB_NO_PADDING 1

// NOTE: Key schedules of TLS 1.3 (HKDF) and TLS 1.2 (PRF).
#define PSA_WANT_ALG_HKDF 1
#define PSA_WANT_ALG_HKDF_EXTRACT 1
#define PSA_WANT_ALG_HKDF_EXPAND 1
#define PSA_WANT_ALG_HMAC 1
#define PSA_WANT_ALG_TLS12_PRF 1
#define PSA_WANT_KEY_TYPE_DERIVE 1
#define PSA_WANT_KEY_TYPE_HMAC 1
#define PSA_WANT_KEY_TYPE_RAW_DATA 1

// NOTE: SHA-1 lets the store parse roots that sign themselves with SHA-1, as a
// few in Mozilla's root store do. Verification still rejects SHA-1 signatures,
// since the default X.509 profile accepts SHA-256 and above only.
#define PSA_WANT_ALG_SHA_1 1
#define PSA_WANT_ALG_SHA_256 1
#define PSA_WANT_ALG_SHA_384 1
#define PSA_WANT_ALG_SHA_512 1

// NOTE: ECDHE key exchange with X25519 and the NIST curves.
#define PSA_WANT_ALG_ECDH 1
#define PSA_WANT_ECC_MONTGOMERY_255 1
#define PSA_WANT_ECC_SECP_R1_256 1
#define PSA_WANT_ECC_SECP_R1_384 1
#define PSA_WANT_ECC_SECP_R1_521 1
#define PSA_WANT_KEY_TYPE_ECC_KEY_PAIR_BASIC 1
#define PSA_WANT_KEY_TYPE_ECC_KEY_PAIR_IMPORT 1
#define PSA_WANT_KEY_TYPE_ECC_KEY_PAIR_EXPORT 1
#define PSA_WANT_KEY_TYPE_ECC_KEY_PAIR_GENERATE 1

// NOTE: Verifying ECDSA and RSA signatures of certificates and handshakes.
#define PSA_WANT_ALG_ECDSA 1
#define PSA_WANT_ALG_RSA_PKCS1V15_SIGN 1
#define PSA_WANT_ALG_RSA_PSS 1
#define PSA_WANT_KEY_TYPE_ECC_PUBLIC_KEY 1
#define PSA_WANT_KEY_TYPE_RSA_PUBLIC_KEY 1

// NOTE: MBEDTLS_KEY_EXCHANGE_ECDHE_RSA_ENABLED requires it, although a client
// only verifies with the public key of the server.
#define PSA_WANT_KEY_TYPE_RSA_KEY_PAIR_BASIC 1

// NOTE: Certificates check their validity period against the time of day.
#define MBEDTLS_HAVE_TIME
#define MBEDTLS_HAVE_TIME_DATE

#define MBEDTLS_PLATFORM_C

// NOTE: PSA crypto with the random generator of the OS, without key storage.
#define MBEDTLS_PSA_CRYPTO_C
#define MBEDTLS_PSA_BUILTIN_GET_ENTROPY
#define MBEDTLS_PSA_KEY_STORE_DYNAMIC
#define MBEDTLS_CTR_DRBG_C

// NOTE: Parsing keys and certificates in DER and PEM.
#define MBEDTLS_MD_C
#define MBEDTLS_PK_C
#define MBEDTLS_PK_PARSE_C
#define MBEDTLS_ASN1_PARSE_C
#define MBEDTLS_BASE64_C
#define MBEDTLS_PEM_PARSE_C

// NOTE: Faster AES and elliptic curves on the CPU.
#define MBEDTLS_HAVE_ASM
#define MBEDTLS_AESNI_C
#define MBEDTLS_AESCE_C
#define MBEDTLS_ECP_NIST_OPTIM

#endif // PSA_CRYPTO_CONFIG_H
