#include <stdio.h>
#include <stdlib.h>
#include "file_reader.h"

char *fr_read_content(const char *path) {
    FILE *fp;
    long size;
    char *buf;
    size_t n;
    fp = fopen(path, "rb");
    if (!fp) return NULL;
    fseek(fp, 0, SEEK_END);
    size = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    if (size < 0) size = 0;
    buf = (char *)malloc((size_t)size + 1);
    if (!buf) { fclose(fp); return NULL; }
    n = fread(buf, 1, (size_t)size, fp);
    buf[n] = '\0';
    fclose(fp);
    return buf;
}

long fr_file_size(const char *path) {
    FILE *fp;
    long size;
    fp = fopen(path, "rb");
    if (!fp) return -1;
    fseek(fp, 0, SEEK_END);
    size = ftell(fp);
    fclose(fp);
    return size;
}

int fr_get_stats(const char *path, long *size, int *lines, int *chars) {
    FILE *fp;
    int ch, lc = 0, cc = 0, last = '\n';
    long sz;
    fp = fopen(path, "rb");
    if (!fp) return 0;
    fseek(fp, 0, SEEK_END);
    sz = ftell(fp);
    fseek(fp, 0, SEEK_SET);
    while ((ch = fgetc(fp)) != EOF) {
        cc++;
        if (ch == '\n') lc++;
        last = ch;
    }
    fclose(fp);
    if (cc > 0 && last != '\n') lc++;
    if (cc == 0) lc = 0;
    if (size) *size = sz;
    if (lines) *lines = lc;
    if (chars) *chars = cc;
    return 1;
}
