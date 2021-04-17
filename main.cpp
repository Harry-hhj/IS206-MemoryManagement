#include <iostream>
#include <cstdio>
#include <string>
#include "MemoryManagement.h"

// haiyao mingling shuru de liangzhong chuli

enum class CMD : uint8_t {
        LMALLOC = 0,
        LFREE = 1,
        QUIT = 2,
    };

bool handle_input(CMD &cmd, unsigned int &size, unsigned int &addr) {
    while (true) {
        char c;
        c = getchar();
        while (c == ' ' || c == '\t' || c == '\n') c = getchar();
        if (c == 'm') {
            c = getchar();
            if (c != ' ') {
                while (c != ' ' && c != '\t' && c != '\n') c = getchar();
            }
            scanf("%u", &size);
            cmd = CMD::LMALLOC;
            return true;
        } else if (c == 'f') {
            c = getchar();
            if (c != ' ') {
                while (c != ' ' && c != '\t' && c != '\n') c = getchar();
            }
            scanf("%u %u", &size, &addr);
            cmd = CMD::LFREE;
            return true;
        } else if (c == 'q') {
            cmd = CMD::QUIT;
            return false;
        } else if (c == 'h') {
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

void handle_file_input(std::string &filename) {
    ;
}

int main() {
//    CMD cmd;
//    unsigned int addr, size;
//    handle_input(cmd, size, addr);
//    std::cout << "Hello, World!" << std::endl;
//    MemoryManagement manager;
//    manager.show(true);
//    manager.lmalloc(100);
//    manager.show(true);
//    manager.lmalloc(500);
//    manager.show(true);  // 600-1000*
//    manager.lfree(300, 100);  // 300-400 600-1000*
//    manager.lfree(400,50);  // 300-450 600-1000*
//    manager.lfree(550,50);  // 300-450 550-1000*
//    manager.show(true);
////    std::cin.get();
//    manager.lmalloc(200);  // 300-450 750-1000*
//    manager.lmalloc(100); // 300-450 850-1000*
//    manager.show(true);
////    std::cin.get();
//    manager.lfree(0,100);  // 0-100 300-450 850-1000*
//    manager.show(true);
////    std::cin.get();
//    /*
//       Available Memory
//    ---------------------
//    |  m_addr:        0  |
//    |  m_size:      100  |
//    ---------------------
//    |  m_addr:      300  |
//    |  m_size:      150  |
//    ---------------------
//    |  m_addr:      900  |
//    |  m_size:      100  |
//    ---------------------
//     */
////    manager.lmalloc(200); // 0-100 300-450* 850-1000; ===false===
////    manager.lmalloc(100); // 0-100 300-450* 950-1000*
//    manager.lmalloc(150);
//    manager.show(true);
//    std::cout << manager.lmalloc(150) << std::endl;
//    manager.lfree(500, 100);
//    manager.lfree(600, 150);
//    manager.show(true);
//    manager.lfree(999, 1);
//    manager.show(true);

    CMD cmd;
    unsigned int addr, size;
    MemoryManagement manager;

    while (handle_input(cmd, size, addr)) {
        if (cmd == CMD::LMALLOC) {
            if (manager.lmalloc(size)) printf("Your command is: malloc %u\n", size);
            else printf("Memory Allocated failed!\n");
        }
        else if (cmd == CMD::LFREE) {
            printf("Your command is: free %u %u\n", addr, size);
            manager.lfree(addr, size);
        }
        manager.show(true);
    }

    return 0;
}
