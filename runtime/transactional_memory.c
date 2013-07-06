#include "transactional_memory.h"
#include "cpu.h"
#include "c11/threads.h"

#include <assert.h>
#include <stdio.h>

static once_flag _three_transactions_once_flag = ONCE_FLAG_INIT;

static bool _three_transaction_supported = false;
static mtx_t _three_transaction_mtx;

void three_transaction_initialize(void) {
    fprintf(stderr, "[runtime] initializing three transactions\n");

    _three_transaction_supported = three_cpu_has_capability(THREE_CPU_TRANSACTIONAL_MEMORY);
    if (!_three_transaction_supported) {
        assert(mtx_init(&_three_transaction_mtx, mtx_recursive) == thrd_success);
    }
}

bool three_transaction_begin(bool strict) {
    call_once(&_three_transactions_once_flag, three_transaction_initialize);

    fprintf(stderr, "[runtime] beginning %s transaction\n", strict ? "strict" : "loose");

    if (_three_transaction_supported) {
        // this is the easy case
        fprintf(stderr, "[runtime] strict transactions not implemented\n");
    }

    if (strict) {
        fprintf(stderr, "[runtime] strict transactions not supported by this CPU\n");
        return false;
    }

    return mtx_lock(&_three_transaction_mtx) == thrd_success;
}

bool three_transaction_end(bool strict) {
    fprintf(stderr, "[runtime] ending %s transaction\n", strict ? "strict" : "loose");

    if (_three_transaction_supported) {
        // this is the easy case
        fprintf(stderr, "[runtime] strict transactions not implemented\n");
    }

    if (strict) {
        fprintf(stderr, "[runtime] strict transactions not supported by this CPU\n");
        return false;
    }

    return mtx_unlock(&_three_transaction_mtx) == thrd_success;
}
