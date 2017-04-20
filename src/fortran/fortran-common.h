#ifndef FORTRAN_COMMON_H
#define FORTRAN_COMMON_H 1

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#define FORTRANIFY(_sym) FC_FUNC_(_sym, _sym)

#define FORTRANIFY_VOID_VOID(_f)                \
    void FORTRANIFY(_f) (void) { _f(); }

#define FORTRANIFY_CACHE(_name)                 \
    void                                        \
    FORTRANIFY(_name)(void *target)             \
    {                                           \
        _name(target);                          \
    }

#endif /* ! FORTRAN_COMMON_H */
