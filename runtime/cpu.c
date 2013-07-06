#include "cpu.h"

#include <assert.h>

#define CPUID_LEVEL_GET_VENDOR_ID (0)
#define CPUID_EAX (0)
#define CPUID_EBX (1)
#define CPUID_ECX (2)
#define CPUID_EDX (3)
#define CPUID

#ifdef _WIN32
// MSVC has an intrinsic for cpuid
#define cpuid __cpuid
#else
static __inline int cpuid(int info[4], int level) {
    __asm__ __volatile__ (
        "cpuid":
        "=a" (info[CPUID_EAX]),
        "=b" (info[CPUID_EBX]),
        "=c" (info[CPUID_ECX]),
        "=d" (info[CPUID_EDX]) :
        "a" (level)
    );

    return 1;
}
#endif

static bool cpuid_register_bit_check(int level, int reg, int bit) {
    assert(reg >= 0 && reg <= 3);

    int info[4];

    // verify that the CPU supports the query level
    if (level > 0 && level < 0x80000000) {
        cpuid(info, CPUID_LEVEL_GET_VENDOR_ID);

        if (level > info[CPUID_EAX]) {
            return false;
        }
    } else if (level > 0x80000000) {
        cpuid(info, 0x80000000);

        if (level > info[CPUID_EAX]) {
            return false;
        }
    }

    cpuid(info, level);

    return (info[reg] & (1 << bit)) != 0;
}

bool three_cpu_has_capability(three_cpu_capability_t capability) {
    switch (capability) {
        case THREE_CPU_64BIT:
            // CPUID.80000001H.EDX.LM [bit 29] "Long Mode"
            return cpuid_register_bit_check(0x80000001, CPUID_EDX, 29);
        case THREE_CPU_LOCK_ELISON:
            // CPUID.07H.EBX.HLE [bit 4] "Hardware Lock Elision"
            return cpuid_register_bit_check(0x7, CPUID_EBX, 4);
        case THREE_CPU_TRANSACTIONAL_MEMORY:
            // CPUID.07H.EBX.RTM [bit 11] = 1 "Restricted Transactional Memory"
            return cpuid_register_bit_check(0x7, CPUID_EBX, 11);
    }

    return false;
}
