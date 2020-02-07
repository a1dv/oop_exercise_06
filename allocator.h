
#pragma once

#include <cstdlib>
#include <cstdint>

#include <exception>
#include <iostream>
#include <type_traits>

#include "list.h"

namespace myal {

    template<class T, size_t ALLOC_SIZE>
    struct my_allocator {
        using value_type = T;
        using size_type = std::size_t;
        using difference_type = std::ptrdiff_t;
        using is_always_equal = std::false_type;

        template<class U>
        struct rebind {
            using other = my_allocator<U, ALLOC_SIZE>;
        };

        my_allocator() :
                pool_begining(new char[ALLOC_SIZE]),
                pool_ending(pool_begining + ALLOC_SIZE),
                pool_tail(pool_begining) {}

        my_allocator(const my_allocator &) = delete;

        my_allocator(my_allocator &&) = delete;

        ~my_allocator() {
            delete[] pool_begining;
        }

        T *allocate(std::size_t n);

        void deallocate(T *ptr, std::size_t n);

    private:
        containers::list<char*> free_blocks;
        char *pool_begining;
        char *pool_ending;
        char *pool_tail;
    };

    template<class T, size_t ALLOC_SIZE>
    T *my_allocator<T, ALLOC_SIZE>::allocate(std::size_t n) {
        if (n != 1) {
            throw std::logic_error("Данный аллокатор не умеет работать с массивами");
        }
        if (size_t(pool_ending - pool_tail) < sizeof(T)) {
            if (!free_blocks.empty()) {
                auto it = free_blocks.begin();
                char *ptr = *it;
                free_blocks.delete_by_it(it);
                return reinterpret_cast<T *>(ptr);
            }
            throw std::bad_alloc();
        }

        T *result = reinterpret_cast<T *>(pool_tail);
        pool_tail += sizeof(T);
        return result;
    }

    template<class T, size_t ALLOC_SIZE>
    void my_allocator<T, ALLOC_SIZE>::deallocate(T *ptr, std::size_t n) {
        if (n != 1) {
            throw std::logic_error("Данный аллокатор не умеет работать с массивами");
        }
        if (ptr == nullptr) {
            return;
        }
        free_blocks.push_back(reinterpret_cast<char*> (ptr));
    }

}
