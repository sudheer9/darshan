/*
 *  (C) 2009 by Argonne National Laboratory.
 *      See COPYRIGHT in top-level directory.
 */

/* TODO: is this the right thing to do to get ahold of RTLD_NEXT? */
#define _GNU_SOURCE

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/uio.h>
#include <pthread.h>
#include <sys/mman.h>
#include <search.h>
#include <assert.h>
#include <dlfcn.h>

/* TODO: need a mechanism to disable all of these wrappers (or make them
 * pass through directly to underlying fn) if MPI is not initialized
 */

#ifdef DARSHAN_PRELOAD
int (*__real_open)(const char* path, int flags, ...) = NULL;
int (*__real_open64)(const char* path, int flags, ...) = NULL;
#else
extern int __real_open(const char *path, int flags, ...);
extern int __real_open64(const char *path, int flags, ...);
#endif

#ifdef DARSHAN_PRELOAD
int open64(const char* path, int flags, ...)
#else
int __wrap_open64(const char* path, int flags, ...)
#endif
{
    int mode = 0;
    int ret;

#ifdef DARSHAN_PRELOAD
    /* TODO: maybe put this in a constructor or something so that we can do
     * them all at once somewhere?
     */
    if(!__real_open64)
        __real_open64 = dlsym(RTLD_NEXT, "open64");
    assert(__real_open64);
#endif

    printf("Hello world, I hijacked open64()!\n");

    if (flags & O_CREAT) 
    {
        va_list arg;
        va_start(arg, flags);
        mode = va_arg(arg, int);
        va_end(arg);

        ret = __real_open64(path, flags, mode);
    }
    else
    {
        ret = __real_open64(path, flags);
    }

    return(ret);
}

#ifdef DARSHAN_PRELOAD
int open(const char* path, int flags, ...)
#else
int __wrap_open(const char* path, int flags, ...)
#endif
{
    int mode = 0;
    int ret;

#ifdef DARSHAN_PRELOAD
    /* TODO: maybe put this in a constructor or something so that we can do
     * them all at once somewhere?
     */
    if(!__real_open)
        __real_open = dlsym(RTLD_NEXT, "open");
    assert(__real_open);
#endif

    printf("Hello world, I hijacked open()!\n");

    if (flags & O_CREAT) 
    {
        va_list arg;
        va_start(arg, flags);
        mode = va_arg(arg, int);
        va_end(arg);

        ret = __real_open(path, flags, mode);
    }
    else
    {
        ret = __real_open(path, flags);
    }

    return(ret);
}

/*
 * Local variables:
 *  c-indent-level: 4
 *  c-basic-offset: 4
 * End:
 *
 * vim: ts=8 sts=4 sw=4 expandtab
 */
