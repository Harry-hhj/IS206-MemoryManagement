//
// Created by hhj on 2021/4/8.
//

#ifndef MEMORYMANAGEMENT_MEMORYMANAGEMENT_H
#define MEMORYMANAGEMENT_MEMORYMANAGEMENT_H

#include <cstdio>
#include <malloc.h>


struct map{
    unsigned m_size;
    char *m_addr;
    map *next, *prior;

    explicit map(char *addr, unsigned int size): m_addr(addr), m_size(size), next(nullptr), prior(nullptr) {};
    explicit map(char *addr, unsigned int size, map *p, map *n): m_addr(addr), m_size(size), next(n), prior(p) {};
    ~map()= default;
};


class MemoryManagement {
private:
    map *current;  // 循环双向链表
    char *start;
    static unsigned int ssize;

    void check(bool, map* = nullptr);

public:
    MemoryManagement();
    bool lmalloc(unsigned);  // fenpei shibai shuchu
    void lfree(unsigned int addr, unsigned int size);   // ruguo chognfu shifang zenme ban
    void show(bool debug = false) const;
    void clear();
    ~MemoryManagement();
};


#endif //MEMORYMANAGEMENT_MEMORYMANAGEMENT_H
