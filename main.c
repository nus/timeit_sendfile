#include <sys/sendfile.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    CMD_OPT_SENDFILE,
    CMD_OPT_USERCOPY,
    CMD_OPT_UNSPECIFIED,
} CMD_OPT;

static CMD_OPT parse_command_option(int argc, char *argv[]) {
    if (argc != 2) {
        return CMD_OPT_UNSPECIFIED;
    }

    if (!strcmp(argv[1], "-s")) {
        return CMD_OPT_SENDFILE;
    } else if (!strcmp(argv[1], "-u")) {
        return CMD_OPT_USERCOPY;
    } else {
        return CMD_OPT_UNSPECIFIED;
    }
}

static int userland_sendcopy(int fd_out, int fd_in) {
    int n = 0;
    const int size = 1024;
    char buf[size];

    while ((n = read(fd_in, buf, size)) > 0) {
        if (write(fd_out, buf, n) != n) {
            return -1;
        }
    }

    return 0;
}

int main(int argc, char *argv[]) {
    CMD_OPT opt;
    char path[] = "input.txt";
    struct stat st = {0};
    int fd_in = 0;
    int fd_out = 0;

    opt = parse_command_option(argc, argv);
    if (opt == CMD_OPT_UNSPECIFIED) {
        fprintf(stderr, "Specify '-s' or '-u'.\n");
        exit(EXIT_FAILURE);
    }

    if (stat(path, &st) == -1) {
        perror("stat() failed. errno");
        exit(EXIT_FAILURE);
    }

    fd_in = open(path, O_RDONLY);
    if (fd_in < 1) {
        perror("fd_in=open() failed. errno");
        exit(EXIT_FAILURE);
    }

    fd_out = open("out.txt", O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if (fd_out < 1) {
        perror("fd_out=open() failed. errno");
        exit(EXIT_FAILURE);
    }

    if (opt == CMD_OPT_SENDFILE) {
        if (sendfile(fd_out, fd_in , NULL, st.st_size) < 0) {
            perror("sendfile() failed. errno");
        }
    } else {
        if (userland_sendcopy(fd_out, fd_in) < 0) {
            perror("sendfile() failed(). errno");
        }
    }

    if (close(fd_out) < 0) {
        perror("close(fd_out) failed.");
    }

    if (close(fd_in) < 0) {
        perror("close(fd_in) failed.");
    }

    return 0;
}
