#pragma once

#include <stddef.h>

// closure types
enum {
    THREE_CLOSURE_FLAGS_DEFAULT = 0,
    THREE_CLOSURE_FLAGS_HAS_REFERENCES = 1
};
typedef int three_closure_flags_t;

typedef void* (*three_closure_function_t)(void*, ...);
typedef struct {
    three_closure_function_t function;
    three_closure_flags_t flags;
    size_t env_size;
    void*  env;
} three_closure_t;

// closure usage helpers
#define THREE_CAPTURE_ENV(env_type, ...) struct env_type env_type ## _value = (struct env_type){__VA_ARGS__}
#define THREE_MAKE_CLOSURE(func, flags) ((three_closure_t){(three_closure_function_t)func, flags, sizeof(struct func ## _env), (void*)&(func ## _env ## _value)})
#define THREE_CALL_CLOSURE(func_type, closure, ...) ((func_type)closure.function)(closure.env, ##__VA_ARGS__)

#define THREE_CHECK_CLOSURE_FUNCTION(func) _Static_assert(THREE_CHECK_SIZEALIGN(__typeof__(func)*, three_closure_function_t), "Closure function must be compatiable")

static size_t three_closure_get_size(three_closure_t closure) {
    return sizeof(three_closure_t) + closure.env_size;
}

static three_closure_flags_t three_closure_get_flags(three_closure_t closure) {
    return closure.flags;
}

three_closure_t* three_closure_copy(three_closure_t closure);
void three_closure_release(three_closure_t* closure);
