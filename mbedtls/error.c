/*
 *  Error message information
 *
 *  Copyright The Mbed TLS Contributors
 *  SPDX-License-Identifier: Apache-2.0 OR GPL-2.0-or-later
 */

#include "mbedtls/error.h"

#if defined(MBEDTLS_ERROR_C) || defined(MBEDTLS_ERROR_STRERROR_DUMMY)

#if defined(MBEDTLS_ERROR_C)

#if defined(MBEDTLS_PLATFORM_C)
#include "mbedtls/platform.h"
#else
#define mbedtls_snprintf snprintf
#endif

#include <stdio.h>
#include <string.h>

#if defined(MBEDTLS_NET_C)
#include "mbedtls/net_sockets.h"
#endif

#if defined(MBEDTLS_SSL_TLS_C)
#include "mbedtls/ssl.h"
#endif

#if defined(MBEDTLS_X509_USE_C) || defined(MBEDTLS_X509_CREATE_C)
#include "mbedtls/x509.h"
#endif

const char *mbedtls_high_level_strerr(int error_code)
{
    int high_level_error_code;

    if (error_code < 0) {
        error_code = -error_code;
    }

    high_level_error_code = error_code & 0xFF80;

    switch (high_level_error_code) {
#if defined(MBEDTLS_SSL_TLS_C)
        case 0x7000:
            return "SSL - A cryptographic operation is in progress";
        case 0x7080:
            return "SSL - The requested feature is not available";
        case 0x7180:
            return "SSL - No valid padding or unknown message length";
        case 0x7200:
            return "SSL - An invalid SSL record was received";
        case 0x7280:
            return "SSL - The connection indicated an EOF";
        case 0x7300:
            return "SSL - A message could not be parsed due to a decoding error";
        case 0x7400:
            return "SSL - No RNG was provided";
        case 0x7480:
            return "SSL - No client certification received from the client";
        case 0x7500:
            return "SSL - An unsupported extension was received";
        case 0x7580:
            return "SSL - No application protocol has been negotiated";
        case 0x7600:
            return "SSL - The own private key or pre-shared key is not set";
        case 0x7680:
            return "SSL - No CA chain is set, but required to operate";
        case 0x7700:
            return "SSL - An unexpected message was received from our peer";
        case 0x7780:
            return "SSL - A fatal alert message was received from our peer";
        case 0x7800:
            return "SSL - Verification of our peer failed";
        case 0x7880:
            return "SSL - The peer notified us that the connection is going to be closed";
        case 0x7A00:
            return "SSL - Processing of the Certificate handshake message failed";
        case 0x7B00:
            return "SSL - A new session ticket has been received";
        case 0x7B80:
            return "SSL - Early data cannot be read from this context";
        case 0x7C00:
            return "SSL - Early data has been received";
        case 0x7C80:
            return "SSL - Early data cannot be written to this context";
        case 0x7E80:
            return "SSL - Cache entry not found";
        case 0x7F80:
            return "SSL - A hardware accelerator encountered an error";
        case 0x6F80:
            return "SSL - The hardware accelerator does not support this key size";
        case 0x6E80:
            return "SSL - Handshake protocol not within min/max boundaries";
        case 0x6E00:
            return "SSL - The handshake negotiation failed";
        case 0x6D80:
            return "SSL - Session ticket has expired";
        case 0x6D00:
            return "SSL - Public key type mismatch";
        case 0x6C80:
            return "SSL - Unknown identity received (PSK)";
        case 0x6C00:
            return "SSL - Internal error";
        case 0x6B80:
            return "SSL - A counter would wrap";
        case 0x6B00:
            return "SSL - Unexpected message at ServerHello in renegotiation";
        case 0x6A80:
            return "SSL - DTLS client must retry for hello verification";
        case 0x6900:
            return "SSL - No data of requested type currently available";
        case 0x6880:
            return "SSL - Connection requires a write call";
        case 0x6800:
            return "SSL - The operation timed out";
        case 0x6780:
            return "SSL - The client initiated a reconnect from the same port";
        case 0x6700:
            return "SSL - Record header looks valid but is not expected";
        case 0x6680:
            return "SSL - The alert message received indicates a non-fatal error";
        case 0x6600:
            return "SSL - A field in a message was incorrect or inconsistent with other fields";
        case 0x6580:
            return "SSL - Internal-only message signaling that further message-processing should be done";
        case 0x6500:
            return "SSL - The asynchronous operation is not completed yet";
        case 0x6480:
            return "SSL - Internal-only message signaling that a message arrived early";
        case 0x6000:
            return "SSL - A record received with a length id that does not match the configured one";
        case 0x5F00:
            return "SSL - A record received with a version that does not match configured";
        case 0x5E80:
            return "SSL - A field in a configuration was incorrect or inconsistent";
        case 0x5D80:
            return "SSL - Certificate verification was attempted without a hostname configured";
#endif /* MBEDTLS_SSL_TLS_C */

#if defined(MBEDTLS_X509_USE_C) || defined(MBEDTLS_X509_CREATE_C)
        case 0x2080:
            return "X509 - Unavailable feature, e.g. RSA hashing/encryption combination";
        case 0x2100:
            return "X509 - Requested OID is unknown";
        case 0x2180:
            return "X509 - The CRT/CRL/CSR format is invalid";
        case 0x2200:
            return "X509 - The CRT/CRL/CSR version element is invalid";
        case 0x2280:
            return "X509 - The serial tag or value is invalid";
        case 0x2300:
            return "X509 - The algorithm tag or value is invalid";
        case 0x2380:
            return "X509 - The name tag or value is invalid";
        case 0x2400:
            return "X509 - The date tag or value is invalid";
        case 0x2480:
            return "X509 - The signature tag or value invalid";
        case 0x2500:
            return "X509 - The extension tag or value is invalid";
        case 0x2580:
            return "X509 - CRT/CRL/CSR has an unsupported version number";
        case 0x2600:
            return "X509 - Signature algorithm is unsupported (unknown, not safe, or incompatible)";
        case 0x2680:
            return "X509 - Certificate and private key types do not match";
        case 0x2700:
            return "X509 - Certificate verification failed";
        case 0x2780:
            return "X509 - Format not recognized as DER or PEM";
        case 0x2800:
            return "X509 - Input invalid";
        case 0x2900:
            return "X509 - Reading/writing of file failed";
        case 0x3000:
            return "X509 - A fatal error occurred";
#endif /* MBEDTLS_X509_USE_C || MBEDTLS_X509_CREATE_C */

        default:
            break;
    }

    return NULL;
}

