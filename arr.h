#ifndef ARR_H
#define ARR_H

#include <string>

struct Array {
    std::string* data;
    int capacity;
    int size;
    
    // Конструкторы
    Array();
    Array(int initialCapacity);
    
    // Деструктор
    ~Array();
    
    // Конструктор копирования
    Array(const Array& other);
    
    // Оператор присваивания
    Array& operator=(const Array& other);
};

// Базовые функции массива
void ACREATE(Array& arr, int initialCapacity = 0);
void AAPPEND(Array& arr, std::string value);
std::string AGET(const Array& arr, int index);
void ASET(Array& arr, int index, std::string value);
int ALENGTH(const Array& arr);
void APRINT(const Array& arr);

#endif
