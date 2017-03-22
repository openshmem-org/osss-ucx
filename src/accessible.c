/*
 * trivial stubs
 */

int
shmem_pe_accessible(int pe)
{
    return 1;
}

int
shmem_addr_accessible(const void *addr, int pe)
{
    return 0;
}
