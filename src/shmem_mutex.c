/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#ifdef ENABLE_THREADS

#include <pthread.h>

pthread_mutex_t comms_mutex = PTHREAD_MUTEX_INITIALIZER;

#endif  /* ENABLE_THREADS */
