#include "filesystem.h"
#include "../drivers/screen.h"

static FileSystem fs;

void fs_init(void) {
    fs.file_count = 0;
    for (int i = 0; i < MAX_FILES; i++) {
        fs.files[i].name[0] = '\0';
        fs.files[i].size = 0;
    }
}

File* fs_create_file(const char* name) {
    if (fs.file_count >= MAX_FILES) return 0;
    File* file = &fs.files[fs.file_count++];
    int i = 0;
    while (name[i] && i < MAX_FILENAME - 1) {
        file->name[i] = name[i];
        i++;
    }
    file->name[i] = '\0';
    file->size = 0;
    return file;
}

File* fs_get_file(const char* name) {
    for (int i = 0; i < fs.file_count; i++) {
        int j = 0;
        while (name[j] && fs.files[i].name[j] && name[j] == fs.files[i].name[j]) j++;
        if (name[j] == '\0' && fs.files[i].name[j] == '\0') return &fs.files[i];
    }
    return 0;
}

int fs_write_file(File* file, const char* data, int len) {
    if (!file) return -1;
    if (len > MAX_FILE_SIZE) len = MAX_FILE_SIZE;
    for (int i = 0; i < len; i++) {
        file->data[i] = data[i];
    }
    file->size = len;
    return len;
}


// ---------------------------
// Terminal-accessible wrappers
// ---------------------------
int fs_get_file_count(void) {
    return fs.file_count;
}

File* fs_get_file_by_index(int idx) {
    if (idx < 0 || idx >= fs.file_count) return 0;
    return &fs.files[idx];
}
