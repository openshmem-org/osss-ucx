/* For license: see LICENSE file at top-level */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif /* HAVE_CONFIG_H */

#include "shmemu.h"

#include <unistd.h>
#include <sys/file.h>
#include <sys/personality.h>

/*
 * if we claimed ASR isn't here, but it actually is, let's say
 * something
 *
 * Linux specific at moment
 *   (see https://wiki.freebsd.org/ASLR)
 */

#define RAND_VARIABLE "randomize_va_space"
#define RAND_FILE "/proc/sys/kernel/" RAND_VARIABLE

#define PERSONALITY_QUERY 0xffffffff

void
test_asr_mismatch(void)
{
    int p;
    int fd;
    ssize_t n;
    char inp;

    p = personality(PERSONALITY_QUERY);
    if (p & ADDR_NO_RANDOMIZE) {
        return;                 /* ASR disabled in this process */
        /* NOT REACHED */
    }

    fd = open(RAND_FILE, O_RDONLY, 0);
    if (fd < 0) {
        return;                 /* no file, carry on */
        /* NOT REACHED */
    }

    n = read(fd, &inp, 1);
    if (n < 1) {
        return;                 /* can't read file, carry on */
        /* NOT REACHED */
    }

    if (inp == '0') {
        return;                 /* file starts with "0", ASR turned off */
        /* NOT REACHED */
    }

    if (proc.leader) {
        shmemu_warn("aligned addresses requested, "
                    "but this node (%s) appears to have ASR enabled "
                    "(%s = %c)",
                    proc.nodename, RAND_VARIABLE, inp);
    }
}
