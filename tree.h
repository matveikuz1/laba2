#ifndef TREE_H
#define TREE_H

#include <string>
using namespace std;

struct TreeNode {
    int key;
    TreeNode* left;
    TreeNode* right;
    TreeNode* parent;
};

struct Tree {
    TreeNode* root;
};

// Структура для представления узла графа
struct GraphNode {
    int key;
    GraphNode* next;
};

// Структура для представления графа
struct Graph {
    GraphNode** adjacencyList;
    int vertexCount;
    int maxVertices;
};

// Объявления структуры стека для строк
struct Stack {
    struct Snode {
        string data;
        Snode* next;
    };
    
    Snode* top;
    int size;
};

// Функции для работы со стеком
void SCREATE(Stack* s);
void SPUSH(Stack* s, string value);
string SPOP(Stack* s);
string STOP(Stack* s);
bool SEMPTY(Stack* s);
int SSIZE(Stack* s);
void SPRINT(Stack* s);
void SCLEAR(Stack* s);

// Функции для работы с деревом
void TCREATE(Tree& tree, int key);
void TINIT(Tree& tree);
TreeNode* TCREATENODE(int key, TreeNode* parent = nullptr);
void TINSERT(Tree& tree, int value);
void TINSERTAT(Tree& tree, int parentValue, int newValue, bool isLeft);
void TREMOVE(Tree& tree, int value);
void TCLEAR(Tree& tree);
TreeNode* TFINDNODE(const Tree& tree, int value);
bool TCONTAINS(const Tree& tree, int value);
void TPRINTBFS(const Tree& tree);
void TPRINTDFSPREORDER(const Tree& tree);
void TPRINTDFSINORDER(const Tree& tree);
void TPRINTDFSPOSTORDER(const Tree& tree);
void TPRINTTREE(const Tree& tree);
bool TISFULL(const Tree& tree);
bool TISCOMPLETE(const Tree& tree);
bool TISBST(const Tree& tree);
int TSIZE(const Tree& tree);
int THEIGHT(const Tree& tree);
bool TEMPTY(const Tree& tree);
void TWRITETOFILE(const Tree& tree, const string& filename);
void TREADFROMFILE(Tree& tree, const string& filename);

#endif
