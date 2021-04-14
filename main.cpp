#include <iostream>
#include "MemoryManagement.h"

int main() {
    std::cout << "Hello, World!" << std::endl;
    MemoryManagement manager;
//    manager.show(true);
    manager.lmalloc(100);
//    manager.show(true);
    manager.lmalloc(500);
//    manager.show(true);  // 600-1000*
    manager.lfree(300, 100);  // 300-400 600-1000*
    manager.lfree(400,50);  // 300-450 600-1000*
    manager.lfree(550,50);  // 300-450 550-1000*
//    manager.show(true);
//    std::cin.get();
    manager.lmalloc(200);  // 300-450 750-1000*
    manager.lmalloc(100); // 300-450 850-1000*
//    manager.show(true);
//    std::cin.get();
    manager.lfree(0,100);  // 0-100 300-450 850-1000*
//    manager.show(true);
//    std::cin.get();
    manager.lmalloc(200); // 0-100 300-450* 850-1000; ===false===
    manager.lmalloc(100); // 0-100 300-450* 950-1000*
    manager.lmalloc(150);
//    std::cout << manager.lmalloc(150) << std::endl;
    manager.show(true);
    return 0;
}
