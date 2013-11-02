// C11 threads.h

#ifndef __THREADS_H
#define __THREADS_H

#include "../platform.h"

#if THREE_PLATFORM_POSIX
#include <time.h>
#include <pthread.h>
#include <sys/cdefs.h>

typedef pthread_cond_t  cnd_t;
typedef pthread_t       thrd_t;
typedef pthread_key_t   tss_t;
typedef pthread_mutex_t mtx_t;
typedef pthread_once_t  once_flag;

#define ONCE_FLAG_INIT PTHREAD_ONCE_INIT
#else
#error Not supported on this platform
#endif

typedef void (*tss_dtor_t)(void*);
typedef int (*thrd_start_t)(void*);

enum {
    mtx_plain     = 0,
    mtx_timed     = 1,
    mtx_recursive = 2
};

enum {
    thrd_success = 0,
    thrd_timeout,
    thrd_error,
    thrd_busy,
    thrd_nomem
};

__BEGIN_DECLS

// threads
int    thrd_create(thrd_t *thr, thrd_start_t func, void *arg);
thrd_t thrd_current(void);
int    thrd_detach(thrd_t thr);
int    thrd_equal(thrd_t thr0, thrd_t thr1);
void   thrd_exit(int res);
int    thrd_join(thrd_t thr, int *res);
void   thrd_sleep(const struct timespec* restrict time_point, struct timespec* remaining);
void   thrd_yield(void);

// mutual exclusion
void mtx_destroy(mtx_t *mtx);
int  mtx_init(mtx_t *mtx, int type);
int  mtx_lock(mtx_t *mtx);
int  mtx_timedlock(mtx_t *mtx, const struct timespec* restrict time_point);
int  mtx_trylock(mtx_t *mtx);
int  mtx_unlock(mtx_t *mtx);

// call once
void call_once(once_flag *flag, void (*func)(void));

// condition variables
int  cnd_broadcast(cnd_t *cond);
void cnd_destroy(cnd_t *cond);
int  cnd_init(cnd_t *cond);
int  cnd_signal(cnd_t *cond);
int  cnd_timedwait(cnd_t *cond, mtx_t *mtx, const struct timespec* restrict time_point);
int  cnd_wait(cnd_t *cond, mtx_t *mtx);

// thread-local storage
int   tss_create(tss_t *key, tss_dtor_t dtor);
void  tss_delete(tss_t key);
void *tss_get(tss_t key);
int   tss_set(tss_t key, void *val);

__END_DECLS

#endif //__THREADS_H
