/* For license: see LICENSE file at top-level */

#ifndef _TABLE_H
#define _TABLE_H 1

/*
 * fn[32,64] will be NULL if "name" not found
 */

void barrier_all_lookup(const char *name, void **fn);
void sync_all_lookup(const char *name, void **fn);
void barrier_lookup(const char *name, void **fn);
void sync_lookup(const char *name, void **fn);
void broadcast_lookup(const char *name, void **fn32, void **fn64);
void alltoall_lookup(const char *name, void **fn32, void **fn64);
void alltoalls_lookup(const char *name, void **fn32, void **fn64);
void collect_lookup(const char *name, void **fn32, void **fn64);
void fcollect_lookup(const char *name, void **fn32, void **fn64);

#endif
