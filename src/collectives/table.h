/* For license: see LICENSE file at top-level */

#ifndef _TABLE_H
#define _TABLE_H 1

#define COLL_NAME_MAX 64         /* longer than any name */

/*
 * most collectives are either just an op, or have 32/64 bit
 * components
 */

typedef void (*coll_fn_t)();

typedef struct sized_op {
    const char op[COLL_NAME_MAX];
    coll_fn_t f32;
    coll_fn_t f64;
} sized_op_t;

typedef struct unsized_op {
    const char op[COLL_NAME_MAX];
    coll_fn_t f;
} unsized_op_t;

/*
 * there are various untyped reduction kinds
 */

typedef struct coll_ops {
    sized_op_t   alltoall;
    sized_op_t   alltoalls;
    sized_op_t   collect;
    sized_op_t   fcollect;
    sized_op_t   broadcast;
    unsized_op_t barrier;
    unsized_op_t barrier_all;
    unsized_op_t sync;
    unsized_op_t sync_all;
} coll_ops_t;

extern coll_ops_t colls;

/*
 * return zero if registered OK, non-zero otherwise
 */

int register_barrier_all(const char *name);
int register_sync_all(const char *name);
int register_barrier(const char *name);
int register_sync(const char *name);
int register_broadcast(const char *name);
int register_alltoall(const char *name);
int register_alltoalls(const char *name);
int register_collect(const char *name);
int register_fcollect(const char *name);

#endif
