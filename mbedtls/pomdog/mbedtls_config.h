/*
 *  Copyright The Mbed TLS Contributors
 *  SPDX-License-Identifier: Apache-2.0 OR GPL-2.0-or-later
 */

// Mbed TLS configuration of Pomdog, which replaces include/mbedtls/mbedtls_config.h
// through MBEDTLS_CONFIG_FILE. It lists every option that Pomdog enables, so an
// option that upstream adds stays disabled until someone adds it here.
//
// Pomdog uses Mbed TLS as a TLS 1.2 and TLS 1.3 client that verifies servers
// against root certificates in PEM. It needs no TLS server, DTLS, PSK, session
// resumption, debug output, or X.509 writing.

#define MBEDTLS_CONFIG_VERSION 0x04000000

// NOTE: TLS streams read through mbedtls_net_recv().
#define MBEDTLS_NET_C

// NOTE: Error messages use mbedtls_strerror().
#define MBEDTLS_ERROR_C

// NOTE: TLS client.
#define MBEDTLS_SSL_TLS_C
#define MBEDTLS_SSL_CLI_C
#define MBEDTLS_SSL_PROTO_TLS1_2
#define MBEDTLS_SSL_PROTO_TLS1_3
#define MBEDTLS_SSL_SERVER_NAME_INDICATION
#define MBEDTLS_SSL_ALL_ALERT_MESSAGES

// NOTE: MBEDTLS_SSL_PROTO_TLS1_3 requires it.
#define MBEDTLS_SSL_KEEP_PEER_CERTIFICATE

// NOTE: TLS 1.2 key exchange with ECDHE only. The AEAD cipher suites follow
// from the algorithms that crypto_config.h enables.
#define MBEDTLS_KEY_EXCHANGE_ECDHE_ECDSA_ENABLED
#define MBEDTLS_KEY_EXCHANGE_ECDHE_RSA_ENABLED
#define MBEDTLS_SSL_EXTENDED_MASTER_SECRET

// NOTE: TLS 1.3 key exchange with (EC)DHE only, without pre-shared keys.
#define MBEDTLS_SSL_TLS1_3_KEY_EXCHANGE_MODE_EPHEMERAL_ENABLED
#define MBEDTLS_SSL_TLS1_3_COMPATIBILITY_MODE

// NOTE: Parsing and verifying certificates.
#define MBEDTLS_X509_USE_C
#define MBEDTLS_X509_CRT_PARSE_C
#define MBEDTLS_X509_RSASSA_PSS_SUPPORT
