#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "file_manager.h"
#include "comparator.h"
#include "similarity.h"
#include "report.h"

static void strip_newline(char *s)
{
    s[strcspn(s, "\r\n")] = '\0';
}

static void menu(void)
{
    printf("+--------------------------------------+\n");
    printf("|     SOURCE FILE DUPLICATE DETECTOR   |\n");
    printf("+--------------------------------------+\n");
    printf("| 1. Add Source File                   |\n");
    printf("| 2. Remove Source File                |\n");
    printf("| 3. Display Source Files              |\n");
    printf("| 4. Compare Two Files                 |\n");
    printf("| 5. Find Exact Duplicates             |\n");
    printf("| 6. Find Similar Files                |\n");
    printf("| 7. Display Similarity Matrix         |\n");
    printf("| 8. Generate Report                   |\n");
    printf("| 9. Run Complete Analysis             |\n");
    printf("| 0. Exit                              |\n");
    printf("+--------------------------------------+\n");
    printf("Choice: ");
}

static void opt_add(void)
{
    char path[MAX_FPATH];
    int id;
    printf("Enter file path: ");
    if (!fgets(path, sizeof path, stdin))
        return;
    strip_newline(path);
    /* strip surrounding quotes from drag-drop */
    if (path[0] == '"' || path[0] == '\'')
    {
        size_t L = strlen(path);
        if (L >= 2 && path[L - 1] == path[0])
        {
            path[L - 1] = '\0';
            memmove(path, path + 1, L - 1);
        }
    }
    id = fm_add(path);
    if (id > 0)
        printf("Added with ID %d.\n", id);
}

static void opt_remove(void)
{
    int id;
    printf("Enter file ID to remove: ");
    if (scanf("%d", &id) != 1)
    {
        while (getchar() != '\n')
            ;
        return;
    }
    while (getchar() != '\n')
        ;
    if (fm_remove(id))
        printf("Removed.\n");
    else
        printf("ID not found.\n");
}

static void opt_compare_two(void)
{
    int a, b, na, nb, lcs;
    struct SourceFile *fa, *fb;
    double s;
    printf("Enter first file ID: ");
    if (scanf("%d", &a) != 1)
    {
        while (getchar() != '\n')
            ;
        return;
    }
    printf("Enter second file ID: ");
    if (scanf("%d", &b) != 1)
    {
        while (getchar() != '\n')
            ;
        return;
    }
    while (getchar() != '\n')
        ;
    fa = fm_find_by_id(a);
    fb = fm_find_by_id(b);
    if (!fa || !fb)
    {
        printf("Invalid ID(s).\n");
        return;
    }
    printf("Exact Match: %s\n", cmp_exact(fa->filepath, fb->filepath) ? "YES" : "NO");
    printf("Normalized Equal: %s\n", cmp_normalized_equal(fa->filepath, fb->filepath) ? "YES" : "NO");
    s = sim_files(fa->filepath, fb->filepath, 1, &na, &nb, &lcs);
    printf("Tokens: %d vs %d, LCS: %d\n", na, nb, lcs);
    printf("Similarity: %.2f%%  [%s]\n", s, sim_classify(s));
}

static void opt_similar(void)
{
    double th = 70.0;
    char buf[64];
    printf("Enter threshold [50-100, default 70]: ");
    if (fgets(buf, sizeof buf, stdin) && sscanf(buf, "%lf", &th) != 1)
        th = 70.0;
    rep_find_similar(th, 1);
}

static void opt_matrix(void)
{
    static double m[MAX_FILES][MAX_FILES];
    rep_build_matrix(m);
    rep_print_matrix(m);
}

int main(void)
{
    int choice;
    char line[64];
    for (;;)
    {
        menu();
        if (!fgets(line, sizeof line, stdin))
            break;
        if (sscanf(line, "%d", &choice) != 1)
            continue;
        switch (choice)
        {
        case 1:
            opt_add();
            break;
        case 2:
            opt_remove();
            break;
        case 3:
            fm_display();
            break;
        case 4:
            opt_compare_two();
            break;
        case 5:
            rep_find_exact();
            break;
        case 6:
            opt_similar();
            break;
        case 7:
            opt_matrix();
            break;
        case 8:
            rep_generate("duplicate_report.txt", 1);
            break;
        case 9:
            rep_complete_analysis();
            break;
        case 0:
            printf("Exiting.\n");
            return 0;
        default:
            printf("Invalid choice.\n");
            break;
        }
        printf("\n");
    }
    return 0;
}
