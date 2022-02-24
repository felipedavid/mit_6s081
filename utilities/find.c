#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "kernel/stat.h"
#include "kernel/fs.h"
#include "user/user.h"

void find(const char *path, const char *pattern) {
    int fd;
    struct dirent de;
    struct stat st, st2;
    char *file_name;
    static char file_path[100]; 

    if ((fd = open(path, O_RDONLY)) < 0) {
        fprintf(2, "[!] Failed to open %d\n", path);
        exit(1);
    }

    if (fstat(fd, &st) < 0) {
        fprintf(2, "[!] Failed to stat %d\n", path);
        close(fd);
        exit(1);
    }

    strcpy(file_path, path);
    file_name = file_path + strlen(file_path);
    *file_name++ = '/';

    if (st.type != T_DIR) return;

    while (read(fd, &de, sizeof(de)) == sizeof(de)) {
        if (de.inum == 0) continue;

        strcpy(file_name, de.name);
        stat(file_path, &st2);

        if (st2.type == T_FILE && !strcmp(file_name, pattern)) {
            printf("%s\n", file_path);
        } else if (st2.type == T_DIR && strcmp(file_name, ".") 
                                     && strcmp(file_name, "..")) {
            find(file_path, pattern);
        }
    }
}

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(2, "[!] Usage: find <directory> <pattern>\n");
        exit(1);
    }

    const char *path = argv[1];
    const char *pattern = argv[2];
    find(path, pattern);

    exit(0);
}
