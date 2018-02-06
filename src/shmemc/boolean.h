/* For license: see LICENSE file at top-level */

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
