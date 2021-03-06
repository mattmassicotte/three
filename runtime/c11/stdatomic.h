// C11 stdatomic.h

#ifndef __STDATOMIC_H
#define __STDATOMIC_H

#include <stddef.h>
#include <stdint.h>

#if defined(__GNUC__)
#include "stdatomic_gcc.h"
#endif

#define ATOMIC_BOOL_LOCK_FREE       __GCC_ATOMIC_BOOL_LOCK_FREE
#define ATOMIC_CHAR_LOCK_FREE       __GCC_ATOMIC_CHAR_LOCK_FREE
#define ATOMIC_CHAR16_T_LOCK_FREE   __GCC_ATOMIC_CHAR16_T_LOCK_FREE
#define ATOMIC_CHAR32_T_LOCK_FREE   __GCC_ATOMIC_CHAR32_T_LOCK_FREE
#define ATOMIC_WCHAR_T_LOCK_FREE    __GCC_ATOMIC_WCHAR_T_LOCK_FREE
#define ATOMIC_SHORT_T_LOCK_FREE    __GCC_ATOMIC_SHORT_T_LOCK_FREE
#define ATOMIC_INT_T_LOCK_FREE      __GCC_ATOMIC_INT_T_LOCK_FREE
#define ATOMIC_LONG_T_LOCK_FREE     __GCC_ATOMIC_LONG_T_LOCK_FREE
#define ATOMIC_LLONG_T_LOCK_FREE    __GCC_ATOMIC_LLONG_T_LOCK_FREE
#define ATOMIC_POINTER_T_LOCK_FREE  __GCC_ATOMIC_POINTER_T_LOCK_FREE

#define ATOMIC_VAR_INIT(value) (value)
#define atomic_init __c11_atomic_init

enum memory_order {
    memory_order_relaxed = __ATOMIC_RELAXED,
    memory_order_consume = __ATOMIC_CONSUME,
    memory_order_acquire = __ATOMIC_ACQUIRE,
    memory_order_release = __ATOMIC_RELEASE,
    memory_order_acq_rel = __ATOMIC_ACQ_REL,
    memory_order_seq_cst = __ATOMIC_SEQ_CST
};

#define kill_dependency(y) (y)

#define atomic_thread_fence __c11_atomic_thread_fence
#define atomic_signal_fence __c11_atomic_signal_fence

#define atomic_is_lock_free(obj) __atomic_is_lock_free(sizeof(sizeof(*obj))

#ifdef __cplusplus
typedef _Atomic(bool)               atomic_bool;
#else
typedef _Atomic(_Bool)              atomic_bool;
#endif
typedef _Atomic(char)               atomic_char;
typedef _Atomic(signed char)        atomic_schar;
typedef _Atomic(unsigned char)      atomic_uchar;
typedef _Atomic(short)              atomic_short;
typedef _Atomic(unsigned short)     atomic_ushort;
typedef _Atomic(int)                atomic_int;
typedef _Atomic(unsigned int)       atomic_uint;
typedef _Atomic(long)               atomic_long;
typedef _Atomic(unsigned long)      atomic_ulong;
typedef _Atomic(long long)          atomic_llong;
typedef _Atomic(unsigned long long) atomic_ullong;
typedef _Atomic(uint_least16_t)     atomic_char16_t;
typedef _Atomic(uint_least32_t)     atomic_char32_t;
typedef _Atomic(wchar_t)            atomic_wchar_t;
typedef _Atomic(int_least8_t)       atomic_int_least8_t;
typedef _Atomic(uint_least8_t)      atomic_uint_least8_t;
typedef _Atomic(int_least16_t)      atomic_int_least16_t;
typedef _Atomic(uint_least16_t)     atomic_uint_least16_t;
typedef _Atomic(int_least32_t)      atomic_int_least32_t;
typedef _Atomic(uint_least32_t)     atomic_uint_least32_t;
typedef _Atomic(int_least64_t)      atomic_int_least64_t;
typedef _Atomic(uint_least64_t)     atomic_uint_least64_t;
typedef _Atomic(int_fast8_t)        atomic_int_fast8_t;
typedef _Atomic(uint_fast8_t)       atomic_uint_fast8_t;
typedef _Atomic(int_fast16_t)       atomic_int_fast16_t;
typedef _Atomic(uint_fast16_t)      atomic_uint_fast16_t;
typedef _Atomic(int_fast32_t)       atomic_int_fast32_t;
typedef _Atomic(uint_fast32_t)      atomic_uint_fast32_t;
typedef _Atomic(int_fast64_t)       atomic_int_fast64_t;
typedef _Atomic(uint_fast64_t)      atomic_uint_fast64_t;
typedef _Atomic(intptr_t)           atomic_intptr_t;
typedef _Atomic(uintptr_t)          atomic_uintptr_t;
typedef _Atomic(size_t)             atomic_size_t;
typedef _Atomic(ptrdiff_t)          atomic_ptrdiff_t;
typedef _Atomic(intmax_t)           atomic_intmax_t;
typedef _Atomic(uintmax_t)          atomic_uintmax_t;

