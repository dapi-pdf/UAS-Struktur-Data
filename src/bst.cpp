#include <iostream>
#include <fstream>
#include <algorithm>
#include <functional>
#include "../header/bst.h"


static std::string toLower(const std::string& s) {
  std::string r = s;
  std::transform(r.begin(), r.end(), r.begin(), ::tolower);
  return r;
}


BSTNode* create_node(const Book& b) {
  BSTNode* n = new BSTNode{b, nullptr, nullptr};
  return n;
}


BSTNode* bst_insert(BSTNode* root, const Book& b) {
  if (!root) return create_node(b);
  std::string key = toLower(b.title);
  std::string rkey = toLower(root->data.title);
  if (key < rkey) root->left = bst_insert(root->left, b);
  else root->right = bst_insert(root->right, b);
  return root;
}


BSTNode* find_min(BSTNode* node) {
  while (node && node->left) node = node->left;
  return node;
}


BSTNode* bst_remove(BSTNode* root, const std::string& title, bool& removed) {
  if (!root) return nullptr;
  std::string key = toLower(title);
  std::string rkey = toLower(root->data.title);
  if (key < rkey) root->left = bst_remove(root->left, title, removed);
  else if (key > rkey) root->right = bst_remove(root->right, title, removed);
  else {
    // found
    removed = true;
    if (!root->left) {
      BSTNode* r = root->right;
      delete root;
      return r;
    } else if (!root->right) {
      BSTNode* l = root->left;
      delete root;
      return l;
    } else {
      BSTNode* minr = find_min(root->right);
      root->data = minr->data;
      root->right = bst_remove(root->right, minr->data.title, removed);
      // note: removed already set true
      removed = true;
    }
  }
  return root;
}

BSTNode* bst_search(BSTNode* root, const std::string& title) {
  if (!root) return nullptr;
  std::string key = toLower(title);
  std::string rkey = toLower(root->data.title);
  if (key == rkey) return root;
  if (key < rkey) return bst_search(root->left, title);
  return bst_search(root->right, title);
}

void bst_inorder_print(BSTNode* root) {
  if (!root) return;
  bst_inorder_print(root->left);
  std::cout << "[" << root->data.id << "] " << root->data.title << " - " << root->data.author << " (" << root->data.year << ")\n";
  bst_inorder_print(root->right);
}


void bst_free(BSTNode* root) {
  if (!root) return;
  bst_free(root->left);
  bst_free(root->right);
  delete root;
}

void bst_export_to_file(BSTNode* root, const std::string& filename) {
    std::ofstream ofs(filename);
    if (!ofs) {
        std::cerr << "Gagal membuka file " << filename << " untuk menulis.\n";
        return;
    }
    // write as inorder
    std::function<void(BSTNode*)> write_inorder = [&](BSTNode* node) {
        if (!node) return;
        write_inorder(node->left);
        ofs << node->data.id << '\t' << node->data.title << '\t' << node->data.author << '\t' << node->data.year << '\n';
        write_inorder(node->right);
    };
    write_inorder(root);
    ofs.close();
}