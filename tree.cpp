
#include "tree.h"
#include <iostream>
#include <fstream>
#include <queue>
#include <vector>
#include <sstream>
using namespace std;

// ===== РЕАЛИЗАЦИЯ СТЕКА =====

void SCREATE(Stack* st) {
    st->top = nullptr;
    st->size = 0;
}

void SPUSH(Stack* st, string value) {
    Stack::Snode* NewSnode = new Stack::Snode;
    NewSnode->data = value;
    NewSnode->next = st->top;
    st->top = NewSnode;
    st->size++;
}

string SPOP(Stack* st) {
    if (st->top == nullptr) {
        throw runtime_error("Stack is empty!");
    }
    Stack::Snode* delSnode = st->top;
    string value = delSnode->data;
    st->top = st->top->next;
    delete delSnode;
    st->size--;
    return value;
}

string STOP(Stack* st) {
    if (st->top == nullptr) {
        throw runtime_error("Stack is empty!");
    }
    return st->top->data;
}

bool SEMPTY(Stack* st) {
    return st->top == nullptr;
}

int SSIZE(Stack* st) {
    return st->size;
}

void SCLEAR(Stack* st) {
    while (st->top != nullptr) {
        SPOP(st);
    }
}

void SPRINT(Stack* st) {
    if (SEMPTY(st)) {
        cout << "Stack is empty" << endl;
        return;
    }

    cout << "Stack (top to bottom): ";
    Stack::Snode* curr = st->top;
    while (curr != nullptr) {
        cout << curr->data;
        if (curr->next != nullptr) {
            cout << " -> ";
        }
        curr = curr->next;
    }
    cout << endl;
}

// ===== РЕАЛИЗАЦИЯ ДЕРЕВА =====

void TCREATE(Tree& tree, int key) {
    tree.root = TCREATENODE(key);
}

void TINIT(Tree& tree) {
    tree.root = nullptr;
}

TreeNode* TCREATENODE(int key, TreeNode* parent) {
    TreeNode* newNode = new TreeNode{key, nullptr, nullptr, parent};
    return newNode;
}

TreeNode* findInsertionPoint(TreeNode* root) {
    if (root == nullptr) return nullptr;
    
    queue<TreeNode*> q;
    q.push(root);
    
    while (!q.empty()) {
        TreeNode* current = q.front();
        q.pop();
        
        if (current->left == nullptr || current->right == nullptr) {
            return current;
        }
        
        if (current->left != nullptr) q.push(current->left);
        if (current->right != nullptr) q.push(current->right);
    }
    
    return nullptr;
}

bool isFullBinaryTree(TreeNode* root) {
    if (root == nullptr) return true;
    
    if (root->left == nullptr && root->right == nullptr) return true;
    
    if (root->left != nullptr && root->right != nullptr) {
        return isFullBinaryTree(root->left) && isFullBinaryTree(root->right);
    }
    
    return false;
}

void TINSERT(Tree& tree, int value) {
    if (tree.root == nullptr) {
        TCREATE(tree, value);
        return;
    }
    
    TreeNode* current = tree.root;
    TreeNode* parent = nullptr;
    
    while (current != nullptr) {
        parent = current;
        
        if (value < current->key) {
            current = current->left;
        } else {
            current = current->right;
        }
    }
    
    if (value < parent->key) {
        parent->left = TCREATENODE(value, parent);
    } else {
        parent->right = TCREATENODE(value, parent);
    }
}

void TINSERTAT(Tree& tree, int parentValue, int newValue, bool isLeft) {
    TreeNode* parentNode = TFINDNODE(tree, parentValue);
    if (parentNode == nullptr) {
        throw runtime_error("Родительский узел не найден");
    }
    
    if (isLeft) {
        if (parentNode->left != nullptr) {
            throw runtime_error("Левый ребенок уже существует");
        }
        parentNode->left = TCREATENODE(newValue, parentNode);
    } else {
        if (parentNode->right != nullptr) {
            throw runtime_error("Правый ребенок уже существует");
        }
        parentNode->right = TCREATENODE(newValue, parentNode);
    }
}

TreeNode* findDeepestNode(TreeNode* root) {
    if (root == nullptr) return nullptr;
    
    queue<TreeNode*> q;
    q.push(root);
    TreeNode* deepest = nullptr;
    
    while (!q.empty()) {
        deepest = q.front();
        q.pop();
        
        if (deepest->left != nullptr) q.push(deepest->left);
        if (deepest->right != nullptr) q.push(deepest->right);
    }
    
    return deepest;
}

