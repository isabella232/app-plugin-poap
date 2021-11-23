#include "poap_plugin.h"

void handle_finalize(void *parameters) {
    ethPluginFinalize_t *msg = (ethPluginFinalize_t *) parameters;
    poap_parameters_t *context = (poap_parameters_t *) msg->pluginContext;
    if (context->valid) {
        msg->numScreens = 3;

        msg->tokenLookup1 = context->token_received;
        PRINTF("Mint token to: %.*H\n", ADDRESS_LENGTH, context->beneficiary);
        msg->uiType = ETH_UI_TYPE_GENERIC;
        msg->result = ETH_PLUGIN_RESULT_OK;
    } else {
        PRINTF("Context not valid\n");
        msg->result = ETH_PLUGIN_RESULT_FALLBACK;
    }
}