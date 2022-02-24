#include "kernel/types.h"
#include "user/user.h"

char *read_line() {
    static char buf[100];
    int buf_size = 0;
    char ch;

    while (read(0, &ch, 1) && (ch != '\n')) {
        buf[buf_size++] = ch;
    }
    buf[buf_size+1] = 0;

    if (buf_size == 0) return 0;
    return buf;
}

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("[!] Usage: ./xargs <command>\n");
        exit(1);
    }

    char *line;
    while ((line = read_line()) > 0) {
        argv[argc] = line;
        argv[argc+1] = 0;
        
        if (fork() == 0) {
            exec(argv[1], argv+1);
        }
    }

    while (wait(0) != -1);

    exit(0);
}
