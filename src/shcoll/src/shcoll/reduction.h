#ifndef _SHCOLL_REDUCTION_H
#define _SHCOLL_REDUCTION_H 1

#define SHCOLL_REDUCE_DECLARE(_name, _type, _algorithm)             \
    void shcoll_##_name##_to_all_##_algorithm(_type *dest,          \
                                              const _type *source,  \
                                              int nreduce,          \
                                              int PE_start,         \
                                              int logPE_stride,     \
                                              int PE_size,          \
                                              _type *pWrk,          \
                                              long *pSync)

#define SHCOLL_REDUCE_DECLARE_ALL(_algorithm)                           \
    /* AND operation */                                                 \
    SHCOLL_REDUCE_DECLARE(short_and,        short,      _algorithm);    \
    SHCOLL_REDUCE_DECLARE(int_and,          int,        _algorithm);    \
    SHCOLL_REDUCE_DECLARE(long_and,         long,       _algorithm);    \
    SHCOLL_REDUCE_DECLARE(longlong_and,     long long,  _algorithm);    \
                                                                        \
    /* MAX operation */                                                 \
    SHCOLL_REDUCE_DECLARE(short_max,        short,          _algorithm); \
    SHCOLL_REDUCE_DECLARE(int_max,          int,            _algorithm); \
    SHCOLL_REDUCE_DECLARE(double_max,       double,         _algorithm); \
    SHCOLL_REDUCE_DECLARE(float_max,        float,          _algorithm); \
    SHCOLL_REDUCE_DECLARE(long_max,         long,           _algorithm); \
    SHCOLL_REDUCE_DECLARE(longdouble_max,   long double,    _algorithm); \
    SHCOLL_REDUCE_DECLARE(longlong_max,     long long,      _algorithm); \
                                                                        \
    /* MIN operation */                                                 \
    SHCOLL_REDUCE_DECLARE(short_min,        short,          _algorithm); \
    SHCOLL_REDUCE_DECLARE(int_min,          int,            _algorithm); \
    SHCOLL_REDUCE_DECLARE(double_min,       double,         _algorithm); \
    SHCOLL_REDUCE_DECLARE(float_min,        float,          _algorithm); \
    SHCOLL_REDUCE_DECLARE(long_min,         long,           _algorithm); \
    SHCOLL_REDUCE_DECLARE(longdouble_min,   long double,    _algorithm); \
    SHCOLL_REDUCE_DECLARE(longlong_min,     long long,      _algorithm); \
                                                                        \
    /* SUM operation */                                                 \
    SHCOLL_REDUCE_DECLARE(complexd_sum,     double _Complex,    _algorithm); \
    SHCOLL_REDUCE_DECLARE(complexf_sum,     float _Complex,     _algorithm); \
    SHCOLL_REDUCE_DECLARE(short_sum,        short,              _algorithm); \
    SHCOLL_REDUCE_DECLARE(int_sum,          int,                _algorithm); \
    SHCOLL_REDUCE_DECLARE(double_sum,       double,             _algorithm); \
    SHCOLL_REDUCE_DECLARE(float_sum,        float,              _algorithm); \
    SHCOLL_REDUCE_DECLARE(long_sum,         long,               _algorithm); \
    SHCOLL_REDUCE_DECLARE(longdouble_sum,   long double,        _algorithm); \
    SHCOLL_REDUCE_DECLARE(longlong_sum,     long long,          _algorithm); \
                                                                        \
    /* PROD operation */                                                \
    SHCOLL_REDUCE_DECLARE(complexd_prod,    double _Complex,    _algorithm); \
    SHCOLL_REDUCE_DECLARE(complexf_prod,    float _Complex,     _algorithm); \
    SHCOLL_REDUCE_DECLARE(short_prod,       short,              _algorithm); \
    SHCOLL_REDUCE_DECLARE(int_prod,         int,                _algorithm); \
    SHCOLL_REDUCE_DECLARE(double_prod,      double,             _algorithm); \
    SHCOLL_REDUCE_DECLARE(float_prod,       float,              _algorithm); \
    SHCOLL_REDUCE_DECLARE(long_prod,        long,               _algorithm); \
    SHCOLL_REDUCE_DECLARE(longdouble_prod,  long double,        _algorithm); \
    SHCOLL_REDUCE_DECLARE(longlong_prod,    long long,          _algorithm); \
                                                                        \
    /* OR operation */                                                  \
    SHCOLL_REDUCE_DECLARE(short_or,         short,      _algorithm);    \
    SHCOLL_REDUCE_DECLARE(int_or,           int,        _algorithm);    \
    SHCOLL_REDUCE_DECLARE(long_or,          long,       _algorithm);    \
    SHCOLL_REDUCE_DECLARE(longlong_or,      long long,  _algorithm);    \
                                                                        \
    /* XOR operation */                                                 \
    SHCOLL_REDUCE_DECLARE(short_xor,        short,      _algorithm);    \
    SHCOLL_REDUCE_DECLARE(int_xor,          int,        _algorithm);    \
    SHCOLL_REDUCE_DECLARE(long_xor,         long,       _algorithm);    \
    SHCOLL_REDUCE_DECLARE(longlong_xor,     long long,  _algorithm);

SHCOLL_REDUCE_DECLARE_ALL(linear)
SHCOLL_REDUCE_DECLARE_ALL(binomial)
SHCOLL_REDUCE_DECLARE_ALL(rec_dbl)
SHCOLL_REDUCE_DECLARE_ALL(rabenseifner)
SHCOLL_REDUCE_DECLARE_ALL(rabenseifner2)

#endif /* ! _SHCOLL_REDUCTION_H */
