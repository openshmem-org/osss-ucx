/* For license: see LICENSE file at top-level */

#ifndef _SHMEM_DEPR_H
#define _SHMEM_DEPR_H 1

#ifdef _SHMEM_IN_SOURCE_TREE_

# define _DEPRECATED_BY(...)
# define _DEPRECATED


#else

#define _ADORNED_DEPRECATION(_text, _version)                           \
    __attribute__((deprecated("with specification " #_version           \
                              " and up, use `" #_text "' instead")))

/*
 * TODO: need better detection
 */
#if defined(__clang__)

# define _DEPRECATED_BY(_text, _version)        \
    _ADORNED_DEPRECATION(_text, _version)
# define _DEPRECATED                            \
    __attribute__((deprecated))

#elif defined(__OPEN64__)

/* not supported */

# define _DEPRECATED_BY(...)
# define _DEPRECATED

#elif defined(__GNUC__)

#define _DEPRECATED                             \
    __attribute__((deprecated))

/* GCC has extended attribute syntax from 4.5 onward */

# if (__GNUC__ >= 5) || (__GNUC__ == 4 && __GNUC_MINOR__ >= 5)
#  define _DEPRECATED_BY(_text, _version)       \
    _ADORNED_DEPRECATION(_text, _version)
# else
#  define _DEPRECATED_BY(...)                   \
    _DEPRECATED
# endif

#else

/* fallback */

# define _DEPRECATED_BY(...)
# define _DEPRECATED

#endif  /* compiler deprecation check */


#endif /* _SHMEM_IN_SOURCE_TREE_ */

#endif  /* ! _SHMEM_DEPR_H */
