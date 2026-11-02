#ifndef REPORT_H
#define REPORT_H

#include "file_manager.h"

#define SIM_THRESHOLD 50.0

void rep_build_matrix(double mat[][MAX_FILES]);
void rep_print_matrix(double mat[][MAX_FILES]);
void rep_find_exact(void);
void rep_find_similar(double threshold, int normalize_ids);
void rep_generate(const char *outpath, int normalize_ids);
void rep_complete_analysis(void);

#endif
