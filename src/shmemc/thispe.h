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

/*
 * Collectives.  Hold these as strings so we can delegate
 * understanding them to the upper collectives shim.
 */

typedef struct collectives {
    char *barrier;
    char *barrier_all;
    char *sync;
    char *sync_all;
    char *broadcast;
    char *collect;
    char *fcollect;
    char *alltoall;
    char *alltoalls;
    char *reductions;
} shmemc_coll_t;

/*
 * PE status
 */

typedef enum shmemc_status {
    SHMEMC_PE_SHUTDOWN = 0,
    SHMEMC_PE_RUNNING,
    SHMEMC_PE_FAILED,
    SHMEMC_PE_UNKNOWN
} shmemc_status_t;

/*
 * Describe a heap
 */

typedef struct heapinfo {
    size_t nheaps;              /**< how many heaps requested */
    size_t *heapsize;           /**< array of their sizes */
} heapinfo_t;

/*
 * implementations support some environment variables
 */
typedef struct env_info {
    /*
     * required
     */
    bool print_version;         /**< produce info output? */
    bool print_info;            /**< show settings at program start? */
    bool debug;                 /**< are we doing sanity debugging? */

    /*
     * this implementation
     */
    bool logging;          /**< turn on message logging? */
    char *logging_file;    /**< where does logging output go? */
    char *logging_events;  /**< show only these types of messages */
    bool teardown_kludge;  /**< protect against UCX bug temporarily */

    shmemc_coll_t coll;         /**< collectives */

    char *progress_threads;     /**< do we need to start our own? */
    size_t progress_delay_ns;     /**< if progress needed, time (ns)
                                     between polls */

    size_t prealloc_contexts;   /**< set up this many at start */
    bool memfatal;              /**< force exit on memory usage error? */
} env_info_t;

/*
 * these are physical values from launch environment.  they get
 * pulled apart during team creation
 */
typedef struct pmi_info {
    int rank;                   /**< per-PE physical rank info */
    int nranks;                 /**< number of ranks requested */
    int maxranks;               /**< universe size (e.g. for spares) */
    int nnodes;                 /**< number of nodes allocated */
    int *peers;                 /**< peer PEs in a node group */
    int npeers;                 /**< how many peers? */
} pmi_info_t;

/*
 * each PE has this state info
 */
typedef struct thispe_info {
    comms_info_t comms;         /**< per-comms layer info */
    env_info_t env;             /**< environment vars */
    heapinfo_t heaps;           /**< the heaps we manage */
    thread_desc_t td;           /**< threading model invoked */
    pmi_info_t li;              /**< launcher-supplied info */
    shmemc_status_t status;     /**< up, down, out to lunch etc */
    int refcount;               /**< library initialization count */
    bool leader;                /**< is this PE a node leader? */
    bool progress_thread;       /**< PE requests progress thread */
    char *nodename;             /**< node we're running on */
    shmemc_team_t *teams;       /**< PE teams we belong to */
    size_t nteams;              /**< how many teams? */
} thispe_info_t;

#endif /* ! _THISPE_H */
