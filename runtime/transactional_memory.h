// transactional_memory.h
//

#pragma once

#include "platform.h"

#include <stdint.h>
#include <stdbool.h>

typedef bool (three_transaction_fallback)(void* context);

typedef struct three_transaction {
    uint32_t flags;
    void* context;
    three_transaction_fallback* begin_fn;
    three_transaction_fallback* end_fn;
} three_transaction_t;

#define THREE_MAKE_TRANSACTION(ctx, begin, end) ((three_transaction_t){0, ctx, begin, end})
#define THREE_MAKE_DEFAULT_TRANSACTION() THREE_MAKE_TRANSACTION(NULL, NULL, NULL)

// transactional memory
void three_transaction_initialize(void);

bool three_transaction_begin(three_transaction_t* tx);
bool three_transaction_end(three_transaction_t* tx);
bool three_transaction_abort(three_transaction_t* tx);
bool three_transaction_in_progress(void);
