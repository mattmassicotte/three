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
static atomic_uint _three_transaction_mtx_count = 0;

static bool three_transaction_lock(void);
static bool three_transaction_unlock(void);

void three_transaction_initialize(void) {
    fprintf(stderr, "[runtime] initializing three transactions\n");

    _three_transaction_supported = three_cpu_has_capability(THREE_CPU_TRANSACTIONAL_MEMORY);
    assert(mtx_init(&_three_transaction_mtx, mtx_recursive) == thrd_success);
}

static bool three_transaction_lock(void) {
    if (mtx_lock(&_three_transaction_mtx) == thrd_success) {
        atomic_fetch_add(&_three_transaction_mtx_count, 1);

        return true;
    }

    return false;
}

static bool three_transaction_unlock(void) {
    assert(atomic_fetch_sub(&_three_transaction_mtx_count, 1) >= 0);

    return mtx_unlock(&_three_transaction_mtx) == thrd_success;
}

bool three_transaction_begin(three_transaction_t* tx) {
    call_once(&_three_transactions_once_flag, three_transaction_initialize);

    // this is an easy case
    if (!_three_transaction_supported) {
        return three_transaction_lock();
    }

    for (int tries = 0; tries < THREE_TRANSACTION_ATTEMPTS; ++tries) {
        unsigned int status = _xbegin();

        if (atomic_load(&_three_transaction_mtx_count) > 0) {
            _xabort(THREE_TRANSACTION_ABORT_NEEDS_LOCK);
        }

        if (status == _XBEGIN_STARTED) {
            return true;
        }

        // fprintf(stderr, "[runtime] strict transaction xbegin: %x\n", status);

        if (status & _XABORT_EXPLICIT) {
            // called xabort
            if (CHECK_XABORT_EXPLICIT_CODE(status, THREE_TRANSACTION_ABORT_NEEDS_LOCK)) {
                break;
            }

            return false;
        }

        if (status & _XABORT_RETRY) {
            // if xbegin reports that a retry might work, let's give that a shot
            // fprintf(stderr, "[runtime] strict transaction retry\n");
            continue;
        }

        break;
    }

    // fprintf(stderr, "[runtime] transaction fallback locking\n");
    return three_transaction_lock();
}

bool three_transaction_end(three_transaction_t* tx) {
    if (!_three_transaction_supported) {
        // fprintf(stderr, "[runtime] ending loose transaction\n");
        return three_transaction_unlock();
    }

    if (_xtest() == 1) {
        _xend();
        return true;
    }

//    fprintf(stderr, "[runtime] transaction fallback unlocking\n");
    return three_transaction_unlock();
}

bool three_transaction_abort(three_transaction_t* tx) {
    if (!_three_transaction_supported) {
        fprintf(stderr, "[runtime] aborting transactions not supported by this CPU\n");

        return false;
    }

    _xabort(THREE_TRANSACTION_ABORT_EXPLICIT);

    return true;
}

bool three_transaction_in_progress(void) {
    if (!_three_transaction_supported) {
        fprintf(stderr, "[runtime] checking for in-progress transactions not supported by this CPU\n");

        return false;
    }

    return _xtest() == 1;
}
