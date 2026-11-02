#include <stdio.h>
#include <time.h>
#include "report.h"
#include "file_manager.h"
#include "comparator.h"
#include "similarity.h"

void rep_build_matrix(double mat[][MAX_FILES])
{
    int i, j;
    for (i = 0; i < g_fileCount; i++)
    {
        for (j = 0; j < g_fileCount; j++)
        {
            if (i == j)
                mat[i][j] = 100.0;
            else if (j < i)
                mat[i][j] = mat[j][i];
            else
            {
                if (cmp_exact(g_files[i].filepath, g_files[j].filepath))
                    mat[i][j] = 100.0;
                else
                    mat[i][j] = sim_files(g_files[i].filepath, g_files[j].filepath, 1, NULL, NULL, NULL);
            }
        }
    }
}

void rep_print_matrix(double mat[][MAX_FILES])
{
    int i, j;
    if (g_fileCount == 0)
    {
        printf("No files.\n");
        return;
    }
    printf("%-14s", "");
    for (j = 0; j < g_fileCount; j++)
        printf("F%-7d", g_files[j].id);
    printf("\n");
    for (i = 0; i < g_fileCount; i++)
    {
        printf("F%-12d", g_files[i].id);
        for (j = 0; j < g_fileCount; j++)
            printf("%-8.1f", mat[i][j]);
        printf("  %s\n", g_files[i].filename);
    }
}

void rep_find_exact(void)
{
    int i, j, found = 0;
    if (g_fileCount < 2)
    {
        printf("Need at least 2 files.\n");
        return;
    }
    printf("EXACT DUPLICATES\n----------------------------------------\n");
    for (i = 0; i < g_fileCount; i++)
    {
        for (j = i + 1; j < g_fileCount; j++)
        {
            unsigned long h1 = 0, h2 = 0;
            cmp_hash_file(g_files[i].filepath, &h1);
            cmp_hash_file(g_files[j].filepath, &h2);
            if (h1 != h2)
                continue; /* fast reject */
            if (cmp_exact(g_files[i].filepath, g_files[j].filepath))
            {
                printf("%s <-> %s  Similarity: 100.00%%\n",
                       g_files[i].filename, g_files[j].filename);
                found = 1;
            }
        }
    }
    if (!found)
        printf("(none)\n");
}

void rep_find_similar(double threshold, int normalize_ids)
{
    int i, j, found = 0;
    if (g_fileCount < 2)
    {
        printf("Need at least 2 files.\n");
        return;
    }
    printf("FILES WITH SIMILARITY >= %.2f%%\n----------------------------------------\n", threshold);
    for (i = 0; i < g_fileCount; i++)
    {
        for (j = i + 1; j < g_fileCount; j++)
        {
            double s;
            if (cmp_exact(g_files[i].filepath, g_files[j].filepath))
                s = 100.0;
            else
                s = sim_files(g_files[i].filepath, g_files[j].filepath, normalize_ids, NULL, NULL, NULL);
            if (s >= threshold)
            {
                printf("%s <-> %s  %.2f%%  [%s]\n", g_files[i].filename,
                       g_files[j].filename, s, sim_classify(s));
                found = 1;
            }
        }
    }
    if (!found)
        printf("(none)\n");
}

