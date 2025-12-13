#ifndef BST_H
#define BST_H

#include "book.h"

struct BSTNode {
Book data;
BSTNode* left;
BSTNode* right;
};

BSTNode* bst_insert(BSTNode* root, const Book& b);
BSTNode* bst_remove(BSTNode* root, const std::string& title, bool& removed);
BSTNode* bst_search(BSTNode* root, const std::string& title);
void bst_inorder_print(BSTNode* root);
void bst_free(BSTNode* root);
void bst_export_to_file(BSTNode* root, const std::string& filename);

#endif 