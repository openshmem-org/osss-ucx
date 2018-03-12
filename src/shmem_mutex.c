/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

/*
 * This only gets used if threading enabled
 */

#include "threading.h"

shmemc_mutex_t comms_mutex = SHMEMC_MUTEX_INITIALIZER;
