#ifndef FORTRAN_COMMON_H
#define FORTRAN_COMMON_H 1

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#define FORTRANIFY(_sym) FC_FUNC_(_sym, _sym)

#define FORTRANIFY_VOID_VOID(F) \
  void FORTRANIFY(F) (void) { F(); }

#define FORTRANIFY_CACHE(Name)                  \
    void                                        \
    FORTRANIFY(Name)(void *target)              \
    {                                           \
        Name(target);                           \
    }

#endif /* ! FORTRAN_COMMON_H */
