/* For license: see LICENSE file at top-level */

#ifndef _MEMCHECK_H
#define _MEMCHECK_H 1

/*
 * This is pulled into internal-malloc.c:2519 to define our customized
 * handlers, which prevents implicit declaration warnings.
 */

extern void report_corruption(mspace m);
extern void report_usage_error(mspace m, void *p);

#endif /* ! _MEMCHECK_H */
