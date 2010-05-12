/*
 *  (C) 2009 by Argonne National Laboratory.
 *      See COPYRIGHT in top-level directory.
 */

#ifndef __DARSHAN_LOG_UTILS_H
#define __DARSHAN_LOG_UTILS_H
#include <darshan-log-format.h>
#include <zlib.h>

struct darshan_fd_s
{
    gzFile gzf;
    int swap_flag;
};
typedef struct darshan_fd_s* darshan_fd;

extern char *darshan_names[];
extern char *darshan_f_names[];

darshan_fd darshan_log_open(const char *name);
int darshan_log_getjob(darshan_fd file, struct darshan_job *job);
int darshan_log_getfile(darshan_fd fd, 
    struct darshan_job* job, 
    struct darshan_file *file);
int darshan_log_getexe(darshan_fd fd, char *buf, int *flag);
int darshan_log_getmounts(darshan_fd fd, int** devs, char*** mnt_pts, char***
    fs_types, int* count, int *flag);
void darshan_log_close(darshan_fd file);
void darshan_log_print_version_warnings(struct darshan_job *job);

/* convenience macros for printing out counters */
#define CP_PRINT_HEADER() printf("#<rank>\t<file>\t<counter>\t<value>\t<name suffix>\t<mount pt>\t<fs type>\n")
#define CP_PRINT(__job, __file, __counter, __mnt_pt, __fs_type) do {\
        printf("%lld\t%llu\t%s\t%lld\t...%s\t%s\t%s\n", \
            lld((__file)->rank), llu((__file)->hash), darshan_names[__counter], \
            lld((__file)->counters[__counter]), (__file)->name_suffix, \
            __mnt_pt, __fs_type); \
} while(0)
#define CP_F_PRINT(__job, __file, __counter, __mnt_pt, __fs_type) do {\
        printf("%lld\t%llu\t%s\t%f\t...%s\t%s\t%s\n", \
            lld((__file)->rank), llu((__file)->hash), darshan_f_names[__counter], \
            (__file)->fcounters[__counter], (__file)->name_suffix, \
            __mnt_pt, __fs_type); \
} while(0)

/* naive byte swap implementation */
#define DARSHAN_BSWAP64(__ptr) do {\
    char __dst_char[8]; \
    char* __src_char = (char*)__ptr; \
    __dst_char[0] = __src_char[7]; \
    __dst_char[1] = __src_char[6]; \
    __dst_char[2] = __src_char[5]; \
    __dst_char[3] = __src_char[4]; \
    __dst_char[4] = __src_char[3]; \
    __dst_char[5] = __src_char[2]; \
    __dst_char[6] = __src_char[1]; \
    __dst_char[7] = __src_char[0]; \
    memcpy(__ptr, __dst_char, 8); \
} while(0)

#endif
