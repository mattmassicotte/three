#include "threads.h"

#include <assert.h>
#include <errno.h>
#include <stdint.h>

// threads
int thrd_create(thrd_t *thr, thrd_start_t func, void *arg) {
    if (!thr) {
        return thrd_error;
    }
    
    // There is a bit of a impedance mis-match between the definitions of thrd_start_t and
    // pthread's start_routine.  But, close enough, right?
    
    return (pthread_create(thr, NULL, (void *(*)(void *))func, arg) == 0) ? thrd_success : thrd_error;
}

thrd_t thrd_current(void) {
    return pthread_self();
}

int thrd_detach(thrd_t thr) {
    return (pthread_detach(thr) == 0) ? thrd_success : thrd_error;
}

int thrd_equal(thrd_t thr0, thrd_t thr1) {
    return pthread_equal(thr0, thr1);
}

void thrd_exit(int res) {
    pthread_exit((void*)(intptr_t)res);
}

int thrd_join(thrd_t thr, int *res) {
    void *code;
    
    if (pthread_join(thr, &code) != 0) {
        return thrd_error;
    }
    
    if (res) {
        *res = (int)code;
    }
    
    return thrd_success;
}

void thrd_sleep(const struct timespec* restrict time_point, struct timespec* remaining) {
    assert(time_point);

    nanosleep(time_point, remaining);
}

void thrd_yield(void) {
#if THREE_PLATFORM_DARWIN
    pthread_yield_np();
#else
    pthread_yield();
#endif
}

// mutual exclusion
void mtx_destroy(mtx_t *mtx) {
    pthread_mutex_destroy(mtx);
}

int mtx_init(mtx_t *mtx, int type) {
    if (!mtx) {
        return thrd_error;
    }

    if ((type & (mtx_plain|mtx_recursive)) == 0) {
        return thrd_error;
    }

    if ((type & (mtx_timed|mtx_recursive)) == 0) {
        return thrd_error;
    }
    
    pthread_mutexattr_t attr;

    pthread_mutexattr_init(&attr);
    
    if ((type & mtx_recursive) != 0) {
        pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
    }
    
    pthread_mutex_init(mtx, &attr);
    pthread_mutexattr_destroy(&attr);
    
    return thrd_success;
}

int mtx_lock(mtx_t *mtx) {
    if (!mtx) {
        return thrd_error;
    }
    
    pthread_mutex_lock(mtx);
    
    return thrd_success;
}

int mtx_timedlock(mtx_t *mtx, const struct timespec* restrict time_point) {
    assert(0 && "mtx_timedlock unimplemented");
}

int mtx_trylock(mtx_t *mtx) {
    switch (pthread_mutex_trylock(mtx)) {
        case 0:
            return thrd_success;
        case EBUSY:
            return thrd_busy;
        default:
            return thrd_error;
    }
}

int mtx_unlock(mtx_t *mtx) {
    if (!mtx) {
        return thrd_error;
    }

    pthread_mutex_unlock(mtx);

    return thrd_success;
}

// call once
void call_once(once_flag *flag, void (*func)(void)) {
    pthread_once(flag, func);
}

// condition variables
int cnd_broadcast(cnd_t *cond) {
    if (!cond) {
        return thrd_error;
    }

    pthread_cond_broadcast(cond);

    return thrd_success;
}

void cnd_destroy(cnd_t *cond) {
    pthread_cond_destroy(cond);
}

int cnd_init(cnd_t *cond) {
    if (!cond) {
        return thrd_error;
    }

    pthread_cond_init(cond, NULL);

    return thrd_success;
    
}

int cnd_signal(cnd_t *cond) {
    if (!cond) {
        return thrd_error;
    }

    pthread_cond_signal(cond);

    return thrd_success;
}

int cnd_timedwait(cnd_t *cond, mtx_t *mtx, const struct timespec* restrict time_point) {
    assert(0 && "cnd_timedwait unimplemented");
}

int cnd_wait(cnd_t *cond, mtx_t *mtx) {
    if (!cond) {
        return thrd_error;
    }

    pthread_cond_wait(cond, mtx);

    return thrd_success;
    
}

// thread-local storage
int tss_create(tss_t *key, tss_dtor_t dtor) {
    if (!key) {
        return thrd_error;
    }

    return (pthread_key_create(key, dtor) == 0) ? thrd_success : thrd_error;
}

void tss_delete(tss_t key) {
    pthread_key_delete(key);
}

void *tss_get(tss_t key) {
    return pthread_getspecific(key);
}

int tss_set(tss_t key, void *val) {
    return (pthread_setspecific(key, val) == 0) ? thrd_success : thrd_error;
}
