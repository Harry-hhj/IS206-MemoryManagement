//
// Created by hhj on 2021/4/8.
//

#ifndef MEMORYMANAGEMENT_MEMORYMANAGEMENT_H
#define MEMORYMANAGEMENT_MEMORYMANAGEMENT_H

#include <cstdio>
#include <malloc.h>


struct map{
    unsigned m_size;  // 空闲分区的起始大小
    char *m_addr;  // 空闲分区的起始地址
    map *next, *prior;  // 按地址排序后的后继节点和前驱节点

    explicit map(char *addr, unsigned int size): m_addr(addr), m_size(size), next(nullptr), prior(nullptr) {};  // 构造函数1
    explicit map(char *addr, unsigned int size, map *p, map *n): m_addr(addr), m_size(size), next(n), prior(p) {};  // 构造函数2
    ~map()= default;  // 默认析构函数
};


class MemoryManagement {
private:
    map *current;  // 循环双向链表
    char *start;  // 连续内存块
    static unsigned int ssize;  // 类静态变量，指定总内存大小

    void check(bool, map* = nullptr);  // 工具函数，确保双向链表规则正确

public:
    MemoryManagement();  // 构造函数
    bool lmalloc(unsigned);  // 内存分配，返回成功与否
    void lfree(unsigned int addr, unsigned int size, bool show);   // 内存释放
    void show(bool debug = false) const;  // 输出函数
    void clear();  // 清空
    ~MemoryManagement();  // 析构函数
};


#endif //MEMORYMANAGEMENT_MEMORYMANAGEMENT_H
