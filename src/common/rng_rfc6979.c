#include "os.h"
#include "libcxng.h"
#include <string.h>  // memmove, memset

/**
 * The nonce generated by internal library CX_RND_RFC6979 is not compatible
 * with Hive. So this is the way to generate nonce for Hive.
 */
void rng_rfc6979(unsigned char *rnd,
                 unsigned char *h1,
                 unsigned char *x,
                 unsigned int x_len,
                 const unsigned char *q,
                 unsigned int q_len,
                 unsigned char *V,
                 unsigned char *K) {
    unsigned int h_len, offset, found, i;
    cx_hmac_sha256_t hmac;

    h_len = 32;
    // a. h1 as input

    // loop for a candidate
    found = 0;
    while (!found) {
        if (x) {
            // b.  Set: V = 0x01 0x01 0x01 ... 0x01
            memset(V, 0x01, h_len);

            // c. Set: K = 0x00 0x00 0x00 ... 0x00
            memset(K, 0x00, h_len);

            // d.  Set: K = HMAC_K(V || 0x00 || int2octets(x) || bits2octets(h1))
            V[h_len] = 0;
            cx_hmac_sha256_init(&hmac, K, 32);
            cx_hmac((cx_hmac_t *) &hmac, 0, V, h_len + 1, K, 32);
            cx_hmac((cx_hmac_t *) &hmac, 0, x, x_len, K, 32);
            cx_hmac((cx_hmac_t *) &hmac, CX_LAST, h1, h_len, K, 32);

            // e.  Set: V = HMAC_K(V)
            cx_hmac_sha256_init(&hmac, K, 32);
            cx_hmac((cx_hmac_t *) &hmac, CX_LAST, V, h_len, V, 32);

            // f.  Set:  K = HMAC_K(V || 0x01 || int2octets(x) || bits2octets(h1))
            V[h_len] = 1;
            cx_hmac_sha256_init(&hmac, K, 32);
            cx_hmac((cx_hmac_t *) &hmac, 0, V, h_len + 1, K, 32);
            cx_hmac((cx_hmac_t *) &hmac, 0, x, x_len, K, 32);
            cx_hmac((cx_hmac_t *) &hmac, CX_LAST, h1, h_len, K, 32);

            // g. Set: V = HMAC_K(V) --
            cx_hmac_sha256_init(&hmac, K, 32);
            cx_hmac((cx_hmac_t *) &hmac, CX_LAST, V, h_len, V, 32);

            // initial setup only once
            x = NULL;
        } else {
            // h.3  K = HMAC_K(V || 0x00)
            V[h_len] = 0;
            cx_hmac_sha256_init(&hmac, K, 32);
            cx_hmac((cx_hmac_t *) &hmac, CX_LAST, V, h_len + 1, K, 32);

            // h.3 V = HMAC_K(V)
            cx_hmac_sha256_init(&hmac, K, 32);
            cx_hmac((cx_hmac_t *) &hmac, CX_LAST, V, h_len, V, 32);
        }

        // generate candidate
        /* Shortcut: As only secp256k1/sha256 is supported, the step h.2 :
         *   While tlen < qlen, do the following:
         *     V = HMAC_K(V)
         *     T = T || V
         * is replace by
         *     V = HMAC_K(V)
         */
        x_len = q_len;
        offset = 0;
        while (x_len) {
            if (x_len < h_len) {
                h_len = x_len;
            }
            cx_hmac_sha256_init(&hmac, K, 32);
            cx_hmac((cx_hmac_t *) &hmac, CX_LAST, V, h_len, V, 32);
            memmove(rnd + offset, V, h_len);
            x_len -= h_len;
        }

        // h.3 Check T is < n
        for (i = 0; i < q_len; i++) {
            if (V[i] < q[i]) {
                found = 1;
                break;
            }
        }
    }
}