void TREMOVE(Tree& tree, int value) {
    if (tree.root == nullptr) {
        throw runtime_error("Дерево пусто");
    }
    
    TreeNode* nodeToRemove = TFINDNODE(tree, value);
    if (nodeToRemove == nullptr) {
        throw runtime_error("Узел не найден");
    }
    
    TreeNode* deepestNode = findDeepestNode(tree.root);
    if (deepestNode == nullptr) {
        throw runtime_error("Не удалось найти узел для замены");
    }
    
    if (nodeToRemove == deepestNode) {
        if (deepestNode->parent != nullptr) {
            if (deepestNode->parent->left == deepestNode) {
                deepestNode->parent->left = nullptr;
            } else {
                deepestNode->parent->right = nullptr;
            }
        } else {
            tree.root = nullptr;
        }
        delete deepestNode;
        return;
    }
    
    nodeToRemove->key = deepestNode->key;
    
    if (deepestNode->parent != nullptr) {
        if (deepestNode->parent->left == deepestNode) {
            deepestNode->parent->left = nullptr;
        } else {
            deepestNode->parent->right = nullptr;
        }
    }
    delete deepestNode;
}

void clearTreeHelper(TreeNode* node) {
    if (node == nullptr) return;
    
    clearTreeHelper(node->left);
    clearTreeHelper(node->right);
    delete node;
}

void TCLEAR(Tree& tree) {
    clearTreeHelper(tree.root);
    tree.root = nullptr;
}

// ===== ПОИСК =====

TreeNode* findNodeHelper(TreeNode* node, int value) {
    if (node == nullptr) return nullptr;
    if (node->key == value) return node;
    
    TreeNode* leftResult = findNodeHelper(node->left, value);
    if (leftResult != nullptr) return leftResult;
    
    return findNodeHelper(node->right, value);
}

TreeNode* TFINDNODE(const Tree& tree, int value) {
    return findNodeHelper(tree.root, value);
}

bool TCONTAINS(const Tree& tree, int value) {
    return TFINDNODE(tree, value) != nullptr;
}

// ===== ОБХОДЫ =====

void TPRINTBFS(const Tree& tree) {
    if (tree.root == nullptr) {
        cout << "Дерево пусто" << endl;
        return;
    }
    
    queue<TreeNode*> q;
    q.push(tree.root);
    
    cout << "BFS обход: ";
    while (!q.empty()) {
        TreeNode* current = q.front();
        q.pop();
        cout << current->key << " ";
        
        if (current->left != nullptr) q.push(current->left);
        if (current->right != nullptr) q.push(current->right);
    }
    cout << endl;
}


void TPRINTDFSPREORDER(const Tree& tree) {
    if (tree.root == nullptr) {
        cout << "Дерево пусто" << endl;
        return;
    }
    
    Stack st;
    SCREATE(&st);
    SPUSH(&st, to_string(reinterpret_cast<long long>(tree.root)));
    
    cout << "DFS Pre-order (итеративный): ";
    
    while (!SEMPTY(&st)) {
        string topStr = SPOP(&st);
        TreeNode* node = reinterpret_cast<TreeNode*>(stoll(topStr));
        
        cout << node->key << " ";
        
        if (node->right != nullptr) {
            SPUSH(&st, to_string(reinterpret_cast<long long>(node->right)));
        }
        if (node->left != nullptr) {
            SPUSH(&st, to_string(reinterpret_cast<long long>(node->left)));
        }
    }
    cout << endl;
}



void printTreeHelper(TreeNode* root, string prefix = "", bool isLeft = false) {
    if (root == nullptr) return;

    printTreeHelper(root->right, prefix + (isLeft ? "|   " : "    "), false);

    cout << prefix;
    cout << (isLeft ? "\\-- " : "/-- ");
    cout << root->key << endl;

    printTreeHelper(root->left, prefix + (isLeft ? "    " : "|   "), true);
}

void TPRINTTREE(const Tree& tree) {
    if (tree.root == nullptr) {
        cout << "Дерево пусто" << endl;
        return;
    }
    printTreeHelper(tree.root);
}

// ===== ПРОВЕРКИ =====

bool TISFULL(const Tree& tree) {
    return isFullBinaryTree(tree.root);
}

bool isCompleteBinaryTree(TreeNode* root, int index, int nodeCount) {
    if (root == nullptr) return true;
    if (index >= nodeCount) return false;
    
    return isCompleteBinaryTree(root->left, 2 * index + 1, nodeCount) &&
           isCompleteBinaryTree(root->right, 2 * index + 2, nodeCount);
}

int countNodes(TreeNode* root) {
    if (root == nullptr) return 0;
    return 1 + countNodes(root->left) + countNodes(root->right);
}

bool TISCOMPLETE(const Tree& tree) {
    int nodeCount = countNodes(tree.root);
    return isCompleteBinaryTree(tree.root, 0, nodeCount);
}

