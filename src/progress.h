/* For license: see LICENSE file at top-level */

/*
 * This code only gets activated if UCX's emulation mode is being
 * used: where we're running on legacy hardware, or where we don't
 * have direct transport-supported RDMA and AMO.
 *
 * Further, this is only enabled if asked for by the end-user.
 *
 * Hope in the future to fine-tune this if UCX can tell us whether its
 * enmulation mode is being used.
 */

#ifndef _SHMEM_PROGRESS_H
#define _SHMEM_PROGRESS_H 1

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

void progress_init(void);
void progress_finalize(void);

#endif  /* ! _SHMEM_PROGRESS_H 1 */
