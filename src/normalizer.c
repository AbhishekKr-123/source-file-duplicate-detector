#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "normalizer.h"

/* State-based comment removal. Preserves string/char literals. */
char *norm_remove_comments(const char *src)
{
    size_t n, i = 0, j = 0;
    char *out;
    enum
    {
        NORMAL,
        IN_STRING,
        IN_CHAR,
        IN_LINE,
        IN_BLOCK
    } st = NORMAL;
    if (!src)
        return NULL;
    n = strlen(src);
    out = (char *)malloc(n + 1);
    if (!out)
        return NULL;
    while (i < n)
    {
        char c = src[i];
        char nx = (i + 1 < n) ? src[i + 1] : '\0';
        switch (st)
        {
        case NORMAL:
            if (c == '"')
            {
                st = IN_STRING;
                out[j++] = c;
                i++;
            }
            else if (c == '\'')
            {
                st = IN_CHAR;
                out[j++] = c;
                i++;
            }
            else if (c == '/' && nx == '/')
            {
                st = IN_LINE;
                i += 2;
            }
            else if (c == '/' && nx == '*')
            {
                st = IN_BLOCK;
                i += 2;
            }
            else
            {
                out[j++] = c;
                i++;
            }
            break;
        case IN_STRING:
            out[j++] = c;
            i++;
            if (c == '\\' && i < n)
            {
                out[j++] = src[i++];
            }
            else if (c == '"')
                st = NORMAL;
            break;
        case IN_CHAR:
            out[j++] = c;
            i++;
            if (c == '\\' && i < n)
            {
                out[j++] = src[i++];
            }
            else if (c == '\'')
                st = NORMAL;
            break;
        case IN_LINE:
            if (c == '\n')
            {
                st = NORMAL;
                out[j++] = c;
            }
            i++;
            break;
        case IN_BLOCK:
            if (c == '*' && nx == '/')
            {
                st = NORMAL;
                i += 2;
            }
            else
                i++;
            break;
        }
    }
    out[j] = '\0';
    return out;
}

/* Remove all whitespace chars (space, tab, newline, CR). */
char *norm_remove_whitespace(const char *src)
{
    size_t n, i, j = 0;
    char *out;
    if (!src)
        return NULL;
    n = strlen(src);
    out = (char *)malloc(n + 1);
    if (!out)
        return NULL;
    for (i = 0; i < n; i++)
    {
        if (!isspace((unsigned char)src[i]))
            out[j++] = src[i];
    }
    out[j] = '\0';
    return out;
}

char *norm_normalize(const char *src)
{
    char *nc, *nw;
    if (!src)
        return NULL;
    nc = norm_remove_comments(src);
    if (!nc)
        return NULL;
    nw = norm_remove_whitespace(nc);
    free(nc);
    return nw;
}
