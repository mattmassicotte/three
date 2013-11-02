#include "closure.h"

#include <assert.h>
#include <string.h>
#include <stdlib.h>

three_closure_t* three_closure_copy(three_closure_t closure) {
    three_closure_t* copy;

    // This little trick of malloc'ing once seems to have some incorrect
    // pointer math.  Obviously.

    // copy = (three_closure_t*)malloc(three_closure_get_size(closure));
    copy = (three_closure_t*)malloc(sizeof(three_closure_t));

    copy->function = closure.function;
    copy->env_size = closure.env_size;

    if (closure.env_size == 0) {
        copy->env = NULL;
    } else {
        // if the closure has an environment, we need to copy it into the
        // space allocated just after the closure struct itself
        // copy->env = (void *)(copy + sizeof(three_closure_t));
        copy->env = (void *)malloc(closure.env_size);

        memcpy(copy->env, closure.env, closure.env_size);
    }

    return copy;
}

void three_closure_release(three_closure_t* closure) {
    assert(closure);
    free(closure->env);
    free(closure);
}
