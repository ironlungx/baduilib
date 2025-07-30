#pragma once

template<typename T>
struct Array {
    T *items;
    size_t len;
    size_t capacity;

    T operator[](size_t index) {
        if (index >= 0 && index < len) {
            return items[i];
        }

        return 0;
    }
};