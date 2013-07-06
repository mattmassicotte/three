// three_cpu.h

#pragma once

#include <stdbool.h>

// CPU capabilities
typedef enum {
    THREE_CPU_64BIT,

    THREE_CPU_LOCK_ELISON,
    THREE_CPU_TRANSACTIONAL_MEMORY
} three_cpu_capability_t;

bool three_cpu_has_capability(three_cpu_capability_t capability);