const char *mbedtls_low_level_strerr(int error_code)
{
    int low_level_error_code;

    if (error_code < 0) {
        error_code = -error_code;
    }

    low_level_error_code = error_code & ~0xFF80;

    switch (low_level_error_code) {
#if defined(MBEDTLS_NET_C)
        case 0x0042:
            return "NET - Failed to open a socket";
        case 0x0044:
            return "NET - The connection to the given server / port failed";
        case 0x0046:
            return "NET - Binding of the socket failed";
        case 0x0048:
            return "NET - Could not listen on the socket";
        case 0x004A:
            return "NET - Could not accept the incoming connection";
        case 0x004C:
            return "NET - Reading information from the socket failed";
        case 0x004E:
            return "NET - Sending information through the socket failed";
        case 0x0050:
            return "NET - Connection was reset by peer";
        case 0x0052:
            return "NET - Failed to get an IP address for the given hostname";
        case 0x0045:
            return "NET - The context is invalid";
        case 0x0047:
            return "NET - Polling the net context failed";
        case 0x0049:
            return "NET - Input invalid";
#endif /* MBEDTLS_NET_C */

        default:
            break;
    }

    return NULL;
}

void mbedtls_strerror(int error_code, char *buffer, size_t buflen)
{
    size_t len;
    int use_ret;
    const char *high_level_error_description = NULL;
    const char *low_level_error_description = NULL;

    if (buflen == 0) {
        return;
    }

    memset(buffer, 0x00, buflen);

    if (error_code < 0) {
        error_code = -error_code;
    }

    if (error_code == 0) {
        return;
    }

    high_level_error_description = mbedtls_high_level_strerr(-error_code);
    low_level_error_description = mbedtls_low_level_strerr(-error_code);

    use_ret = error_code & 0xFF80;

    if (use_ret != 0) {
        if (high_level_error_description == NULL) {
            mbedtls_snprintf(buffer, buflen, "UNKNOWN ERROR CODE (%04X)", use_ret);
        } else {
            mbedtls_snprintf(buffer, buflen, "%s", high_level_error_description);
        }

        use_ret = error_code & ~0xFF80;
        if (use_ret == 0) {
            return;
        }

        len = strlen(buffer);
        if (len < buflen - 1) {
            mbedtls_snprintf(buffer + len, buflen - len, " : ");
        }

        buffer += len + 3;
        buflen -= len + 3;
    }

    use_ret = error_code & ~0xFF80;

    if (use_ret == 0) {
        return;
    }

    if (low_level_error_description == NULL) {
        mbedtls_snprintf(buffer, buflen, "UNKNOWN ERROR CODE (%04X)", use_ret);
    } else {
        mbedtls_snprintf(buffer, buflen, "%s", low_level_error_description);
    }
}

#else /* MBEDTLS_ERROR_C */

/*
 * Provide a dummy implementation when MBEDTLS_ERROR_C is not defined.
 */
void mbedtls_strerror(int error_code, char *buffer, size_t buflen)
{
    ((void) error_code);

    if (buflen > 0) {
        buffer[0] = '\0';
    }
}

#endif /* MBEDTLS_ERROR_C */

#endif /* MBEDTLS_ERROR_C || MBEDTLS_ERROR_STRERROR_DUMMY */
