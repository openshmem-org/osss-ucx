/* For license: see LICENSE file at top-level */

#ifndef _SHMEM_OSH_INFO_H
#define _SHMEM_OSH_INFO_H 1

typedef struct shmem_oshinfo {
    void (*spec_version)(void);
    void (*package_name)(void);
    void (*package_contact)(void);
    void (*package_version)(int);
    void (*buildenv)(void);
    void (*features)(void);
    void (*comms)(void);
    void (*help)(void);
} shmem_oshinfo_t;

extern shmem_oshinfo_t osh_info;

#endif /* ! _SHMEM_OSH_INFO_H */
