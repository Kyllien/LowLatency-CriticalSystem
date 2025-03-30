//
// Created by 33640 on 30/03/2025.
//

#ifndef CXUTILS_H
#define CXUTILS_H

#include <array>
#include <optional>
#include <stdexcept>

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

template<typename Enum, typename Value, size_t Size>
class ArrayMap {
private:
    // Table de mapping d'enum vers index
    using IndexType = size_t;
    std::array<std::optional<IndexType>, 16> enumToIndex; // Taille suffisante pour couvrir les valeurs

    // Stockage des valeurs
    std::array<std::pair<Enum, Value>, Size> data;
    size_t count = 0;

public:
    ArrayMap() {
        // Initialiser le mapping à "non présent"
        for (auto& entry : enumToIndex) {
            entry = std::nullopt;
        }
    }

    void insert(Enum key, Value value) {
        // Vérifier si la clé existe déjà
        auto enumIdx = static_cast<size_t>(key);
        if (enumIdx >= enumToIndex.size()) {
            throw std::out_of_range("Enum value out of bounds");
        }

        if (enumToIndex[enumIdx].has_value()) {
            // Mettre à jour la valeur existante
            data[enumToIndex[enumIdx].value()].second = std::move(value);
        } else {
            // Ajouter une nouvelle entrée
            if (count >= Size) {
                throw std::out_of_range("Map is full");
            }

            enumToIndex[enumIdx] = count;
            data[count] = std::make_pair(key, std::move(value));
            count++;
        }
    }

    Value* find(Enum key) {
        auto enumIdx = static_cast<size_t>(key);
        if (enumIdx < enumToIndex.size() && enumToIndex[enumIdx].has_value()) {
            return &data[enumToIndex[enumIdx].value()].second;
        }
        return nullptr;
    }

    const Value* find(Enum key) const {
        auto enumIdx = static_cast<size_t>(key);
        if (enumIdx < enumToIndex.size() && enumToIndex[enumIdx].has_value()) {
            return &data[enumToIndex[enumIdx].value()].second;
        }
        return nullptr;
    }

    Value& operator[](Enum key) {
        Value* found = find(key);
        if (found) {
            return *found;
        }

        // Insérer une nouvelle entrée
        insert(key, Value{});
        return *find(key);
    }

    // Itérateurs
    auto begin() { return data.begin(); }
    auto end() { return data.begin() + count; }
    auto begin() const { return data.begin(); }
    auto end() const { return data.begin() + count; }

    size_t size() const { return count; }
};

#endif //CXUTILS_H
