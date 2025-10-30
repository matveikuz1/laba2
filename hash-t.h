#ifndef HASH_T_H
#define HASH_T_H

#include <string>
#include <vector>

using namespace std;

//Метод цепочек

struct HNode {
    string key;
    int data;
    HNode* next;
    HNode(const string& k, int d = 0);
};

struct HTable {
    vector<HNode*> buckets;
    int capacity;
    int size;
};

// Базовые операции для метода цепочек
HTable* createTable(int capacity);
void deleteTable(HTable* ht);
int hashFunction(const string& key, int capacity);
bool contains(HTable* ht, const string& key);
bool insert(HTable* ht, const string& key, int value = 0);
bool remove(HTable* ht, const string& key);
void printTable(HTable* ht);
int getSize(HTable* ht);
int getValue(HTable* ht, const string& key);  //    н Для цепочек

//Open адрессация

enum CellState {
    EMPTY,
    OCCUPIED,
    DELETED
};

struct OACell {
    string key;
    int value;
    CellState state;
    
    OACell() : key(""), value(0), state(EMPTY) {}
};

struct OATable {
    vector<OACell> cells;
    int capacity;
    int size;
};

// Операции для метода открытой адресации
OATable* createOATable(int capacity);
int findSlotOA(OATable* ht, const string& key);
void deleteOATable(OATable* ht);
bool containsOA(OATable* ht, const string& key);
bool insertOA(OATable* ht, const string& key, int value = 0);
bool removeOA(OATable* ht, const string& key);
void printOATable(OATable* ht);
int getSizeOA(OATable* ht); 
#endif
