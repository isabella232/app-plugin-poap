#include "poap_plugin.h"

// Called once to init.
void handle_init_contract(void *parameters) {
    ethPluginInitContract_t *msg = (ethPluginInitContract_t *) parameters;

    if (msg->interfaceVersion != ETH_PLUGIN_INTERFACE_VERSION_LATEST) {
        PRINTF("Wrong interface version: expected %d got %d\n",
               ETH_PLUGIN_INTERFACE_VERSION_LATEST,
               msg->interfaceVersion);
        msg->result = ETH_PLUGIN_RESULT_UNAVAILABLE;
        return;
    }

    if (msg->pluginContextLength < sizeof(poap_parameters_t)) {
        PRINTF("Poap context size too big: expected %d got %d\n",
               sizeof(poap_parameters_t),
               msg->pluginContextLength);
        msg->result = ETH_PLUGIN_RESULT_ERROR;
        return;
    }

    poap_parameters_t *context = (poap_parameters_t *) msg->pluginContext;
    memset(context, 0, sizeof(*context));
    context->valid = 1;

    for (uint8_t i = 0; i < NUM_POAP_SELECTORS; i++) {
        if (memcmp((uint8_t *) PIC(POAP_SELECTORS[i]), msg->selector, SELECTOR_SIZE) == 0) {
            context->selectorIndex = i;
            break;
        }
    }

    // Set `next_param` to be the first field we expect to parse.
    switch (context->selectorIndex) {
        case MINT_TOKEN:
            // context->skip = 1;
            context->next_param = EVENT_ID;
            break;
        default:
            PRINTF("Missing selectorIndex\n");
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            return;
    }
    msg->result = ETH_PLUGIN_RESULT_OK;
}
