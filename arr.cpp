#include "arr.h"
#include <iostream>
#include <stdexcept>

Array::Array() : data(nullptr), capacity(0), size(0) {}

Array::Array(int initialCapacity) {
    if (initialCapacity > 0) {
        data = new std::string[initialCapacity];
        capacity = initialCapacity;
    } else {
        data = nullptr;
        capacity = 0;
    }
    size = 0;
}

Array::~Array() {
    delete[] data;
}

Array::Array(const Array& other) {
    capacity = other.capacity;
    size = other.size;
    if (capacity > 0) {
        data = new std::string[capacity];
        for (int i = 0; i < size; i++) {
            data[i] = other.data[i];
        }
    } else {
        data = nullptr;
    }
}

Array& Array::operator=(const Array& other) {
    if (this != &other) {
        delete[] data;
        capacity = other.capacity;
        size = other.size;
        if (capacity > 0) {
            data = new std::string[capacity];
            for (int i = 0; i < size; i++) {
                data[i] = other.data[i];
            }
        } else {
            data = nullptr;
        }
    }
    return *this;
}

void ACREATE(Array& arr, int initialCapacity) {
    if (arr.data != nullptr) {
        delete[] arr.data;
    }
    
    if (initialCapacity > 0) {
        arr.data = new std::string[initialCapacity];
        arr.capacity = initialCapacity;
    } else {
        arr.data = nullptr;
        arr.capacity = 0;
    }
    arr.size = 0;
}

void ARESIZE(Array& arr, int newCapacity) {
    if (newCapacity < arr.size) {
        throw std::invalid_argument("New capacity cannot be less than current size");
    }

    if (newCapacity == arr.capacity) {
        return;
    }

    if (newCapacity == 0) {
        delete[] arr.data;
        arr.data = nullptr;
        arr.capacity = 0;
        arr.size = 0;
        return;
    }

    std::string* newData = new std::string[newCapacity];
    for (int i = 0; i < arr.size; i++) {
        newData[i] = arr.data[i];
    }

    delete[] arr.data;
    arr.data = newData;
    arr.capacity = newCapacity;
}

void AAPPEND(Array& arr, std::string value) {
    if (arr.size >= arr.capacity) {
        int newCapacity = (arr.capacity == 0) ? 4 : arr.capacity * 2;
        ARESIZE(arr, newCapacity);
    }
    arr.data[arr.size] = value;
    arr.size++;
}

std::string AGET(const Array& arr, int index) {
    if (index < 0 || index >= arr.size) {
        throw std::out_of_range("Index out of range");
    }
    return arr.data[index];
}

void ASET(Array& arr, int index, std::string value) {
    if (index < 0 || index >= arr.size) {
        throw std::out_of_range("Index out of range");
    }
    arr.data[index] = value;
}

int ALENGTH(const Array& arr) {
    return arr.size;
}

void APRINT(const Array& arr) {
    std::cout << "Массив [" << arr.size << " элементов]: ";
    for (int i = 0; i < arr.size; i++) {
        std::cout << arr.data[i];
        if (i < arr.size - 1) {
            std::cout << ", ";
        }
    }
    std::cout << std::endl;
}
