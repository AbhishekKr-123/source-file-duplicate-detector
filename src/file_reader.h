#ifndef FILE_READER_H
#define FILE_READER_H

char *fr_read_content(const char *path);
long fr_file_size(const char *path);
int fr_get_stats(const char *path, long *size, int *lines, int *chars);

#endif
