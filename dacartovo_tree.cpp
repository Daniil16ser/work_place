// split next task 
// декартово дерево
// merge, delete, split
// std::set - бин дерево поиска
// какому-то объекту можно приствоить значение 
// что должно быть у объекта, чтобы положить его в map: должен быть оператор меньше 
// shd::map<int,char> int - ключ, st[5] = 'c' опасноть в том, что если такого объекта нету, 
// то он создастся, нужно писать: auto it = st.find(5) - работает за log(h), тк перебирает как дерево; if (it != st.end()){it -> second = 'c'}
// st = std::find(st.begin(), st.end(), 5) работает за линию

#include <iostream>
#include <cstdlib>
#include <ctime>

template <typename T>
class Treap {bin_tree_search
private:
    struct Node {
        T value;
        int priority;
        int count;
        int size;
        Node* left;
        Node* right;

        Node(const T& val) 
            : value(val), priority(rand()), count(1), size(1), left(nullptr), right(nullptr) {}
    };

    Node* root;

    void updateSize(Node* node) {
        if (node) {
            node->size = node->count;
            if (node->left) node->size += node->left->size;
            if (node->right) node->size += node->right->size;
        }
    }

    Node* insert(Node* node, Node* newNode) {
        if (!node) return newNode;

        if (newNode->priority > node->priority) {
            if (newNode->value < node->value) {
                node->left = insert(node->left, newNode);
            } else {
                node->right = insert(node->right, newNode);
            }
            updateSize(node);
            return node;
        }

        if (newNode->value < node->value) {
            node->left = insert(node->left, newNode);
        } else if (newNode->value > node->value) {
            node->right = insert(node->right, newNode);
        } else {
            node->count++;
        }

        updateSize(node);
        return node;
    }

    Node* remove(Node* node, const T& key) {
        if (!node) return nullptr;

        if (key < node->value) {
            node->left = remove(node->left, key);
        } else if (key > node->value) {
            node->right = remove(node->right, key);
        } else {
            if (node->count > 1) {
                node->count--;
            } else {
                if (!node->left && !node->right) {
                    delete node;
                    return nullptr;
                } else if (!node->left) {
                    Node* temp = node->right;
                    delete node;
                    node = temp;
                } else if (!node->right) {
                    Node* temp = node->left;
                    delete node;
                    node = temp;
                } else {
                    if (node->left->priority > node->right->priority) {
                        Node* temp = node->left;
                        node->left = remove(node->left, temp->value);
                        temp->right = node->right;
                        temp->left = node->left;
                        delete node;
                        node = temp;
                    } else {
                        Node* temp = node->right;
                        node->right = remove(node->right, temp->value);
                        temp->left = node->left;
                        temp->right = node->right;
                        delete node;
                        node = temp;
                    }
                }
            }
        }

        updateSize(node);
        return node;
    }

    void clear(Node* node) {
        if (node) {
            clear(node->left);
            clear(node->right);
            delete node;
        }
    }

public:
    Treap() : root(nullptr) {
        std::srand(std::time(nullptr));
    }

    ~Treap() {
        clear(root);
    }

    void insert(const T& value) {
        Node* newNode = new Node(value);
        root = insert(root, newNode);
    }

    void remove(const T& value) {
        root = remove(root, value);
    }

    int size() const {
        if (!root) {
            return 0;  
        } else {
            return root->size;  
        }
    }
};

int main() {
    Treap<int> tree;

    tree.insert(5);
    tree.insert(3);
    tree.insert(7);
    tree.insert(5);
    tree.insert(9);

    std::cout << "Size: " << tree.size() << std::endl; // 5

    tree.remove(5);
    std::cout << tree.size() << std::endl; // 4

    tree.remove(5);
    std::cout  << tree.size() << std::endl; // 3

    return 0;
}