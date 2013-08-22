// intel_tsx.h
//

#pragma once

#include "annotations.h"

#define _XBEGIN_STARTED     (~0u)
#define _XABORT_EXPLICIT    (1 << 0)
#define _XABORT_RETRY       (1 << 1)
#define _XABORT_CONFLICT    (1 << 2)
#define _XABORT_CAPACITY    (1 << 3)
#define _XABORT_DEBUG       (1 << 4)
#define _XABORT_NESTED      (1 << 5)

#define _XABORT_CODE(x)     (((x) >> 24) & 0xff)

static THREE_ATTR_INLINE_ALWAYS int _xbegin(void) {
    int ret = _XBEGIN_STARTED;

    __asm__ volatile(".byte 0xc7, 0xf8 ; .long 0" : "+a" (ret) :: "memory");

    return ret;
}

static THREE_ATTR_INLINE_ALWAYS void _xend(void) {
    __asm__ volatile(".byte 0x0f, 0x01, 0xd5" ::: "memory");
}

static THREE_ATTR_INLINE_ALWAYS void _xabort(const unsigned int status) {
    __asm__ volatile(".byte 0xc6, 0xf8, %P0" :: "i" (status) : "memory");
}

static THREE_ATTR_INLINE_ALWAYS unsigned char _xtest(void) {
    unsigned char out;

    __asm__ volatile(".byte 0x0f, 0x01, 0xd6 ; setnz %0" : "=r" (out) :: "memory");

    return out;
}
