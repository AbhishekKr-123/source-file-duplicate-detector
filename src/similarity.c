#include <stdlib.h>
#include <string.h>
#include "similarity.h"
#include "tokenizer.h"
#include "normalizer.h"
#include "file_reader.h"

int sim_lcs_length(struct Token *a, int na, struct Token *b, int nb)
{
    int *prev, *cur, i, j;
    if (na <= 0 || nb <= 0)
        return 0;
    /* iterate over shorter as columns to save memory */
    prev = (int *)calloc((size_t)nb + 1, sizeof(int));
    cur = (int *)calloc((size_t)nb + 1, sizeof(int));
    if (!prev || !cur)
    {
        free(prev);
        free(cur);
        return 0;
    }
    for (i = 1; i <= na; i++)
    {
        for (j = 1; j <= nb; j++)
        {
            if (strcmp(a[i - 1].value, b[j - 1].value) == 0)
                cur[j] = prev[j - 1] + 1;
            else
                cur[j] = prev[j] > cur[j - 1] ? prev[j] : cur[j - 1];
        }
        memcpy(prev, cur, ((size_t)nb + 1) * sizeof(int));
    }
    i = cur[nb];
    free(prev);
    free(cur);
    return i;
}

double sim_score_from_lcs(int lcs, int na, int nb)
{
    int m = na > nb ? na : nb;
    if (m == 0)
        return 100.0; /* both empty -> identical */
    return (double)lcs * 100.0 / (double)m;
}

double sim_files(const char *path1, const char *path2, int normalize_ids,
                 int *na_out, int *nb_out, int *lcs_out)
{
    char *c1, *c2, *nc1, *nc2;
    struct Token *t1 = NULL, *t2 = NULL;
    int na = 0, nb = 0, lcs = 0;
    double score = 0.0;
    c1 = fr_read_content(path1);
    c2 = fr_read_content(path2);
    if (!c1 || !c2)
    {
        free(c1);
        free(c2);
        return 0.0;
    }
    nc1 = norm_remove_comments(c1);
    nc2 = norm_remove_comments(c2);
    free(c1);
    free(c2);
    if (!nc1 || !nc2)
    {
        free(nc1);
        free(nc2);
        return 0.0;
    }
    na = tok_tokenize(nc1, &t1);
    nb = tok_tokenize(nc2, &t2);
    free(nc1);
    free(nc2);
    if (na < 0 || nb < 0)
    {
        tok_free(t1);
        tok_free(t2);
        return 0.0;
    }
    if (normalize_ids)
    {
        tok_normalize_identifiers(t1, na);
        tok_normalize_identifiers(t2, nb);
    }
    lcs = sim_lcs_length(t1, na, t2, nb);
    score = sim_score_from_lcs(lcs, na, nb);
    tok_free(t1);
    tok_free(t2);
    if (na_out)
        *na_out = na;
    if (nb_out)
        *nb_out = nb;
    if (lcs_out)
        *lcs_out = lcs;
    return score;
}

const char *sim_classify(double pct)
{
    if (pct >= 100.0)
        return "Exact Duplicate";
    if (pct >= 90.0)
        return "Highly Similar";
    if (pct >= 70.0)
        return "Similar";
    if (pct >= 50.0)
        return "Moderately Similar";
    return "Low Similarity";
}
