// transactional_memory.h
//

#pragma once

#include <stdbool.h>

// transactional memory
void three_transaction_initialize(void);

bool three_transaction_begin(bool strict);
bool three_transaction_end(bool strict);
bool three_transaction_abort(void);
bool three_transaction_in_progress(void);
