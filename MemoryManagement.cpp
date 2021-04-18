//
// Created by hhj on 2021/4/8.
//

#include "MemoryManagement.h"


unsigned int MemoryManagement::ssize = 1000;

MemoryManagement::MemoryManagement(){
    start = (char *)malloc(1000);
    if(start)
        printf("Memory Allocated at: %x\n", start);
    else {
        printf("Not Enough Memory!\n");
        start = nullptr;
        current = nullptr;
    }
    current = new map(start, ssize);
    current->next = current;
    current->prior = current;
}


bool MemoryManagement::lmalloc(const unsigned int size) {
    if (current == nullptr) return false;

    if (current->next == current) {
        if (current->m_size >= size) {
            current->m_addr += size;
            current->m_size -= size;
            check(false);
            return true;
        } else {
            return false;
        }
    } else {
        if (current->m_size >= size) {
            current->m_addr += size;
            current->m_size -= size;
            check(false);
            return true;
        } else {
            map *temp = current;
            current = current->next;
            while (current != temp) {
                if (current->m_size >= size) {
                    current->m_addr += size;
                    current->m_size -= size;
                    check(false);
                    return true;
                }
                current = current->next;
            }
        }
        return false;
    }
}

void MemoryManagement::lfree(unsigned int addr, unsigned int size, bool show) {
    if (size <= 0) {
        printf("Exceed allowed space [%x:%x].\n", start+size, start);
        return;
    }
    map *tmp = new map(start+addr, size);
    if (current == nullptr) {
        current = tmp;
        current->next = current;
        current->prior = current;
    } else {
        map *head = current;
        while (head->m_addr < head->next->m_addr) head = head->next;
        head = head->next;
        map *temp;
        if (addr < head->m_addr-start) {
            if (start+addr+size > head->m_addr) {
                printf("Double free space [%x:%x].\n", head->m_addr, start+addr);
                return;
            }
            temp = head;
        } else if (addr >= head->prior->m_addr-start) {
            if (addr+size > ssize) {
                printf("Exceed allowed space [%x:%x].\n", start+ssize, start+addr+size);
                return;
            } else if (addr >= head->prior->m_addr-start && addr < head->prior->m_addr-start+head->prior->m_size) {
                printf("Double free space [%x:%x].\n", start+addr, (start+addr+size > head->prior->m_addr+head->prior->m_size)? head->prior->m_addr+head->prior->m_size:start+addr+size);
                return;
            }
            temp = head->prior;
        } else {
            temp = head;
            while (addr > temp->m_addr-start) temp = temp->next;
            if (start+addr < temp->prior->m_addr+temp->prior->m_size) {
                printf("Double free space [%x:%x].\n", start+addr, temp->prior->m_addr+temp->prior->m_size);
                return;
            } else if (addr+size > ssize) {
                printf("Exceed allowed space [%x:%x].\n", start+ssize, start+addr+size);
                return;
            } else if (start+addr+size >= temp->m_addr) {
                printf("Double free space [%x:%x].\n", temp->m_addr, start+addr+size);
                return;
            }
        }
        tmp->next = temp;
        tmp->prior = temp->prior;
        temp->prior = tmp;
        tmp->prior->next = tmp;
        check(true, tmp);
    }
    if (show) this->show(true);
}

void MemoryManagement::check(bool flag, map *loc) {
    if (flag == 0) {  // 解决分配内存产生０的问题
        if (current->m_size == 0) {
            if (current->next == current) {
                delete current;
                current = nullptr;
            } else {
                map *tmp = current;
                current = current->next;
                current->prior = tmp->prior;
                tmp->prior->next = current;
                delete tmp;
            }
        }
    }
    else {  // 解决回收内存的合并问题
        if (loc->next == loc) return;
        else {
            if (loc->m_addr-loc->prior->m_addr == loc->prior->m_size) {  // case1:向前合并
                loc->prior->m_size += loc->m_size;
                loc->prior->next = loc->next;
                loc->next->prior = loc->prior;
                map *tmp = loc->prior;
                delete loc;
                loc = tmp;
            }
            if (loc->next->m_addr-loc->m_addr == loc->m_size) {  // case2: 向后合并 && case3: 前后合并
                loc->m_size += loc->next->m_size;
                map *tmp = loc->next;
                if (current == tmp) current = loc;
                loc->next = loc->next->next;
                loc->next->prior = loc;
                delete tmp;
            }
        }
    }
}

void MemoryManagement::show(bool debug) const {
    if (current == nullptr) {
        printf("No memory available now!");
        return;
    }
    map *head = current;
    while (head->m_addr < head->next->m_addr) head = head->next;
    head = head->next;
    map *tmp = head;
    printf("  Available Memory  \n");
    printf("---------------------\n");
    while (tmp != head->prior) {
        if (debug)
            printf("|  m_addr: %8ld  |\n", tmp->m_addr-start);
        else
            printf("|  m_addr: %x  |\n", tmp->m_addr);
        printf("|  m_size: %8d  |\n", tmp->m_size);
        printf("---------------------\n");
        tmp = tmp->next;
    }
    if (debug)
        printf("|  m_addr: %8ld  |\n", tmp->m_addr-start);
    else
        printf("|  m_addr: %x  |\n", tmp->m_addr);
    printf("|  m_size: %8d  |\n", tmp->m_size);
    printf("---------------------\n\n");
}

void MemoryManagement::clear() {
    if (current != nullptr) {
        current->prior->next = nullptr;
        map *temp = current, *tmp;
        while (temp->next != nullptr) {
            tmp = temp;
            temp = temp->next;
            delete tmp;
        }
        delete temp;
    }

    current = new map(start, ssize);
    current->next = current;
    current->prior = current;
}

MemoryManagement::~MemoryManagement() {
    if (current != nullptr) {
        current->prior->next = nullptr;
        map *temp = current, *tmp;
        while (temp->next != nullptr) {
            tmp = temp;
            temp = temp->next;
            delete tmp;
        }
        delete temp;
        free(start);
    }
}

