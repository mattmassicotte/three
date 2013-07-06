#include "transactional_memory.h"
#include "cpu.h"

#include <stdio.h>

static bool _three_transaction_supported = false;

void three_transaction_initialize(void) {
    fprintf(stderr, "[runtime] initializing three transactions\n");

    _three_transaction_supported = three_cpu_has_capability(THREE_CPU_TRANSACTIONAL_MEMORY);
}

bool three_begin_transaction(bool strict) {
    if (strict && !_three_transaction_supported) {
        fprintf(stderr, "[runtime] strict transactions not supported by this CPU\n");
    }

    fprintf(stderr, "[runtime] beginning %s transaction\n", strict ? "strict" : "loose");

    return true;
}

bool three_end_transaction(bool strict) {
    fprintf(stderr, "[runtime] ending %s transaction\n", strict ? "strict" : "loose");

    return true;
}
