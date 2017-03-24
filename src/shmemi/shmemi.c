#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "shmemi.h"
#include "shmemu.h"
#include "shmemc.h"

#include "pmix-client.h"
#include "pmi2-client.h"
#include "pmi1-client.h"

/* -------------------------------------------------------------- */

typedef enum pmi_ver {
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
        if (shmemc_getenv(tp->env_var) != NULL) {
            goto out;
            /* NOT REACHED */
        }
        tp += 1;
    }

 out:
    pmi_version = tp->ver;
    pmi_verstr = tp->str;
}

/* -------------------------------------------------------------- */

typedef struct api_def {
    void (*init_fn)(void);
    void (*finalize_fn)(void);
    void (*heap_setup_fn)(void);
} api_def_t;

static api_def_t api;

/* -------------------------------------------------------------- */

void
shmemi_setup_heaps(void)
{
    api.heap_setup_fn();
}

/* -------------------------------------------------------------- */

void
shmemi_init(void)
{
    shmemu_init();

#ifdef ENABLE_DEBUG
    shmemi_logger_init();
#endif
    shmemi_malloc_init();

    select_pmi_version();

    switch (pmi_version) {
    case PMI_VERSION_1:
        api = (api_def_t) {
            .init_fn = shmemi_init_pmi1,
            .heap_setup_fn = shmemi_setup_heaps_pmi1,
            .finalize_fn = shmemi_finalize_pmi1
        };
        break;
    case PMI_VERSION_2:
        api = (api_def_t) {
            .init_fn = shmemi_init_pmi2,
            .heap_setup_fn = shmemi_setup_heaps_pmi2,
            .finalize_fn = shmemi_finalize_pmi2
        };
        break;
    case PMI_VERSION_X:
        api = (api_def_t) {
            .init_fn = shmemi_init_pmix,
            .heap_setup_fn = shmemi_setup_heaps_pmix,
            .finalize_fn = shmemi_finalize_pmix
        };
        break;
    default:
        logger(LOG_FATAL, "Unknown or missing PMI version");
        break;
    }

    api.init_fn();

    logger(LOG_INIT, "using PMI%s", pmi_verstr);
}

void
shmemi_finalize(void)
{
    logger(LOG_FINALIZE, "finalizing PMI%s", pmi_verstr);

    api.finalize_fn();

    shmemi_malloc_finalize();
#ifdef ENABLE_DEBUG
    shmemi_logger_finalize();
#endif

    shmemu_finalize();
}
