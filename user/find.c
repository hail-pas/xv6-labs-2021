#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char *fmtname(char *path) {
    static char buff[DIRSIZ + 1];
    char *p;

    for (p = path + strlen(path); p >= path && *p != '/'; p--)
        ;
    p++;

    if (strlen(p) >= DIRSIZ)
        return p;

    memmove(buff, p, strlen(p));
    // memset(buff + strlen(p), ' ', DIRSIZ - strlen(p));
    buff[strlen(p)] = '\0';
    return buff;
}

void find(char *path, char *content) {
    // list files
    char buff[512], *p;
    int fd;
    struct stat st;
    struct dirent de;

    if ((fd = open(path, 0)) < 0) {
        fprintf(2, "find: cannot open %s\n", path);
        return;
    }

    // 获取 path stat
    if (fstat(fd, &st) < 0) {
        fprintf(2, "find: cannot get stat of%s\n", path);
        return;
    }

    switch (st.type) {
    case T_FILE:
        // compare
        if (strcmp(fmtname(path), content)) {
            printf("%s\n", path);
        }
        close(fd);
        break;
    case T_DIR:
        // recursion
        if (strlen(path) + 1 + DIRSIZ + 1 > sizeof(buff)) {
            fprintf(2, "find: path too long");
            break;
        }

        strcpy(buff, path);
        p = buff + strlen(buff);
        if (*(p-1) != '/')
            *p++ = '/';

        while (read(fd, &de, sizeof(de)) == sizeof(de)) {
            if (de.inum == 0)
                continue;
            memmove(p, de.name, DIRSIZ);
            p[DIRSIZ] = 0;
            if (stat(buff, &st) < 0) {
                fprintf(2, "find: cannot get stat of%s\n", path);
                return;
            }

            if (st.type == T_FILE && strcmp(de.name, content) == 0) {
                printf("%s\n", buff);
                continue;
                ;
            }

            else if (st.type == T_DIR) {
                if ((strcmp(fmtname(buff), ".") == 0 ||
                     strcmp(fmtname(buff), "..") == 0))
                    continue;
                find(buff, content);
            }
        }
        close(fd);
    default:
        close(fd);
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(2, "find: 2 Parameters need\n");
        exit(1);
    }

    find(argv[1], argv[2]);

    exit(0);
}
