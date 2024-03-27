#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

struct Node { //структура узла дерева
    char symbol; //символ
    int count; //его вес (количество в строке)
    struct Node* left, * right; //указатели на дочерние узлы
    Node(char symbol) { //конструктор с символом
        this->symbol = symbol;
        this->count = 1;
        this->left = nullptr;
        this->right = nullptr;
    }
    Node(Node* left, Node* right) { //конструктор для создания нового узла, который будет иметь суммарный вес двух дочерних и указатели на них
        this->symbol = '\0';
        this->count = left->count + right->count;
        this->left = left;
        this->right = right;
    }
    void deleteNode(Node* node) { //функция удаления всех дочерних элементов
        if (node != nullptr) {
            deleteNode(node->left);
            deleteNode(node->right);
            delete node;
        }
    }
    ~Node() { //деструктор
        deleteNode(this);
    }
};

Node* makeTreeOfSymbols(string str) { //использующая алгоритм Хаффмана функция создания бинарного дерева и возвращающая указатель на его корень
    vector<Node*> symbols; //создаем вектор указателей на узлы
    for (const auto& el : str) { //проходимся по всем символам строки
        auto it = find_if(symbols.begin(), symbols.end(), [&el](Node* node) { //получаем итератор, который указывает на место символа в векторе
            return node->symbol == el;
            });
        if (it != symbols.end()) { //если итератор не указывает на конец вектора, то значит символ уже есть в векторе, и нужно лишь увеличить его вес
            (*it)->count++;
        }
        else {
            symbols.push_back(new Node(el)); //если же вектор указал на конец вектора, то такого символа нет в векторе, соответственно, добавляем новый элемент
        }
    }

    sort(symbols.begin(), symbols.end(), [](Node* node1, Node* node2) { //сортируем наш вектор по возрастанию весов
        return node1->count < node2->count;
        });

    while (symbols.size() > 1) { //выполнение алгоритма Хаффмана до момента, пока все веса не сложатся (тогда мы получим вес корневого элемента)
        Node* node = new Node(*symbols.begin(), *(symbols.begin() + 1)); //создаем новый указатель на узел с помощью двух самых маленьких по весу символов
        symbols.erase(symbols.begin(), symbols.begin() + 2); //удаляем эти два самых маленьких по весу указателя из вектора
        symbols.insert(symbols.begin(), node); //вставляем на их место в начало наш новый указатель, который указывает на узел, который имеет суммарный вес двух удаленных указателей и указатели на них внутри себя
        sort(symbols.begin(), symbols.end(), [](Node* node1, Node* node2) { //снова сортируем наш вектор по возрастанию весов символов, так как добавленный в начало элемент может оказаться не самым легким по весу
            return node1->count < node2->count;
            });
    }
    return *symbols.begin(); //когда в векторе останется лишь один элемент, это будет означать, что он корневой и находится единственным в векторе, возвращаем его как ссылку на корень дерева
}

void getBinaryCodes(Node* root, string str, vector<pair<char, string>>& alphabet) { //функция прохода по всему дереву и составления двоичного кода для каждого элемента + закидываем в вектор пары типа символ-бинарный код
    if (root->symbol != '\0') {
        alphabet.push_back({ root->symbol, str });
    }
    if (root->left != nullptr) { //если есть куда идти влево, идем и добавляем к бинарному коду 0
        getBinaryCodes(root->left, str + '0', alphabet);
    }
    if (root->right != nullptr) { //если есть куда идти вправо, идем и добавляем к бинарному коду 1
        getBinaryCodes(root->right, str + '1', alphabet);
    }
    if (!str.empty()) { //если дошли до тупика, то возвращаемся назад, так же убираем последний бит из бинарного кода
        str.erase(str.end() - 1);
    }
}

int main()
{
    string str = "", result = "";
    cout << "Enter your string: ";
    cin >> str;
    Node* root = makeTreeOfSymbols(str); //вызываем функцию создания дерева символов (которая использует алгоритм Хаффмана) по введенной пользователем строке
    vector<pair<char, string>> alphabet;
    getBinaryCodes(root, string(), alphabet); //вызываем функцию для получения бинарных комбинаций, которые запишутся в алфавит (вектор пар символ-комбинация)
    sort(alphabet.begin(), alphabet.end(), [](const auto& pair1, const auto& pair2) { //сотрировка алфавита по возрастанию веса символов
        return pair1.second.length() < pair2.second.length();
        });
    for (const auto& el : alphabet) { //вывод алфавита
        cout << el.first << ": " << el.second << endl;
    }
    for (const auto& el : str) { //вывод закодированного двоичным кодом слова
        cout << find_if(alphabet.begin(), alphabet.end(), [&el](const auto& pair) {
            return pair.first == el;
            })->second;
    }
    return 0;
}