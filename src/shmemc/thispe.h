/* For license: see LICENSE file at top-level */

#ifndef _THISPE_H
#define _THISPE_H 1

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "boolean.h"
#include "threading.h"

#include "ucx/pe.h"

#include <sys/types.h>

/*
 * -- General --------------------------------------------------------
 */

typedef enum shmemc_status {
    SHMEMC_PE_SHUTDOWN = 0,
    SHMEMC_PE_RUNNING,
    SHMEMC_PE_FAILED,
    SHMEMC_PE_UNKNOWN
} shmemc_status_t;

/*
 * which collectives we're using.  might want to move this per-team at
 * some point so we can optimize through locality-awareness
 */
typedef enum shmemc_coll {
    SHMEMC_COLL_LINEAR = 0,
    SHMEMC_COLL_TREE,
    SHMEMC_COLL_DISSEM,
    SHMEMC_COLL_BINOMIAL,
    SHMEMC_COLL_BINOMIAL2,
    SHMEMC_COLL_UNKNOWN
} shmemc_coll_t;

#define SHMEMC_COLL_DEFAULT SHMEMC_COLL_TREE

typedef struct heapinfo {
    size_t nheaps;              /* how many heaps reuqested */
    size_t *heapsize;           /* array of their sizes */
} heapinfo_t;

/*
 * implementations support some environment variables
 */
typedef struct env_info {
    /*
     * required
     */
    bool print_version;         /* produce info output? */
    bool print_info;            /* show settings at program start? */
    heapinfo_t heaps;           /* the heaps we manage */
    bool debug;                 /* are we doing sanity debugging? */

    /*
     * this implementation
     */
    bool logging;            /* turn on message logging? */
    char *logging_file;      /* where does logging output go? */
    char *logging_events;    /* show only these types of messages */
    bool xpmem_kludge;       /* protect against UCX bug temporarily */
    shmemc_coll_t barrier_algo;   /* algorithms to use ... */
    shmemc_coll_t broadcast_algo; /* ... for these collectives */
} env_info_t;

/*
 * PEs can belong to teams
 */
typedef struct shmemc_team {
    size_t id;                  /* team ID# */
    int *members;               /* list of PEs in the team */
    size_t nmembers;            /* how many PEs */
} shmemc_team_t;

/*
 * each PE has this state info
 */
typedef struct thispe_info {
    comms_info_t comms;         /* per-comms layer info */
    env_info_t env;             /* environment vars */
    thread_desc_t td;           /* threading model invoked */
    int rank;                   /* physical rank info */
    int nranks;                 /* number of ranks requested */
    int maxranks;               /* universe size (e.g. for spares) */
    shmemc_status_t status;     /* up, down, out to lunch etc */
    int refcount;               /* library initialization count */
    int *peers;                 /* peer PEs in a node group */
    int npeers;                 /* how many peers */
    shmemc_team_t *teams;       /* PE teams we belong to */
    size_t nteams;              /* how many teams */
} thispe_info_t;

#endif /* ! _THISPE_H */
