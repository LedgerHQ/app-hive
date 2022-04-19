#pragma once

#include <stdint.h>   // uint*_t
#include <stdbool.h>  // bool

#include "common/buffer.h"

/**
 * Handler for SIGN_HASH command. If successfully parse BIP32 path
 * and transaction hash, sign the hash and send APDU response.
 *
 * @see G_context.bip32_path, G_context.tx_info.raw_transaction,
 * G_context.tx_info.signature. // TODO modify
 *
 * @param[in,out] cdata
 *   Command data with BIP32 path and raw transaction serialized.
 *
 * @return zero or positive integer if success, negative integer otherwise.
 *
 */
int handler_sign_hash(buffer_t *cdata);
