/* For license: see LICENSE file at top-level */

#ifndef _SHMEM_OSH_INFO_H
#define _SHMEM_OSH_INFO_H 1

#include <stdio.h>

void info_output_spec_version(FILE *strm, const char *prefix);
void info_output_package_name(FILE *strm, const char *prefix);
void info_output_package_contact(FILE *strm, const char *prefix);
void info_output_package_version(FILE *strm, const char *prefix, int terse);
void info_output_build_env(FILE *strm, const char *prefix);
void info_output_features(FILE *strm, const char *prefix);
void info_output_comms(FILE *strm, const char *prefix);
void info_output_help(FILE *strm, const char *prefix);

#endif /* ! _SHMEM_OSH_INFO_H */
