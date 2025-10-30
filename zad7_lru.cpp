#include "hash-t.h"
#include <iostream>
#include <string>
#include <sstream>

using namespace std;

struct LRUNode {
	string key;
	string value;
	LRUNode* prev;
	LRUNode* next;
	LRUNode(const string& k, const string& v) : key(k), value(v), prev(nullptr), next(nullptr) {}
};

struct LRUCache {
	int capacity;
	int size;
	LRUNode* head;
	LRUNode* tail;
	HTable* hashTable;
};

LRUCache* createLRUCache(int capacity) {
	LRUCache* cache = new LRUCache;
	cache->capacity = capacity;
	cache->size = 0;
	cache->head = nullptr;
	cache->tail = nullptr;
	cache->hashTable = createTable(capacity*2);
	return cache;
}

void deleteLRUCache (LRUCache* cache) {
	LRUNode* curr = cache->head;
	while (curr != nullptr) {
		LRUNode* temp = curr;
		curr = curr->next;
		delete temp;
	}
	deleteTable(cache->hashTable);
	delete cache;
}

void moveToFront(LRUCache* cache, LRUNode* node) {
	if (node == cache->head) return;
	if (node->prev) node->prev->next = node->next;
	if (node->next) node->next->prev = node->prev;
	if (node = cache->tail) cache->tail = node->prev;
	node->prev = nullptr;
	node->next = cache->head;
	if (cache->head != nullptr) cache->head->prev = node;
	cache->head = node;
	if (cache->head == nullptr) cache->tail = node;
}
void addToFront(LRUCache* cache, const string& key, const string& value) {
	LRUNode* newNode = new LRUNode(key,value);
	newNode->next = cache->head;
	if (cache->head != nullptr) cache->head->prev = newNode;
	cache->head = newNode;
	if (cache->tail == nullptr) cache->tail = newNode;
	cache->size++;
}
void removeOldest(LRUCache* cache) {
	if (cache->tail == nullptr) return;
	LRUNode* oldest = cache->tail;
	cache->tail = oldest->prev;
	if (cache->tail != nullptr) cache->tail->next = nullptr;
	if (oldest == cache->head) cache->head = nullptr;
	remove(cache->hashTable, oldest->key);
	delete oldest;
	cache->size--;
}

LRUNode* findNode(LRUCache* cache, const string& key) {
    LRUNode* curr = cache->head;
    while (curr != nullptr) {
        if (curr->key == key) {
            return curr;
        }
        curr = curr->next;
    }
    return nullptr;
}

void LRUSET(LRUCache* cache, const string& key, const string& value) {

    if (contains(cache->hashTable, key)) {

        LRUNode* node = findNode(cache, key);
        if (node != nullptr) {
            node->value = value;
            moveToFront(cache, node);
        }
    } else {
        if (cache->size >= cache->capacity) {
            removeOldest(cache);
        }

        addToFront(cache, key, value);
        insert(cache->hashTable,key,0);
    }
}

string LRUGET(LRUCache* cache, const string& key) {
	if (!contains(cache->hashTable,key)) return "-1";
	LRUNode* curr = cache->head;
	while (curr != nullptr) {
		if (curr->key == key) {
			moveToFront(cache,curr);
			return curr->value;
		}
		curr = curr->next;
	}
	return "-1";
}

void printLRUCache(LRUCache* cache) {
    cout << "LRU Cache (capacity: " << cache->capacity << ", size: " << cache->size << "): ";
    LRUNode* curr = cache->head;
    while (curr != nullptr) {
        cout << curr->key << "->" << curr->value;
        if (curr->next != nullptr) cout << ", ";
        curr = curr->next;
    }
    cout << endl;
}

void parseCommand(const string& command, string& operation, string& key, string& value) {
    operation = "";
    key = "";
    value = "";
    
    stringstream ss(command);
    ss >> operation;
    
    if (operation == "SET") {
        ss >> key >> value;
    } else if (operation == "GET") {
        ss >> key;
    }
}


string getState(LRUCache* cache) {
	if (cache->head == nullptr) return "";
	string state;
	LRUNode* curr = cache->head;
	while (curr != nullptr) {
		state += curr->key + "->" + curr->value;
		if (curr->next != nullptr) state += ", ";
		curr = curr->next;
	}
	return state;
}

int main() {
    int capacity;
    
    // Ввод емкости
    cout << "cap = ";
    cin >> capacity;
    cin.ignore();
    
    LRUCache* cache = createLRUCache(capacity);
    vector<string> results;
    
    cout << "Вводите команды (SET key value, GET key, PRINT, EXIT):" << endl;
    
    string command;
    while (true) {
        cout << "> ";
        getline(cin, command);
        
        if (command == "EXIT") break;
        
        string operation, key, value;
        parseCommand(command, operation, key, value);
        
        if (operation == "SET") {
            if (!key.empty() && !value.empty()) {
                LRUSET(cache, key, value);
                cout << "SET " << key << " " << value << " : " << getState(cache) << endl;
            } else {
                cout << "Ошибка: Используйте SET key value" << endl;
            }
        } else if (operation == "GET") {
            if (!key.empty()) {
                string result = LRUGET(cache, key);
                results.push_back(result);
                cout << "GET " << key << " : " << result << endl;
            } else {
                cout << "Ошибка: Используйте GET key" << endl;
            }
        } else if (command == "PRINT") {
            cout << "Текущее состояние: " << getState(cache) << endl;
        } else {
            cout << "Неизвестная команда. Используйте SET, GET, PRINT или EXIT" << endl;
        }
    }

    if (!results.empty()) {
        cout << "Вывод: ";
        for (size_t i = 0; i < results.size(); i++) {
            cout << results[i];
            if (i < results.size() - 1) cout << " ";
        }
        cout << endl;
    }
    
    deleteLRUCache(cache);
    return 0;
}