bool isBSTHelper(TreeNode* node, TreeNode* minNode = nullptr, TreeNode* maxNode = nullptr) {
    if (node == nullptr) return true;

    if (minNode != nullptr && node->key <= minNode->key) {
        return false;
    }
    if (maxNode != nullptr && node->key >= maxNode->key) {
        return false;
    }

    return isBSTHelper(node->left, minNode, node) &&
           isBSTHelper(node->right, node, maxNode);
}

bool TISBST(const Tree& tree) {
    return isBSTHelper(tree.root);
}

int TSIZE(const Tree& tree) {
    return countNodes(tree.root);
}

int heightHelper(TreeNode* node) {
    if (node == nullptr) return 0;
    return 1 + max(heightHelper(node->left), heightHelper(node->right));
}

int THEIGHT(const Tree& tree) {
    return heightHelper(tree.root);
}

bool TEMPTY(const Tree& tree) {
    return tree.root == nullptr;
}

// ===== ФАЙЛОВЫЕ ОПЕРАЦИИ =====

void serializeTree(TreeNode* node, ostream& os) {
    if (node == nullptr) {
        os << "null ";
        return;
    }
    
    os << node->key << " ";
    serializeTree(node->left, os);
    serializeTree(node->right, os);
}

void TWRITETOFILE(const Tree& tree, const string& filename) {
    ofstream file(filename);
    if (!file.is_open()) {
        throw runtime_error("Не удалось открыть файл для записи");
    }
    
    serializeTree(tree.root, file);
    file.close();
}

TreeNode* deserializeTree(istream& is, TreeNode* parent = nullptr) {
    string token;
    if (!(is >> token) || token == "null") {
        return nullptr;
    }
    
    int value = stoi(token);
    TreeNode* node = TCREATENODE(value, parent);
    node->left = deserializeTree(is, node);
    node->right = deserializeTree(is, node);
    
    return node;
}

void TREADFROMFILE(Tree& tree, const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        throw runtime_error("Не удалось открыть файл для чтения");
    }
    
    TCLEAR(tree);
    tree.root = deserializeTree(file);
    file.close();
}

// ===== ФУНКЦИИ ДЛЯ РАБОТЫ С КОМАНДНОЙ СТРОКОЙ =====

bool createNewTreeFile(const string& filename, const string& initialData = "") {
    ofstream file(filename);
    if (!file.is_open()) {
        cout << "Error: Cannot create file " << filename << endl;
        return false;
    }
    
    if (!initialData.empty()) {
        file << initialData << endl;
    }
    
    file.close();
    cout << "New tree file created: " << filename << endl;
    return true;
}

void processQuery(Tree& tree, const string& query, const string& filename = "", const string& outputFile = "") {
    istringstream iss(query);
    string operation;
    iss >> operation;
    
    try {
        if (operation == "INSERT") {
            int value;
            iss >> value;
            TINSERT(tree, value);
            cout << "Element " << value << " inserted successfully" << endl;
            if (!filename.empty()) {
                TWRITETOFILE(tree, filename);
            }
        }
        else if (operation == "INSERTAT") {
            int parentValue, newValue;
            string side;
            iss >> parentValue >> newValue >> side;
            TINSERTAT(tree, parentValue, newValue, side == "left");
            cout << "Element " << newValue << " inserted " 
                 << (side == "left" ? "left" : "right") 
                 << " of " << parentValue << endl;
            if (!filename.empty()) {
                TWRITETOFILE(tree, filename);
            }
        }
        else if (operation == "REMOVE") {
            int value;
            iss >> value;
            TREMOVE(tree, value);
            cout << "Element " << value << " removed successfully" << endl;
            if (!filename.empty()) {
                TWRITETOFILE(tree, filename);
            }
        }
        else if (operation == "FIND") {
            int value;
            iss >> value;
            TreeNode* found = TFINDNODE(tree, value);
            if (found != nullptr) {
                cout << "Element " << value << " found in the tree" << endl;
            } else {
                cout << "Element " << value << " not found in the tree" << endl;
            }
        }
        else if (operation == "CONTAINS") {
            int value;
            iss >> value;
            bool contains = TCONTAINS(tree, value);
            cout << "Tree " << (contains ? "contains" : "does not contain") 
                 << " element " << value << endl;
        }
        else if (operation == "BFS") {
            TPRINTBFS(tree);
        }
        else if (operation == "DFS_PREORDER") {
            TPRINTDFSPREORDER(tree);
        }

        else if (operation == "PRINT_TREE") {
            TPRINTTREE(tree);
        }
        else if (operation == "IS_FULL") {
            cout << "Tree is " << (TISFULL(tree) ? "" : "not ") << "a full binary tree" << endl;
        }
        else if (operation == "IS_COMPLETE") {
            cout << "Tree is " << (TISCOMPLETE(tree) ? "" : "not ") << "a complete binary tree" << endl;
        }
        else if (operation == "IS_BST") {
            cout << "Tree is " << (TISBST(tree) ? "" : "not ") << "a binary search tree" << endl;
        }
        else if (operation == "SIZE") {
            cout << "Tree size: " << TSIZE(tree) << endl;
        }
        else if (operation == "HEIGHT") {
            cout << "Tree height: " << THEIGHT(tree) << endl;
        }
        else if (operation == "EMPTY") {
            cout << "Tree is " << (TEMPTY(tree) ? "empty" : "not empty") << endl;
        }
        else if (operation == "CLEAR") {
            TCLEAR(tree);
            cout << "Tree cleared" << endl;
            if (!filename.empty()) {
                TWRITETOFILE(tree, filename);
            }
        }
        else if (operation == "CREATE") {
            int value;
            iss >> value;
            TCREATE(tree, value);
            cout << "Tree created with root " << value << endl;
            if (!filename.empty()) {
                TWRITETOFILE(tree, filename);
            }
        }
        
        if (!outputFile.empty() && operation != "") {
            TWRITETOFILE(tree, outputFile);
            cout << "Result saved to " << outputFile << endl;
        }
        
    } catch (const exception& e) {
        cout << "Error: " << e.what() << endl;
    }
}

