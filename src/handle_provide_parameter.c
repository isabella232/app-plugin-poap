#include "poap_plugin.h"

static void handle_beneficiary(const ethPluginProvideParameter_t *msg, poap_parameters_t *context) {
    memset(context->beneficiary, 0, sizeof(context->beneficiary));
    memcpy(context->beneficiary,
           &msg->parameter[PARAMETER_LENGTH - ADDRESS_LENGTH],
           sizeof(context->beneficiary));
    PRINTF("BENEFICIARY: %.*H\n", ADDRESS_LENGTH, context->beneficiary);
}

static void handle_token_received(const ethPluginProvideParameter_t *msg,
                                  poap_parameters_t *context) {
    memset(context->token_received, 0, sizeof(context->token_received));
    memcpy(context->token_received,
           &msg->parameter[PARAMETER_LENGTH - ADDRESS_LENGTH],
           sizeof(context->token_received));
    PRINTF("TOKEN RECEIVED: %.*H\n", ADDRESS_LENGTH, context->token_received);
}

static void handle_mint_token(ethPluginProvideParameter_t *msg, poap_parameters_t *context) {
    switch (context->next_param) {
        case EVENT_ID:
            context->next_param = TOKEN_RECEIVED;
            break;
        case TOKEN_RECEIVED:  // path[1] -> id of the token received
            handle_token_received(msg, context);
            context->next_param = BENEFICIARY;
            break;
        case BENEFICIARY:  // to
            handle_beneficiary(msg, context);
            context->next_param = NONE;
            break;
        case NONE:
            break;
        default:
            PRINTF("Param not supported\n");
            msg->result = ETH_PLUGIN_RESULT_ERROR;
            break;
    }
}

void handle_provide_parameter(void *parameters) {
    ethPluginProvideParameter_t *msg = (ethPluginProvideParameter_t *) parameters;
    poap_parameters_t *context = (poap_parameters_t *) msg->pluginContext;

    msg->result = ETH_PLUGIN_RESULT_OK;

    if (context->skip) {
        // Skip this step, and don't forget to decrease skipping counter.
        context->skip--;
    } else {
        if ((context->offset) && msg->parameterOffset != context->checkpoint + context->offset) {
            PRINTF("offset: %d, checkpoint: %d, parameterOffset: %d\n",
                   context->offset,
                   context->checkpoint,
                   msg->parameterOffset);
            return;
        }
        context->offset = 0;  // Reset offset
        switch (context->selectorIndex) {
            case MINT_TOKEN:
                handle_mint_token(msg, context);
            default:
                PRINTF("Selector Index %d not supported\n", context->selectorIndex);
                msg->result = ETH_PLUGIN_RESULT_ERROR;
                break;
        }
    }
}