#ifndef SIMILARITY_H
#define SIMILARITY_H

struct Token;

int sim_lcs_length(struct Token *a, int na, struct Token *b, int nb);
double sim_score_from_lcs(int lcs, int na, int nb);
double sim_files(const char *path1, const char *path2, int normalize_ids,
                 int *na_out, int *nb_out, int *lcs_out);
const char *sim_classify(double pct);

#endif
