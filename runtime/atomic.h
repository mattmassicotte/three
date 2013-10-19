#pragma once

#include <three/c11/stdatomic.h>
#include <three/runtime/types.h>

#define THREE_CHECK_ATOMIC(type) _Static_assert(THREE_CHECK_SIZEALIGN(_Atomic(type), type), "Atomic and non-atomic must be compatiable")
