#ifndef NORMALIZER_H
#define NORMALIZER_H

char *norm_remove_comments(const char *src);
char *norm_remove_whitespace(const char *src);
char *norm_normalize(const char *src);

#endif
