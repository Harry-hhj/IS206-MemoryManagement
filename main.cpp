#include <iostream>
#include <cstdio>
#include <cstring>
#include "MemoryManagement.h"


// 命令枚举类型
enum class CMD : uint8_t {
    LMALLOC = 0,  // malloc size
    LFREE = 1,  // free size addr
    QUIT = 2,  // quit input
};

// 用于处理用户终端输入
bool handle_input(CMD &cmd, unsigned int &size, unsigned int &addr) {
    while (true) {
        char c;
        c = getchar();
        while (c == ' ' || c == '\t' || c == '\n') c = getchar();
        if (c == 'm') {  // malloc
            c = getchar();
            if (c != ' ') {
                while (c != ' ' && c != '\t' && c != '\n') c = getchar();
            }
            scanf("%u", &size);
            cmd = CMD::LMALLOC;
            return true;
        } else if (c == 'f') {  // free
            c = getchar();
            if (c != ' ') {
                while (c != ' ' && c != '\t' && c != '\n') c = getchar();
            }
            scanf("%u %u", &size, &addr);
            cmd = CMD::LFREE;
            return true;
        } else if (c == 'q') {  // quit
            cmd = CMD::QUIT;
            return false;
        } else if (c == 'h') {  // help for more information
            c = getchar();
            if (c != ' ') {
                while (c != ' ' && c != '\t' && c != '\n') c = getchar();
            }
            printf("Command Format:\n\nm(alloc) &size \nf(ree) &size &addr \nq(uit) -- exit \n");
        } else {
            printf("Invalid command, use help for more information.\n");
            fflush(stdin);
        }
    }
}

// 处理用户文件输入
void handle_file_input(const char *filename) {
    char c[1000];
    FILE *fptr;

    if ((fptr = fopen(filename, "r")) == nullptr) {
        printf("Error! opening file");
        // 文件指针返回 NULL 则退出
        exit(1);
    }

    CMD cmd;
    unsigned int addr = 0, size = 0, line = 0;
    MemoryManagement manager;
    while(fgets(c,1000,fptr) != nullptr) {  // 读取文本，直到碰到新的一行开始
        int len = strlen(c);
        c[len-1] = '\0';  /*去掉换行符*/
        printf("读取内容:\n%s\n", c);
        ++line;
        addr = 0;
        size = 0;
        char *p = c;
        while (*p == ' ' || *p == '\t') ++p;
        if (*p == 'm') {  // malloc
            cmd = CMD::LMALLOC;
            while (*p != ' ' && *p != '\t') {
                if (*p == '\0') {
                    printf("Command error at line %d.\nExpected parameter:[size].\n", line);
                    break;
                }
                ++p;
            }
            while (*p == ' ' || *p == '\t') {
                if (*p == '\0') {
                    printf("Command error at line %d.\nExpected parameter:[size].\n", line);
                    break;
                }
                ++p;
            }
            while (*p != ' ' && *p != '\t' && *p != '\0') {
                if (*p >= '0' && *p <= '9') {
                    size *= 10;
                    size += *p - '0';
                    ++p;
                } else {
                    printf("Command error at line %d.\nExpected unsigned int for parameter:[size].\n", line);
                    break;
                }
            }
        } else if (*p == 'f') {  // free
            cmd = CMD::LFREE;
            while (*p != ' ' && *p != '\t') {
                if (*p == '\0') {
                    printf("Command error at line %d.\nExpected parameter:[size].\n", line);
                    break;
                }
                ++p;
            }
            while (*p == ' ' || *p == '\t') {
                if (*p == '\0') {
                    printf("Command error at line %d.\nExpected parameter:[size].\n", line);
                    break;
                }
                ++p;
            }
            while (*p != ' ' && *p != '\t') {
                if (*p >= '0' && *p <= '9') {
                    size *= 10;
                    size += *p - '0';
                    ++p;
                } else if (*p == '\0') {
                    printf("Command error at line %d.\nExpected parameter:[addr].\n", line);
                    break;
                } else {
                    printf("Command error at line %d.\nExpected unsigned int for parameter:[size].\n", line);
                    break;
                }
            }
            while (*p == ' ' || *p == '\t') {
                if (*p == '\0') {
                    printf("Command error at line %d.\nExpected parameter:[addr].\n", line);
                    break;
                }
                ++p;
            }
            while (*p != ' ' && *p != '\t' && *p != '\0') {
                if (*p >= '0' && *p <= '9') {
                    addr *= 10;
                    addr += *p - '0';
                    ++p;
                } else {
                    printf("Command error at line %d.\nExpected unsigned int for parameter:[addr].\n", line);
                    break;
                }
            }
        } else {
            printf("Command error at line %d.\nUnrecognized parameter:[command].\n", line);
            continue;
        }
        printf("Command %hhu, size %d, addr %d\n", cmd, size, addr);
        if (cmd == CMD::LMALLOC) {
            if (manager.lmalloc(size)) {
                manager.show();
            } else {
                printf("Memory Allocated failed!\n");
            }
        } else {
            manager.lfree(addr, size, true);
        }
    }

    fclose(fptr);
}

int main() {
    int mode = -1;
    while (mode != 0 && mode != 1) {
        printf("Please choose input mode: 0 is keyboard while 1 is file.\n");
        char cmode;
        scanf("%c", &cmode);
        mode = cmode - '0';
    }
    if (mode == 0) {
        CMD cmd;
        unsigned int addr, size;
        MemoryManagement manager;

        while (handle_input(cmd, size, addr)) {
            if (cmd == CMD::LMALLOC) {
                if (manager.lmalloc(size)) printf("Your command is: malloc %u\n", size);
                else printf("Memory Allocated failed!\n");
                manager.show(true);
            } else if (cmd == CMD::LFREE) {
                printf("Your command is: free %u %u\n", addr, size);
                manager.lfree(addr, size, true);
            }
        }
    } else {
        char filename[100] = "../test.txt";
        printf("Please input you filename(e.g. ../test.txt):\n");
        scanf("%s", filename);
        handle_file_input(filename);
    }

    return 0;
}
