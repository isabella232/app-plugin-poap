#include "poap_plugin.h"

// Set UI for "Mint" screen.
static void set_mint_ui(ethQueryContractUI_t *msg, context_t *context) {
    strlcpy(msg->title, "Mint", msg->titleLength);
    strlcpy(msg->msg, "POAP", msg->msgLength);
}

// Set UI for "Token" screen.
static void set_token_ui(ethQueryContractUI_t *msg, context_t *context) {
    strlcpy(msg->title, "Token", msg->titleLength);

    amountToString(context->token_received,
                   sizeof(context->token_received),
                   context->decimals,
                   context->ticker,
                   msg->msg,
                   msg->msgLength);
}

// Set UI for "Beneficiary" screen.
static void set_beneficiary_ui(ethQueryContractUI_t *msg, context_t *context) {
    strlcpy(msg->title, "Beneficiary", msg->titleLength);

    msg->msg[0] = '0';
    msg->msg[1] = 'x';

    getEthAddressStringFromBinary(context->beneficiary, msg->msg + 2, msg->pluginSharedRW->sha3, 0);
}

// Set UI for "Warning" screen.
static void set_warning_ui(ethQueryContractUI_t *msg,
                           const context_t *context __attribute__((unused))) {
    strlcpy(msg->title, "WARNING", msg->titleLength);
    strlcpy(msg->msg, "Unknown token", msg->msgLength);
}

// Helper function that returns the enum corresponding to the screen that should be displayed.
static screens_t get_screen(const ethQueryContractUI_t *msg, const context_t *context) {
    uint8_t index = msg->screenIndex;

    bool token_received_found = context->tokens_found & TOKEN_RECEIVED_FOUND;
    bool token_received_not_found = !token_received_found;
    switch (index) {
        case 0:
            if (token_received_found) {
                return TOKEN_SCREEN;
            } else if (token_received_not_found) {
                return WARN_SCREEN;
            }
            break;
        case 1:
            if (token_received_found) {
                return BENEFICIARY_SCREEN;
            } else if (token_received_not_found) {
                return WARN_SCREEN;
            }
            break;
        default:
            return ERROR;
            break;
    }
}

void handle_query_contract_ui(void *parameters) {
    ethQueryContractUI_t *msg = (ethQueryContractUI_t *) parameters;
    context_t *context = (context_t *) msg->pluginContext;

    memset(msg->title, 0, msg->titleLength);
    memset(msg->msg, 0, msg->msgLength);
    msg->result = ETH_PLUGIN_RESULT_OK;

    screens_t screen = get_screen(msg, context);

    switch (screen) {
        case MINT_SCREEN:
            set_mint_ui(msg, context);
            break;
        case TOKEN_SCREEN:
            set_token_ui(msg, context);
            break;
        case BENEFICIARY_SCREEN:
            set_beneficiary_ui(msg, context);
            break;
        case WARN_SCREEN:
            set_warning_ui(msg, context);
            break;
        default:
            PRINTF("Received an invalid screenIndex\n");
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            return;
    }
}
