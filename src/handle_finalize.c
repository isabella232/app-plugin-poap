#include "poap_plugin.h"

void handle_finalize(void *parameters) {
    ethPluginFinalize_t *msg = (ethPluginFinalize_t *) parameters;
    poap_parameters_t *context = (poap_parameters_t *) msg->pluginContext;
    if (context->valid) {
        msg->numScreens = 3;

    msg->uiType = ETH_UI_TYPE_GENERIC;

    msg->numScreens = 2;

   // msg->tokenLookup1 = context->poap_token;

    msg->result = ETH_PLUGIN_RESULT_OK;
}