#define atomic_store_explicit                   __c11_atomic_store
#define atomic_load_explicit                    __c11_atomic_load
#define atomic_exchange_explicit                __c11_atomic_exchange
#define atomic_compare_exchange_strong_explicit __c11_atomic_compare_exchange_strong
#define atomic_compare_exchange_weak_explicit   __c11_atomic_compare_exchange_weak
#define atomic_fetch_add_explicit               __c11_atomic_fetch_add
#define atomic_fetch_sub_explicit               __c11_atomic_fetch_sub
#define atomic_fetch_or_explicit                __c11_atomic_fetch_or
#define atomic_fetch_xor_explicit               __c11_atomic_fetch_xor
#define atomic_fetch_and_explicit               __c11_atomic_fetch_and

#define atomic_store(object, desired)     atomic_store_explicit(object, desired, memory_order_seq_cst)
#define atomic_load(object)               atomic_load_explicit(object, memory_order_seq_cst)
#define atomic_exchange(object, desired)  atomic_exchange_explicit(object, desired, memory_order_seq_cst)
#define atomic_compare_exchange_strong(object, expected, desired) atomic_compare_exchange_strong_explicit(object, expected, desired, memory_order_seq_cst, memory_order_seq_cst)
#define atomic_compare_exchange_weak(object, expected, desired) atomic_compare_exchange_weak_explicit(object, expected, desired, memory_order_seq_cst, memory_order_seq_cst)
#define atomic_fetch_add(object, operand) atomic_fetch_add_explicit(object, operand, memory_order_seq_cst)
#define atomic_fetch_sub(object, operand) atomic_fetch_sub_explicit(object, operand, memory_order_seq_cst)
#define atomic_fetch_or(object, operand)  atomic_fetch_or_explicit(object, operand, memory_order_seq_cst)
#define atomic_fetch_xor(object, operand) atomic_fetch_xor_explicit(object, operand, memory_order_seq_cst)
#define atomic_fetch_and(object, operand) atomic_fetch_and_explicit(object, operand, memory_order_seq_cst)

typedef struct atomic_flag { atomic_bool _Value; } atomic_flag;

#define ATOMIC_FLAG_INIT { 0 }

#define atomic_flag_test_and_set_explicit(obj, order) atomic_exchange_explicit(&(obj)->_Value, 1, order)
#define atomic_flag_test_and_set(obj)                 atomic_flag_test_and_set_explicit(obj, memory_order_seq_cst)

#define atomic_flag_clear_explicit(obj, order)        atomic_store(&(obj)->_Value, 0, order)
#define atomic_flag_clear(obj)                        atomic_flag_clear_explicit(obj, memory_order_seq_cst)

#endif // __STDATOMIC_H
