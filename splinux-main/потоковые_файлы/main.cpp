#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stack>
#include <iostream>

void file_typer_stack(char *fileName, int buff_size) {
    int fdesc;
    ssize_t wr_bytes;
    ssize_t rd_bytes;
    std::stack <std::string> buff;

    mode_t mode = S_IRUSR | S_IWUSR;
    int flags = O_WRONLY | O_CREAT | O_TRUNC;

    fdesc = open(fileName, flags, mode);
    if (fdesc < 0) {
        fprintf(stderr, "Can't open file!\n");
        exit(1);
    }

    while (true) {
        char str[buff_size];
        fgets(str, sizeof(str), stdin);

        if ((str[0]=='0') && (strlen(str)<3))
            break;
        else {
            buff.push(str);
        }
    }

    while(!buff.empty()) {
        std::string dest;
        dest = buff.top();
        rd_bytes = dest.length();
        wr_bytes = write(fdesc, dest.c_str(), rd_bytes);
        buff.pop();
    }
}

void file_typer(char *file_name, int buff_size) {
    int fdesc;
    ssize_t wr_bytes;
    ssize_t rd_bytes;
    char str[buff_size];

    mode_t mode = S_IRUSR | S_IWUSR;
    int flags = O_WRONLY | O_CREAT | O_TRUNC;

    fdesc = open(file_name, flags, mode);
    if (fdesc < 0) {
        fprintf(stderr, "Can't open file!\n");
        exit(1);
    }

    while (true) {
        fgets(str, sizeof(str), stdin);

        if ((str[0]=='0') && (strlen(str)<3))
            break;
        else {
            rd_bytes = strlen(str);
            wr_bytes = write(fdesc, str, rd_bytes);
        }
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Too few arguments!\n");
        exit(2);
    }
    char *file_name = argv[1];
    int buffSize;
    buffSize = atoi(argv[2]);
    file_typer_stack(file_name, buffSize);
}
