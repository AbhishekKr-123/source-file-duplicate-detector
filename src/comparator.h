#ifndef COMPARATOR_H
#define COMPARATOR_H

int cmp_exact(const char *path1, const char *path2);
unsigned long cmp_hash_djb2_str(const char *s);
int cmp_hash_file(const char *path, unsigned long *out);
int cmp_normalized_equal(const char *path1, const char *path2);

#endif
