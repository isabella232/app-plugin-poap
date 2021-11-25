#include "poap_plugin.h"

void handle_provide_token(void *parameters) {
    ethPluginProvideToken_t *msg = (ethPluginProvideToken_t *) parameters;
    poap_parameters_t *context = (poap_parameters_t *) msg->pluginContext;

    PRINTF("POAP plugin provide token: 0x%p\n", msg->token1);

    if (msg->token1 != NULL) {
        context->decimals = msg->token1->decimals;
        strlcpy(context->ticker, (char *) msg->token1->ticker, sizeof(context->ticker));
        context->tokens_found |= TOKEN_FOUND;
    } else {
        context->decimals = DEFAULT_DECIMAL;
        msg->additionalScreens++;
    }
    msg->result = ETH_PLUGIN_RESULT_OK;
}