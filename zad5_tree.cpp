#include "tree.h"
#include <iostream>
using namespace std;

int main() {
    Tree tree;
    TINIT(tree);
    
    int number;
    while (cin >> number) {
        if (!TCONTAINS(tree, number)) {
            TINSERT(tree, number);
        }
    }
    
    cout << "Высота дерева: " << THEIGHT(tree) << endl;
    
    cout << "Размер дерева: " << TSIZE(tree) << endl;
    
    cout << "\nВизуальное представление дерева:" << endl;
    TPRINTTREE(tree);
    

    cout << "\nBFS обход (уровневой):" << endl;
    TPRINTBFS(tree);
    
    TCLEAR(tree);
    return 0;
}
