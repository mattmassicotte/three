#pragma once

#define THREE_ATTR_NORETURN        __attribute__((noreturn))
#define THREE_ATTR_INLINE_ALWAYS   __attribute__((always_inline)) inline
#define THREE_ATTR_INLINE_NEVER    __attribute__((noinline))

#define THREE_ATTR_FLOW_LOCAL      __attribute__((nothrow))
#define THREE_ATTR_FLOW_NONLOCAL

#define THREE_ATTR_ABI_FASTCALL    __attribute__((fastcall))
#define THREE_ATTR_ABI_THISCALL    __attribute__((thiscall))

#define THREE_ATTR_NULLABLE
