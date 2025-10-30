#include "hash-t.h"
#include <iostream>
using namespace std;



HNode::HNode(const string& k, int d) : key(k), data(d), next(nullptr) {}

HTable* createTable(int capacity) {
    HTable* ht = new HTable;
    ht->capacity = capacity;
    ht->size = 0;
    ht->buckets.resize(capacity, nullptr);
    return ht;
}

void deleteTable(HTable* ht) {
    for (int i = 0; i < ht->capacity; i++) {
        HNode* curr = ht->buckets[i];
        while (curr != nullptr) {
            HNode* temp = curr;
            curr = curr->next;
            delete temp;
        }
    }
    delete ht;
}

int hashFunction(const string& key, int capacity) {
    int hash = 0;
    for (char c : key) {
        hash = (hash * 31 + c) % capacity;
    }
    return hash;
}

bool contains(HTable* ht, const string& key) {
    int index = hashFunction(key, ht->capacity);
    HNode* curr = ht->buckets[index];
    
    while (curr != nullptr) {
        if (curr->key == key) {
            return true;
        }
        curr = curr->next;
    }
    return false;
}

bool insert(HTable* ht, const string& key, int value) {
    if (contains(ht, key)) return false;
    int index = hashFunction(key, ht->capacity);
    HNode* newHNode = new HNode(key, value);
    
    newHNode->next = ht->buckets[index];
    ht->buckets[index] = newHNode;
    ht->size++;
    
    return true;
}

bool remove(HTable* ht, const string& key) {
    int index = hashFunction(key, ht->capacity);
    HNode* curr = ht->buckets[index];
    HNode* prev = nullptr;
    
    while (curr != nullptr) {
        if (curr->key == key) {
            if (prev == nullptr) {
                ht->buckets[index] = curr->next;
            } else {
                prev->next = curr->next;
            }
            
            delete curr;
            ht->size--;
            return true;
        }
        prev = curr;
        curr = curr->next;
    }
    return false;
}

void printTable(HTable* ht) {
    cout << "Hash Table (Chaining) - size: " << ht->size << endl;
    for (int i = 0; i < ht->capacity; i++) {
        cout << "Bucket " << i << ": ";
        HNode* curr = ht->buckets[i];
        while (curr != nullptr) {
            cout << curr->key << "(" << curr->data << ") -> ";
            curr = curr->next;
        }
        cout << "nullptr" << endl;
    }
}

int getSize(HTable* ht) {
    return ht->size;
}

//открытая адрессация 
OATable* createOATable(int capacity) {
    OATable* ht = new OATable;
    ht->capacity = capacity;
    ht->size = 0;
    ht->cells.resize(capacity);
    return ht;
}

void deleteOATable(OATable* ht) {
    delete ht;
}


int findSlotOA(OATable* ht, const string& key) {
    int index = hashFunction(key, ht->capacity);
    int startIndex = index;
    bool foundDeleted = false;
    int deletedIndex = -1;
    
    // ищем точное совпадение или первую удаленную ячейку
    
    do {
        if (ht->cells[index].state == OCCUPIED && ht->cells[index].key == key) {
            return index; // Найден существующий элемент
        }
        if (!foundDeleted && ht->cells[index].state == DELETED) {
            foundDeleted = true;
            deletedIndex = index;
        }
        if (ht->cells[index].state == EMPTY) {
            // Возвращаем либо удаленную ячейку, либо пустую
            
            return foundDeleted ? deletedIndex : index;
        }
        index = (index + 1) % ht->capacity;
    } while (index != startIndex);
    
    // Если таблица полная, возвращаем первую удаленную ячейку или -1
    
    return foundDeleted ? deletedIndex : -1;
}


bool containsOA(OATable* ht, const string& key) {
    int index = findSlotOA(ht, key);
    return index != -1 && ht->cells[index].state == OCCUPIED && ht->cells[index].key == key;
}

bool insertOA(OATable* ht, const string& key, int value) {
    if (ht->size >= ht->capacity) {
        cout << "Hash table is full!" << endl;
        return false;
    }
    
    int index = findSlotOA(ht, key);
    if (index == -1) {
        cout << "No available slot found!" << endl;
        return false;
    }
    
    if (ht->cells[index].state == OCCUPIED && ht->cells[index].key == key) {
        return false; // Элемент уже существует
    }
    
    ht->cells[index].key = key;
    ht->cells[index].value = value;
    ht->cells[index].state = OCCUPIED;
    ht->size++;
    
    return true;
}

bool removeOA(OATable* ht, const string& key) {
    int index = findSlotOA(ht, key);
    if (index != -1 && ht->cells[index].state == OCCUPIED && ht->cells[index].key == key) {
        ht->cells[index].state = DELETED;
        ht->size--;
        return true;
    }
    return false;
}

void printOATable(OATable* ht) {
    cout << "Hash Table (Open Addressing) - size: " << ht->size << endl;
    for (int i = 0; i < ht->capacity; i++) {
        cout << "Slot " << i << ": ";
        if (ht->cells[i].state == OCCUPIED) {
            cout << ht->cells[i].key << "(" << ht->cells[i].value << ")";
        } else if (ht->cells[i].state == DELETED) {
            cout << "DELETED";
        } else {
            cout << "EMPTY";
        }
        cout << endl;
    }
}

int getSizeOA(OATable* ht) {
    return ht->size;
}

// Для хэш-таблицы с цепочками (chaining)
int getValue(HTable* ht, const string& key) {
    int index = hashFunction(key, ht->capacity);
    HNode* curr = ht->buckets[index];
    
    while (curr != nullptr) {
        if (curr->key == key) {
            return curr->data;
        }
        curr = curr->next;
    }

    return -1;
}

int getValueOA(OATable* ht, const string& key) {
    int index = findSlotOA(ht, key);
    if (index != -1 && ht->cells[index].state == OCCUPIED && ht->cells[index].key == key) {
        return ht->cells[index].value;
    }

    return -1;
}
