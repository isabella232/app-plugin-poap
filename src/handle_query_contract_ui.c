#include "poap_plugin.h"

// Set UI for the "Send" screen.
static void set_send_ui(ethQueryContractUI_t *msg, poap_parameters_t *context) {
    switch (context->selectorIndex) {
        case MINT_TOKEN:
            strlcpy(msg->title, "Mint", msg->titleLength);
            break;
        default:
            PRINTF("Unhandled selector Index: %d\n", context->selectorIndex);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            return;
    }
}
// Set UI for "Receive" screen.
static void set_receive_ui(ethQueryContractUI_t *msg, poap_parameters_t *context) {
    switch (context->selectorIndex) {
        case MINT_TOKEN:
            strlcpy(msg->title, "Token", msg->titleLength);
            break;
        default:
            PRINTF("Unhandled selector Index: %d\n", context->selectorIndex);
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            return;
    }

    amountToString(context->token_received,
                   sizeof(context->token_received),
                   context->decimals,
                   context->ticker_received,
                   msg->msg,
                   msg->msgLength);
}

// Set UI for "Beneficiary" screen.
static void set_beneficiary_ui(ethQueryContractUI_t *msg, poap_parameters_t *context) {
    strlcpy(msg->title, "Beneficiary", msg->titleLength);

    msg->msg[0] = '0';
    msg->msg[1] = 'x';

    getEthAddressStringFromBinary((uint8_t *) context->beneficiary,
                                  (uint8_t *) msg->msg + 2,
                                  msg->pluginSharedRW->sha3,
                                  0);
}

// Set UI for "Warning" screen.
static void set_warning_ui(ethQueryContractUI_t *msg,
                           const poap_parameters_t *context __attribute__((unused))) {
    strlcpy(msg->title, "WARNING", msg->titleLength);
    strlcpy(msg->msg, "Unknown token", msg->msgLength);
}

// Helper function that returns the enum corresponding to the screen that should be displayed.
static screens_t get_screen(const ethQueryContractUI_t *msg, const poap_parameters_t *context) {
    uint8_t index = msg->screenIndex;

    bool token_received_found = context->tokens_found & TOKEN_RECEIVED_FOUND;

    switch (index) {
        case 0:
            return MINT_SCREEN;
            break;
        case 1:
            return BENEFICIARY_SCREEN;
            break;
        case 2:
            return TOKEN_SCREEN;
            break;
        case 3:
            return WARN_SCREEN;
            break;
        default:
            return ERROR;
            break;
    }
}

void handle_query_contract_ui(void *parameters) {
    ethQueryContractUI_t *msg = (ethQueryContractUI_t *) parameters;
    poap_parameters_t *context = (poap_parameters_t *) msg->pluginContext;

    memset(msg->title, 0, msg->titleLength);
    memset(msg->msg, 0, msg->msgLength);
    msg->result = ETH_PLUGIN_RESULT_OK;

    screens_t screen = get_screen(msg, context);
    switch (screen) {
        case MINT_SCREEN:
            set_send_ui(msg, context);
            break;
        case TOKEN_SCREEN:
            set_receive_ui(msg, context);
            break;
        case WARN_SCREEN:
            set_beneficiary_ui(msg, context);
            break;
        case BENEFICIARY_SCREEN:
            set_beneficiary_ui(msg, context);
            break;
        default:
            PRINTF("Received an invalid screenIndex\n");
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            return;
    }
}
