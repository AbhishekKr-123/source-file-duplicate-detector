#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "comparator.h"
#include "file_reader.h"
#include "normalizer.h"

int cmp_exact(const char *path1, const char *path2)
{
    FILE *a, *b;
    long sa, sb;
    int ca, cb;
    sa = fr_file_size(path1);
    sb = fr_file_size(path2);
    if (sa < 0 || sb < 0)
        return 0;
    if (sa != sb)
        return 0;
    a = fopen(path1, "rb");
    b = fopen(path2, "rb");
    if (!a || !b)
    {
        if (a)
            fclose(a);
        if (b)
            fclose(b);
        return 0;
    }
    do
    {
        ca = fgetc(a);
        cb = fgetc(b);
        if (ca != cb)
        {
            fclose(a);
            fclose(b);
            return 0;
        }
    } while (ca != EOF);
    fclose(a);
    fclose(b);
    return 1;
}

unsigned long cmp_hash_djb2_str(const char *s)
{
    unsigned long h = 5381;
    while (*s)
    {
        h = ((h << 5) + h) + (unsigned char)(*s);
        s++;
    }
    return h;
}

int cmp_hash_file(const char *path, unsigned long *out)
{
    FILE *fp;
    unsigned long h = 5381;
    int ch;
    fp = fopen(path, "rb");
    if (!fp)
        return 0;
    while ((ch = fgetc(fp)) != EOF)
        h = ((h << 5) + h) + (unsigned char)ch;
    fclose(fp);
    if (out)
        *out = h;
    return 1;
}

int cmp_normalized_equal(const char *path1, const char *path2)
{
    char *c1, *c2, *n1, *n2;
    int eq;
    c1 = fr_read_content(path1);
    c2 = fr_read_content(path2);
    if (!c1 || !c2)
    {
        free(c1);
        free(c2);
        return 0;
    }
    n1 = norm_normalize(c1);
    n2 = norm_normalize(c2);
    free(c1);
    free(c2);
    if (!n1 || !n2)
    {
        free(n1);
        free(n2);
        return 0;
    }
    eq = (strcmp(n1, n2) == 0);
    free(n1);
    free(n2);
    return eq;
}