void rep_generate(const char *outpath, int normalize_ids)
{
    FILE *fp;
    int i, j;
    time_t t = time(NULL);
    fp = fopen(outpath, "w");
    if (!fp)
    {
        printf("Cannot write %s\n", outpath);
        return;
    }
    fprintf(fp, "========================================\n");
    fprintf(fp, "SOURCE FILE DUPLICATE DETECTOR\n");
    fprintf(fp, "========================================\n");
    fprintf(fp, "Date: %s", ctime(&t));
    fprintf(fp, "Files analyzed: %d\n\n", g_fileCount);
    for (i = 0; i < g_fileCount; i++)
    {
        fprintf(fp, "%d: %s (%ld bytes, %d lines)\n", g_files[i].id,
                g_files[i].filename, g_files[i].fileSize, g_files[i].lineCount);
    }
    fprintf(fp, "\nEXACT DUPLICATES\n----------------------------------------\n");
    for (i = 0; i < g_fileCount; i++)
        for (j = i + 1; j < g_fileCount; j++)
            if (cmp_exact(g_files[i].filepath, g_files[j].filepath))
                fprintf(fp, "%s <-> %s  Similarity: 100.00%%\n",
                        g_files[i].filename, g_files[j].filename);
    fprintf(fp, "\nHIGHLY SIMILAR FILES (>=90%%)\n----------------------------------------\n");
    for (i = 0; i < g_fileCount; i++)
        for (j = i + 1; j < g_fileCount; j++)
        {
            double s = cmp_exact(g_files[i].filepath, g_files[j].filepath) ? 100.0 : sim_files(g_files[i].filepath, g_files[j].filepath, normalize_ids, NULL, NULL, NULL);
            if (s >= 90.0 && s < 100.0)
                fprintf(fp, "%s <-> %s  Similarity: %.2f%%\n",
                        g_files[i].filename, g_files[j].filename, s);
        }
    fprintf(fp, "\nSIMILAR FILES (70-90%%)\n----------------------------------------\n");
    for (i = 0; i < g_fileCount; i++)
        for (j = i + 1; j < g_fileCount; j++)
        {
            double s = cmp_exact(g_files[i].filepath, g_files[j].filepath) ? 100.0 : sim_files(g_files[i].filepath, g_files[j].filepath, normalize_ids, NULL, NULL, NULL);
            if (s >= 70.0 && s < 90.0)
                fprintf(fp, "%s <-> %s  Similarity: %.2f%%\n",
                        g_files[i].filename, g_files[j].filename, s);
        }
    fprintf(fp, "\nMODERATELY SIMILAR FILES (50-70%%)\n----------------------------------------\n");
    for (i = 0; i < g_fileCount; i++)
        for (j = i + 1; j < g_fileCount; j++)
        {
            double s = cmp_exact(g_files[i].filepath, g_files[j].filepath) ? 100.0 : sim_files(g_files[i].filepath, g_files[j].filepath, normalize_ids, NULL, NULL, NULL);
            if (s >= 50.0 && s < 70.0)
                fprintf(fp, "%s <-> %s  Similarity: %.2f%%\n",
                        g_files[i].filename, g_files[j].filename, s);
        }
    fprintf(fp, "\nLOW SIMILARITY (<50%%)\n----------------------------------------\n");
    for (i = 0; i < g_fileCount; i++)
        for (j = i + 1; j < g_fileCount; j++)
        {
            double s = cmp_exact(g_files[i].filepath, g_files[j].filepath) ? 100.0 : sim_files(g_files[i].filepath, g_files[j].filepath, normalize_ids, NULL, NULL, NULL);
            if (s < 50.0)
                fprintf(fp, "%s <-> %s  Similarity: %.2f%%\n",
                        g_files[i].filename, g_files[j].filename, s);
        }
    fprintf(fp, "\nAnalysis completed successfully.\n");
    fclose(fp);
    printf("Report written to %s\n", outpath);
}

void rep_complete_analysis(void)
{
    int i, j;
    double m[MAX_FILES][MAX_FILES];
    if (g_fileCount < 2)
    {
        printf("Add at least 2 files first.\n");
        return;
    }
    printf("Reading files...\nCalculating file statistics...\n");
    printf("Checking exact duplicates...\n");
    rep_find_exact();
    printf("\nNormalizing source code...\nRemoving comments...\n");
    printf("Tokenizing source code...\nCalculating similarity...\n");
    rep_build_matrix(m);
    printf("\n========================================\n");
    printf("           ANALYSIS RESULTS\n");
    printf("========================================\n");
    for (i = 0; i < g_fileCount; i++)
        for (j = i + 1; j < g_fileCount; j++)
            printf("%s <-> %s : %.2f%% [%s]%s\n", g_files[i].filename,
                   g_files[j].filename, m[i][j], sim_classify(m[i][j]),
                   cmp_exact(g_files[i].filepath, g_files[j].filepath) ? " (exact)" : "");
    printf("========================================\nAnalysis Complete\n");
    rep_generate("duplicate_report.txt", 1);
}
