/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "shmem.h"

#include <stdio.h>
#include <string.h>
#include <sys/time.h>

#define GLUE(_thr) { SHMEM_THREAD_##_thr,  #_thr }

#define NONE (SHMEM_THREAD_SINGLE - 1)

static struct thread_encdec {
    int level;
    const char *name;
} threads_table[] = {
    GLUE(SINGLE),
    GLUE(FUNNELED),
    GLUE(SERIALIZED),
    GLUE(MULTIPLE),
    { NONE, "unknown" }
};

const char *
shmemu_thread_name(int tl)
{
    struct thread_encdec *tp = threads_table;

    while (tp->level != NONE) {
        if (tp->level == tl) {
            return tp->name;
            /* NOT REACHED */
        }
        ++tp;
    }
    return tp->name;
}

int
shmemu_thread_level(const char *tn)
{
    struct thread_encdec *tp = threads_table;

    while (tp->level != NONE) {
        if (strncmp(tp->name, tn, strlen(tp->name)) == 0) {
            return tp->level;
            /* NOT REACHED */
        }
        ++tp;
    }
    return tp->level;
}
