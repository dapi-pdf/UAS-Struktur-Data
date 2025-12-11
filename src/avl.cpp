#include <iostream>
#include <fstream>
#include <algorithm>
#include <functional>
#include "../header/avl.h"

static std::string toLowerA(const std::string& s) {
    std::string r = s;
    std::transform(r.begin(), r.end(), r.begin(), ::tolower);
    return r;
}

int height(AVLNode* n) { return n ? n->height : 0; }
int balanceFactor(AVLNode* n) { return n ? height(n->left) - height(n->right) : 0; }

AVLNode* create_anode(const Book& b) {
    AVLNode* n = new AVLNode{b, nullptr, nullptr, 1};
    return n;
}

AVLNode* rightRotate(AVLNode* y) {
    AVLNode* x = y->left;
    AVLNode* T2 = x->right;
    x->right = y;
    y->left = T2;
    y->height = std::max(height(y->left), height(y->right)) + 1;
    x->height = std::max(height(x->left), height(x->right)) + 1;
    return x;
}

AVLNode* leftRotate(AVLNode* x) {
    AVLNode* y = x->right;
    AVLNode* T2 = y->left;
    y->left = x;
    x->right = T2;
    x->height = std::max(height(x->left), height(x->right)) + 1;
    y->height = std::max(height(y->left), height(y->right)) + 1;
    return y;
}

AVLNode* avl_insert(AVLNode* node, const Book& b) {
    if (!node) return create_anode(b);
    std::string key = toLowerA(b.title);
    std::string nkey = toLowerA(node->data.title);
    if (key < nkey) node->left = avl_insert(node->left, b);
    else node->right = avl_insert(node->right, b);

    node->height = 1 + std::max(height(node->left), height(node->right));
    int bf = balanceFactor(node);

    // LL
    if (bf > 1 && toLowerA(b.title) < toLowerA(node->left->data.title))
        return rightRotate(node);
    // RR
    if (bf < -1 && toLowerA(b.title) > toLowerA(node->right->data.title))
        return leftRotate(node);
    // LR
    if (bf > 1 && toLowerA(b.title) > toLowerA(node->left->data.title)) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }
    // RL
    if (bf < -1 && toLowerA(b.title) < toLowerA(node->right->data.title)) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }
    return node;
}

AVLNode* find_min_avl(AVLNode* node) {
    AVLNode* cur = node;
    while (cur && cur->left) cur = cur->left;
    return cur;
}

AVLNode* avl_remove(AVLNode* root, const std::string& title, bool& removed) {
    if (!root) return nullptr;
    std::string key = toLowerA(title);
    std::string rkey = toLowerA(root->data.title);
    if (key < rkey) root->left = avl_remove(root->left, title, removed);
    else if (key > rkey) root->right = avl_remove(root->right, title, removed);
    else {
        removed = true;
        if (!root->left || !root->right) {
            AVLNode* temp = root->left ? root->left : root->right;
            if (!temp) {
                delete root;
                return nullptr;
            } else {
                *root = *temp;
                delete temp;
            }
        } else {
            AVLNode* temp = find_min_avl(root->right);
            root->data = temp->data;
            root->right = avl_remove(root->right, temp->data.title, removed);
        }
    }
    if (!root) return root;
    root->height = 1 + std::max(height(root->left), height(root->right));
    int bf = balanceFactor(root);
    // LL
    if (bf > 1 && balanceFactor(root->left) >= 0) return rightRotate(root);
    // LR
    if (bf > 1 && balanceFactor(root->left) < 0) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }
    // RR
    if (bf < -1 && balanceFactor(root->right) <= 0) return leftRotate(root);
    // RL
    if (bf < -1 && balanceFactor(root->right) > 0) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }
    return root;
}

AVLNode* avl_search(AVLNode* root, const std::string& title) {
    if (!root) return nullptr;
    std::string key = toLowerA(title);
    std::string rkey = toLowerA(root->data.title);
    if (key == rkey) return root;
    if (key < rkey) return avl_search(root->left, title);
    return avl_search(root->right, title);
}

void avl_inorder_print(AVLNode* root) {
    if (!root) return;
    avl_inorder_print(root->left);
    std::cout << "[" << root->data.id << "] " << root->data.title << " - " << root->data.author << " (" << root->data.year << ")\n";
    avl_inorder_print(root->right);
}

void avl_free(AVLNode* root) {
    if (!root) return;
    avl_free(root->left);
    avl_free(root->right);
    delete root;
}

void avl_export_to_file(AVLNode* root, const std::string& filename) {
    std::ofstream ofs(filename);
    if (!ofs) {
        std::cerr << "Gagal membuka file " << filename << " untuk menulis.\n";
        return;
    }
    std::function<void(AVLNode*)> write_inorder = [&](AVLNode* node) {
        if (!node) return;
        write_inorder(node->left);
        ofs << node->data.id << '\t' << node->data.title << '\t' << node->data.author << '\t' << node->data.year << '\n';
        write_inorder(node->right);
    };
    write_inorder(root);
    ofs.close();
}