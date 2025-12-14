#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#define MAX_FILES 32
#define MAX_FILENAME 16
#define MAX_FILE_SIZE 512

typedef struct {
    char name[MAX_FILENAME];
    int size;
    char data[MAX_FILE_SIZE];
} File;

typedef struct {
    File files[MAX_FILES];
    int file_count;
} FileSystem;

void fs_init(void);
File* fs_create_file(const char* name);
File* fs_get_file(const char* name);

// New functions for terminal access (freestanding)
int fs_get_file_count(void);
// Write data into a file (overwrite). Returns number of bytes written.
int fs_write_file(File* file, const char* data, int len);

File* fs_get_file_by_index(int idx);

#endif
