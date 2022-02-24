#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char **argv) {
    int p1[2], p2[2];
    unsigned char byte;

    pipe(p1);
    pipe(p2);

    int pid = fork();
    if (pid == 0) {
        close(p1[1]);
        read(p1[0], &byte, sizeof(byte)); 
        close(p1[0]);
        printf("%d: received ping\n", getpid());

        close(p2[0]);
        write(p2[1], &byte, sizeof(byte));
        close(p2[1]);
    } else if (pid > 0) {
        close(p1[0]);
        write(p1[1], &byte, sizeof(byte));
        close(p1[1]);

        close(p2[1]);
        read(p2[0], &byte, sizeof(byte));
        close(p2[0]);
        printf("%d: received pong\n", getpid());
    } else {
        fprintf(2, "[!] fork failed.\n");
        exit(1);
    }

    exit(0);
}
