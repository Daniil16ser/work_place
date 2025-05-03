#include <vector>
#include <algorithm>
#include <iostream>
#include <cassert>
#include <stdexcept>

struct AVLNode
{
    int value;
    AVLNode* left = nullptr;
    AVLNode* right = nullptr;
    int height = 1;
    int size = 1; // размер левого поддерева
};

class AVLTree
{
private:
    AVLNode* root;
    
    int getHeight(AVLNode* node) const {
        return node ? node->height : 0;
    }
    
    int getSize(AVLNode* node) const {
        return node ? node->size : 0;
    }
    
    int getBalance(AVLNode* node) const {
        return node ? getHeight(node->left) - getHeight(node->right) : 0;
    }
    
    void update(AVLNode* node) {
        if (!node) return;
        node->height = 1 + std::max(getHeight(node->left), getHeight(node->right));
        node->size = 1 + getSize(node->left) + getSize(node->right);
    }
    
    AVLNode* rotateRight(AVLNode* a){
        AVLNode* b=a->left;
        AVLNode*B = b->right;
        
        b->right = a;
        a->left =B;
        
        update(a);
        update(b);
        
        return b;
    }
    
    AVLNode* rotateLeft(AVLNode* a) {
        AVLNode* b= a->right;
        AVLNode*B = b->left;
        
        b->left = a;
        a->right =B;
        
        update(a);
        update(b);
        
        return b;
    }
    
    AVLNode* balance(AVLNode* node) {
        if (!node) return nullptr;
        
        update(node);
        
        int balanceFactor = getBalance(node);
        // малое правое вращение
        if (balanceFactor > 1 && getBalance(node->left) >= 0) {
            return rotateRight(node);
        }
        // малое левое вращение
        if (balanceFactor < -1 && getBalance(node->right) <= 0) {
            return rotateLeft(node);
        }
        //большое правое вращение
        if (balanceFactor > 1 && getBalance(node->left) < 0) {
            node->left = rotateLeft(node->left);
            return rotateRight(node);
        }
        // большое левое вращение
        if (balanceFactor < -1 && getBalance(node->right) > 0) {
            node->right = rotateRight(node->right);
            return rotateLeft(node);
        }
        
        return node;
    }
    
    AVLNode* insert(AVLNode* node, int value) {
        if (!node) return new AVLNode{value};
        
        if (value < node->value) {
            node->left = insert(node->left, value);
        } else if (value > node->value) {
            node->right = insert(node->right, value);
        } else {
            return node; // не может быть дубликатов
        }
        
        return balance(node);
    }
    
    int findKthHelper(AVLNode* node, int k) const {
        if (!node) {
            throw std::out_of_range("k is out of range");
        }
        
        int leftSize = getSize(node->left);
        
        if (k == leftSize) {
            return node->value;
        } else if (k < leftSize) {
            return findKthHelper(node->left, k);
        } else {
            return findKthHelper(node->right, k - leftSize - 1);
        }
    }
    
    void clear(AVLNode* node) {
        if (!node) return;
        clear(node->left);
        clear(node->right);
        delete node;
    }

public:
    AVLTree() : root(nullptr) {}
    ~AVLTree() {
        clear(root);
    }
    
    void insert(int value) {
        root = insert(root, value);
    }
    
    int findKth(int k) const {
        if (k < 0 || k >= getSize(root)) {
            throw std::out_of_range("k is out of range");
        }
        return findKthHelper(root, k);
    }
};

int kthSmallest(const std::vector<int>& nums, int k) {
    AVLTree tree;
    for (int num : nums) {
        tree.insert(num);
    }
    return tree.findKth(k);
}

int main() {
    std::vector<int> nums = {7, 10, 4, 3, 20, 15};
    int k = 5;
    
    std::cout << "Array: ";
    for (int num : nums) std::cout << num << " ";
    std::cout << "\n";
    
    std::cout << k << "-porudk is "<<kthSmallest(nums, k - 1) << std::endl;
    
    return 0;
}