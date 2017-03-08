#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "shmemi_pmi1.h"
#include "shmemi_pmi2.h"
#include "shmemi_pmix.h"
#include "logger.h"
#include "timer.h"
#include "malloc.h"

typedef
enum pmi_ver {
    PMI_VERSION_NONE,
    PMI_VERSION_1,
    PMI_VERSION_2,
    PMI_VERSION_X
} pmi_ver_t;

static pmi_ver_t pmi_version;
static char *pmi_verstr;

typedef struct pmi_lookup {
    char *env_var;
    pmi_ver_t ver;
    char *str;
} pmi_lookup_t;

static
pmi_lookup_t pmi_table[] = {
    { "PMIX_RANK", PMI_VERSION_X,    "x"    },
    { "PMI2_RANK", PMI_VERSION_2,    "2"    },
    { "PMI_RANK",  PMI_VERSION_1,    "1"    },
    { NULL,        PMI_VERSION_NONE, "none" }
};

static
void
select_pmi_version(void)
{
    pmi_lookup_t *tp = pmi_table;

    while (tp->env_var != NULL) {
        if (getenv(tp->env_var) != NULL) {
            goto out;
            /* NOT REACHED */
        }
        tp += 1;
    }

 out:
    pmi_version = tp->ver;
    pmi_verstr = tp->str;
}

void
shmem_init(void)
{
    shmemi_timer_init();
    shmemi_logger_init();
    shmemi_malloc_init();

    select_pmi_version();

    switch (pmi_version) {
    case PMI_VERSION_1:
        shmemi_init_pmi1();
        break;
    case PMI_VERSION_2:
        shmemi_init_pmi2();
        break;
    case PMI_VERSION_X:
        shmemi_init_pmix();
        break;
    default:
        logger(LOG_FATAL, "Unknown or missing PMI version");
        break;
    }

    logger(LOG_INIT, "using PMI%s", pmi_verstr);

    /* urgh! */
}

void
shmem_finalize(void)
{

    switch (pmi_version) {
    case PMI_VERSION_1:
        shmemi_finalize_pmi1();
        break;
    case PMI_VERSION_2:
        shmemi_finalize_pmi2();
        break;
    case PMI_VERSION_X:
        shmemi_finalize_pmix();
        break;
    default:
        break;
    }

    shmemi_malloc_finalize();
    shmemi_logger_finalize();
    shmemi_timer_finalize();
}
