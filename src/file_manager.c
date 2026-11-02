#include <stdio.h>
#include <string.h>
#include "file_manager.h"
#include "file_reader.h"

struct SourceFile g_files[MAX_FILES];
int g_fileCount = 0;
int g_nextId = 1;

static void extract_filename(const char *path, char *out, int outsz) {
    const char *p1 = strrchr(path, '/');
    const char *p2 = strrchr(path, '\\');
    const char *p = p1;
    if (p2 && (!p || p2 > p)) p = p2;
    if (p) p++; else p = path;
    strncpy(out, p, (size_t)(outsz - 1));
    out[outsz - 1] = '\0';
}

int fm_validate_path(const char *path) {
    FILE *fp;
    if (!path || !*path) return 0;
    fp = fopen(path, "r");
    if (!fp) return 0;
    fclose(fp);
    return 1;
}

int fm_exists(const char *path) {
    int i;
    for (i = 0; i < g_fileCount; i++) {
        if (strcmp(g_files[i].filepath, path) == 0) return 1;
    }
    return 0;
}

int fm_add(const char *path) {
    long size = 0;
    int lines = 0, chars = 0;
    struct SourceFile *s;
    if (g_fileCount >= MAX_FILES) {
        printf("File list full (max %d).\n", MAX_FILES);
        return -1;
    }
    if (!fm_validate_path(path)) {
        printf("Invalid path or unreadable file: %s\n", path);
        return -1;
    }
    if (fm_exists(path)) {
        printf("File already added.\n");
        return -1;
    }
    if (!fr_get_stats(path, &size, &lines, &chars)) {
        printf("Unable to read file stats.\n");
        return -1;
    }
    s = &g_files[g_fileCount++];
    s->id = g_nextId++;
    extract_filename(path, s->filename, MAX_FNAME);
    strncpy(s->filepath, path, MAX_FPATH - 1);
    s->filepath[MAX_FPATH - 1] = '\0';
    s->fileSize = size;
    s->lineCount = lines;
    s->charCount = chars;
    return s->id;
}

int fm_remove(int id) {
    int i, idx = -1;
    for (i = 0; i < g_fileCount; i++) {
        if (g_files[i].id == id) { idx = i; break; }
    }
    if (idx < 0) return 0;
    for (i = idx; i < g_fileCount - 1; i++) g_files[i] = g_files[i + 1];
    g_fileCount--;
    return 1;
}

void fm_display(void) {
    int i;
    if (g_fileCount == 0) {
        printf("No files added.\n");
        return;
    }
    printf("%-4s %-25s %-8s %-6s %s\n", "ID", "Name", "Bytes", "Lines", "Path");
    printf("----------------------------------------------------------------------\n");
    for (i = 0; i < g_fileCount; i++) {
        printf("%-4d %-25.25s %-8ld %-6d %s\n",
               g_files[i].id, g_files[i].filename,
               g_files[i].fileSize, g_files[i].lineCount, g_files[i].filepath);
    }
}

struct SourceFile *fm_find_by_id(int id) {
    int i;
    for (i = 0; i < g_fileCount; i++) {
        if (g_files[i].id == id) return &g_files[i];
    }
    return NULL;
}
