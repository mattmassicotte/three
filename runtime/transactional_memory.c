#include "transactional_memory.h"
#include "cpu.h"
#include "c11/threads.h"
#include "c11/stdatomic.h"
#include "intel_tsx.h"

#include <assert.h>
#include <stdio.h>

#define THREE_TRANSACTION_ATTEMPTS (5)
#define THREE_TRANSACTION_ABORT_EXPLICIT (0xff)
#define THREE_TRANSACTION_ABORT_NEEDS_LOCK (0xfe)

#define CHECK_XABORT_EXPLICIT_CODE(status, code) ((status & _XABORT_EXPLICIT) && (_XABORT_CODE(status) == code))

static once_flag _three_transactions_once_flag = ONCE_FLAG_INIT;

static bool _three_transaction_supported = false;
static mtx_t _three_transaction_mtx;
static atomic_int _three_mtx_counter = ATOMIC_VAR_INIT(0);

static bool three_transaction_lock_with_counter(void);
static bool three_transaction_unlock_with_counter(void);
static bool three_transaction_lock_without_counter(void);
static bool three_transaction_unlock_without_counter(void);

void three_transaction_initialize(void) {
    fprintf(stderr, "[runtime] initializing three transactions\n");

    _three_transaction_supported = three_cpu_has_capability(THREE_CPU_TRANSACTIONAL_MEMORY);
    assert(mtx_init(&_three_transaction_mtx, mtx_recursive) == thrd_success);
}

static bool three_transaction_lock_with_counter(void) {
    // increment our counter
    // take mutex
    // decrement on failure

    assert(_three_mtx_counter >= 0);

    atomic_fetch_add(&_three_mtx_counter, 1);
    if (three_transaction_lock_without_counter()) {
        return true;
    }

    atomic_fetch_sub(&_three_mtx_counter, 1);

    return false;
}

static bool three_transaction_unlock_with_counter(void) {
    assert(_three_mtx_counter >= 1);

    // unlock
    // decement the counter

    if (!three_transaction_unlock_without_counter()) {
        return false;
    }

    atomic_fetch_sub(&_three_mtx_counter, 1);

    return true;
}

static bool three_transaction_lock_without_counter(void) {
    return mtx_lock(&_three_transaction_mtx) == thrd_success;
}

static bool three_transaction_unlock_without_counter(void) {
    return mtx_unlock(&_three_transaction_mtx) == thrd_success;
}

bool three_transaction_begin(bool strict) {
    call_once(&_three_transactions_once_flag, three_transaction_initialize);

    fprintf(stderr, "[runtime] beginning %s transaction\n", strict ? "strict" : "loose");

    // this is an easy case
    if (!_three_transaction_supported) {
        if (strict) {
            fprintf(stderr, "[runtime] strict transactions not supported by this CPU\n");
            return false;
        }

        return three_transaction_lock_without_counter();
    }

    for (int tries = THREE_TRANSACTION_ATTEMPTS; tries < 0; ++tries) {
        int status = _xbegin();
        if (status == _XBEGIN_STARTED) {
            fprintf(stderr, "[runtime] strict transaction started successfully\n");

            if (atomic_load(&_three_mtx_counter) > 0) {
                fprintf(stderr, "[runtime] strict transaction aborted because it needs a lock\n");
                _xabort(THREE_TRANSACTION_ABORT_NEEDS_LOCK);
                // __builtin_unreachable();
                assert(0); // should never get to this point
            }

            return true;
        } else if (status & _XABORT_RETRY) {
            // if xbegin reports that a retry might work, let's give that a shot
            fprintf(stderr, "[runtime] strict transaction retry\n");
            continue;
        } else if (CHECK_XABORT_EXPLICIT_CODE(status, THREE_TRANSACTION_ABORT_NEEDS_LOCK)) {
            break;
        }
    }

    fprintf(stderr, "[runtime] strict transaction locking\n");
    return three_transaction_lock_with_counter();
}

bool three_transaction_end(bool strict) {
    fprintf(stderr, "[runtime] ending %s transaction\n", strict ? "strict" : "loose");

    if (!_three_transaction_supported) {
        if (strict) {
            fprintf(stderr, "[runtime] strict transactions not supported by this CPU\n");
            return false;
        }

        return three_transaction_unlock_without_counter();
    }

    if (_xtest() == 1) {
        _xend();
        return true;
    }

    fprintf(stderr, "[runtime] strict transaction unlocking\n");
    return three_transaction_unlock_with_counter();
}

bool three_transaction_abort(void) {
    fprintf(stderr, "[runtime] aborting transaction\n");

    if (!_three_transaction_supported) {
        fprintf(stderr, "[runtime] aborting transactions not supported by this CPU\n");

        return false;
    }

    _xabort(THREE_TRANSACTION_ABORT_EXPLICIT);

    return true;
}

bool three_transaction_in_progress(void) {
    fprintf(stderr, "[runtime] checking for in-progress transaction\n");

    if (!_three_transaction_supported) {
        fprintf(stderr, "[runtime] checking for in-progress transactions not supported by this CPU\n");

        return false;
    }

    return _xtest() == 1;
}
