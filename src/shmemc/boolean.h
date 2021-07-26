/* For license: see LICENSE file at top-level */

#ifndef _SHMEMC_BOOLEAN_H
#define _SHMEMC_BOOLEAN_H 1

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#ifdef HAVE_STDBOOL_H

# include <stdbool.h>

#else

# define bool _Bool
# define true 1
# define false 0

#endif  /* HAVE_STDBOOL_H */

#endif /* ! _SHMEMC_BOOLEAN_H */
