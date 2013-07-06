#pragma once

// closure types
typedef void* (*three_closure_function_t)(void*, ...);
typedef struct {
    three_closure_function_t function;
    int   env_size;
    void* env;
} three_closure_t;

// closure usage helpers
#define THREE_MAKE_CLOSURE(func) ((three_closure_t){(three_closure_function_t)func, sizeof(struct func ## _env), (void*)&(func ## _env ## _value)})
#define THREE_CALL_CLOSURE(clos_type, clos, ...) (((clos_type)clos.function)(clos.env, __VA_ARGS__))
