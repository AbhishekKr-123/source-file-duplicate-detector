#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

#define MAX_FILES 100
#define MAX_FNAME 100
#define MAX_FPATH 300

struct SourceFile {
    int id;
    char filename[MAX_FNAME];
    char filepath[MAX_FPATH];
    long fileSize;
    int lineCount;
    int charCount;
};

extern struct SourceFile g_files[MAX_FILES];
extern int g_fileCount;
extern int g_nextId;

int fm_add(const char *path);
int fm_remove(int id);
void fm_display(void);
struct SourceFile *fm_find_by_id(int id);
int fm_exists(const char *path);
int fm_validate_path(const char *path);

#endif
