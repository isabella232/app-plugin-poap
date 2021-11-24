#pragma once

#include "eth_internals.h"
#include "eth_plugin_interface.h"
#include <string.h>

#define PARAMETER_LENGTH 32
#define SELECTOR_SIZE    4

#define RUN_APPLICATION 1

#define NUM_POAP_SELECTORS 1

#define SELECTOR_SIZE      4

#define PLUGIN_NAME "Poap"

#define TOKEN_RECEIVED_FOUND 1 << 1

typedef enum { MINT_TOKEN } poapSelector_t;

extern const uint8_t *const POAP_SELECTORS[NUM_POAP_SELECTORS];

typedef enum {
    MINT_SCREEN,
    TOKEN_SCREEN,
    WARN_SCREEN,
    BENEFICIARY_SCREEN,
    ERROR,
} screens_t;

// Would've loved to make this an enum but we don't have enough room because enums are `int` and not
// `uint8_t`.

#define EVENT_ID       0
#define TOKEN_RECEIVED 1
#define BENEFICIARY    2
#define NONE           3

// Number of decimals used when the token wasn't found in the CAL.
#define DEFAULT_DECIMAL WEI_TO_ETHER

// Ticker used when the token wasn't found in the CAL.
#define DEFAULT_TICKER ""

// Shared global memory with Ethereum app. Must be at most 5 * 32 bytes.
typedef struct poap_parameters_t {
    uint8_t token_received[INT256_LENGTH];
    char beneficiary[ADDRESS_LENGTH];
    char ticker_received[MAX_TICKER_LEN];

    uint16_t offset;
    uint16_t checkpoint;
    uint8_t next_param;
    uint8_t tokens_found;
    uint8_t valid;
    uint8_t decimals;
    uint8_t selectorIndex;
    uint8_t array_len;
    uint8_t skip;
    // 4 * 1 + 2 * 2 + 7 * 1 == 8 + 7 == 15 bytes. There are 16 - 15 == 1 byte left.
} poap_parameters_t;

void handle_init_contract(void *parameters);
void handle_provide_parameter(void *parameters);
void handle_query_contract_ui(void *parameters);
void handle_finalize(void *parameters);
void handle_provide_token(void *parameters);
void handle_query_contract_id(void *parameters);