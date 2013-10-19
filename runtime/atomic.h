#pragma once

#include <three/c11/stdatomic.h>
#include <three/c11/stdalign.h>

#define THREE_CHECK_ATOMIC_SIZEOF(type) (sizeof(_Atomic(type)) == sizeof(type))
#define THREE_CHECK_ATOMIC_ALIGNOF(type) (alignof(_Atomic(type)) == alignof(type))

#define THREE_CHECK_ATOMIC(type) _Static_assert(THREE_CHECK_ATOMIC_SIZEOF(type) && THREE_CHECK_ATOMIC_ALIGNOF(type), "Atomic and non-atomic must be compatiable")