void printHelp() {
    cout << "Использование: program --file <имя_файла> [--query <операция>] [--create] [--output <выходной_файл>]" << endl;
    cout << "Доступные операции:" << endl;
    cout << "  ОПЕРАЦИИ С ДЕРЕВОМ:" << endl;
    cout << "  INSERT <значение>                 - Вставить элемент в дерево" << endl;
    cout << "  INSERTAT <родитель> <значение> <сторона> - Вставить элемент в определенную позицию" << endl;
    cout << "  REMOVE <значение>                 - Удалить элемент из дерева" << endl;
    cout << "  FIND <значение>                   - Найти элемент в дереве" << endl;
    cout << "  CONTAINS <значение>               - Проверить наличие элемента в дереве" << endl;
    cout << "  BFS                               - Обход в ширину (BFS)" << endl;
    cout << "  DFS_PREORDER                      - Обход в глубину Pre-order" << endl;
    cout << "  PRINT_TREE                        - Вывести структуру дерева" << endl;
    cout << "  IS_FULL                           - Проверить является ли дерево полным" << endl;
    cout << "  IS_COMPLETE                       - Проверить является ли дерево завершенным" << endl;
    cout << "  IS_BST                            - Проверить является ли дерево бинарным деревом поиска" << endl;
    cout << "  SIZE                              - Получить размер дерева" << endl;
    cout << "  HEIGHT                            - Получить высоту дерева" << endl;
    cout << "  EMPTY                             - Проверить пустое ли дерево" << endl;
    cout << "  CLEAR                             - Очистить дерево" << endl;
    cout << "  CREATE <значение>                 - Создать новое дерево с корнем" << endl;
    cout << endl;
    }

int main(int argc, char* argv[]) {
    string filename, query, outputFile;
    bool createNew = false;
    string initialData;
    
    for (int i = 1; i < argc; i++) {
        string arg = argv[i];
        
        if (arg == "--file" && i + 1 < argc) {
            filename = argv[++i];
        }
        else if (arg == "--query" && i + 1 < argc) {
            query = argv[++i];
        }
        else if (arg == "--create" || arg == "-c") {
            createNew = true;
        }
        else if (arg == "--data" && i + 1 < argc) {
            initialData = argv[++i];
        }
        else if (arg == "--output" && i + 1 < argc) {
            outputFile = argv[++i];
        }
        else if (arg == "--help" || arg == "-h") {
            printHelp();
            return 0;
        }
    }
    
    if (filename.empty()) {
        cout << "Error: Missing filename" << endl;
        printHelp();
        return 1;
    }
    
    if (createNew) {
        if (!createNewTreeFile(filename, initialData)) {
            return 1;
        }
    }
    
    Tree tree;
    TINIT(tree);
    

    
    try {
        TREADFROMFILE(tree, filename);
        cout << "Tree loaded from " << filename << " (size: " << TSIZE(tree) << ")" << endl;
        

    } catch (const exception& e) {
        if (createNew) {
            cout << "New tree created" << endl;
        } else {
            cout << "Error loading tree: " << e.what() << endl;
            cout << "Use --create to create a new tree file." << endl;
            return 1;
        }
    }
    
    if (!query.empty()) {
        processQuery(tree, query, filename, outputFile);
    } else {
        cout << "No query specified. Use --query <operation>" << endl;
        cout << "Use --help for available operations." << endl;
    }
    
    TCLEAR(tree);
    
    return 0;
}
