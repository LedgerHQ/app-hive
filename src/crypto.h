
#pragma once

#include <stdint.h>  // uint*_t

#include "os.h"
#include "cx.h"
#include "constants.h"

/**
 * Derive private key given BIP32 path.
 *
 * @param[out] private_key
 *   Pointer to private key.
 * @param[out] chain_code
 *   Pointer to 32 bytes array for chain code.
 * @param[in]  bip32_path
 *   Pointer to buffer with BIP32 path.
 * @param[in]  bip32_path_len
 *   Number of path in BIP32 path.
 *
 * @return 0 if success, -1 otherwise.
 *
 * @throw INVALID_PARAMETER
 *
 */
int crypto_derive_private_key(cx_ecfp_private_key_t *private_key, uint8_t chain_code[static 32], const uint32_t *bip32_path, uint8_t bip32_path_len);

/**
 * Initialize public key given private key.
 *
 * @param[in]  private_key
 *   Pointer to private key.
 * @param[out] public_key
 *   Pointer to public key.
 * @param[out] raw_public_key
 *   Pointer to raw public key.
 *
 * @return 0 if success, -1 otherwise.
 *
 * @throw INVALID_PARAMETER
 *
 */
int crypto_init_public_key(cx_ecfp_private_key_t *private_key, cx_ecfp_public_key_t *public_key, uint8_t raw_public_key[static PUBKEY_UNCOMPRESSED_LEN]);

/**
 * Sign message hash in global context.
 *
 * @param[in]  digest
 *   Pointer to transaction digest.
 * @param[out] signature
 *  Pointer to signature.
 *
 * @see G_context.bip32_path
 *
 * @return true if success, false otherwise.
 *
 * @throw INVALID_PARAMETER
 *
 */
bool crypto_sign_digest(const uint8_t digest[static DIGEST_LEN], uint8_t signature[static SIGNATURE_LEN]);
