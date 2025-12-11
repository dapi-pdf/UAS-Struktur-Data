// File: avl.h
#ifndef AVL_H
#define AVL_H

#include "book.h"

struct AVLNode {
  Book data;
  AVLNode* left;
  AVLNode* right;
  int height;
};

// API
AVLNode* avl_insert(AVLNode* root, const Book& b);
AVLNode* avl_remove(AVLNode* root, const std::string& title, bool& removed);
AVLNode* avl_search(AVLNode* root, const std::string& title);
void avl_inorder_print(AVLNode* root);
void avl_free(AVLNode* root);
void avl_export_to_file(AVLNode* root, const std::string& filename);

#endif // AVL_H