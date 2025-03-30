//
// Created by 33640 on 30/03/2025.
//

#ifndef CXUTILS_H
#define CXUTILS_H

#include <array>

// File d'attente à taille fixe
template<typename T, size_t MaxSize>
class FixedSizeQueue {
    std::array<T, MaxSize> data;
    size_t head = 0;
    size_t tail = 0;
    size_t count = 0;
public:
    bool push(const T& value) {
        if (count == MaxSize) return false;
        data[tail] = value;
        tail = (tail + 1) % MaxSize;
        count++;
        return true;
    }

    bool pop(T& value) {
        if (count == 0) return false;
        value = data[head];
        head = (head + 1) % MaxSize;
        count--;
        return true;
    }
};

// Allocateur de pool avec temps d'allocation/désallocation constant
template<typename T, size_t PoolSize>
class PoolAllocator {
    union Slot {
        T value;
        Slot* next;
    };

    std::array<Slot, PoolSize> pool;
    Slot* free_list;

public:
    PoolAllocator() {
        // Initialiser la liste chaînée des emplacements libres
        for (size_t i = 0; i < PoolSize - 1; i++) {
            pool[i].next = &pool[i + 1];
        }
        pool[PoolSize - 1].next = nullptr;
        free_list = &pool[0];
    }

    T* allocate() {
        if (!free_list) return nullptr;

        Slot* slot = free_list;
        free_list = free_list->next;
        return &(slot->value);
    }

    void deallocate(T* ptr) {
        if (!ptr) return;

        Slot* slot = reinterpret_cast<Slot*>(ptr);
        slot->next = free_list;
        free_list = slot;
    }
};

#endif //CXUTILS_H
