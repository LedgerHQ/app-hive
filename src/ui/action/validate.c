/*****************************************************************************
 *   Ledger App Hive.
 *   (c) 2020 Ledger SAS.
 *   Modifications (c) Bartłomiej (@engrave) Górnicki
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *****************************************************************************/

#include <stdbool.h>  // bool

#include "validate.h"
#include "ui/menu.h"
#include "sw.h"
#include "io.h"
#include "crypto.h"
#include "globals.h"
#include "helper/send_response.h"

void ui_action_validate_pubkey(bool choice) {
    if (choice) {
        helper_send_response_pubkey();
    } else {
        io_send_sw(SW_DENY);
    }

    ui_menu_main();
}

void ui_action_validate_transaction(bool choice) {
    if (choice) {
        G_context.state = STATE_APPROVED;

        ui_display_signing_message();

        // refresh the display before intensive operation
        io_seproxyhal_io_heartbeat();

        if (!crypto_sign_message()) {
            io_send_sw(SW_SIGNATURE_FAIL);
        } else {
            helper_send_response_sig();
        }
    } else {
        io_send_sw(SW_DENY);
    }

    G_context.state = STATE_NONE;
    ui_menu_main();
}
