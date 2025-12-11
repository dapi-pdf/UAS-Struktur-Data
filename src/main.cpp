// File: main.cpp
#include <iostream>
#include <string>
#include <limits>
#include "../header/bst.h"
#include "../header/avl.h"
#include "../header/book.h"

// helper to read a line safely
static void readLine(const std::string& prompt, std::string& out) {
    std::cout << prompt;
    std::getline(std::cin, out);
}

static int readInt(const std::string& prompt) {
    int x;
    while (true) {
        std::cout << prompt;
        if (std::cin >> x) {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return x;
        }
        std::cout << "Input tidak valid, coba lagi.\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
}

// sample data: minimal 10 books
Book sample_books[] = {
    {1, "Clean Code", "Robert C. Martin", 2008},
    {2, "Introduction to Algorithms", "Cormen et al.", 2009},
    {3, "Design Patterns", "Gamma et al.", 1994},
    {4, "The C++ Programming Language", "Bjarne Stroustrup", 2013},
    {5, "Effective C++", "Scott Meyers", 2005},
    {6, "Artificial Intelligence: A Modern Approach", "Russell & Norvig", 2010},
    {7, "Database System Concepts", "Korth", 2011},
    {8, "Operating System Concepts", "Silberschatz", 2012},
    {9, "Computer Networks", "Tanenbaum", 2010},
    {10, "Structure and Interpretation of Computer Programs", "Abelson & Sussman", 1996}
};

void print_main_menu() {
    std::cout << "\n=== SISTEM PENCARIAN BUKU (BST & AVL) ===\n";
    std::cout << "1. Gunakan BST (Binary Search Tree)\n";
    std::cout << "2. Gunakan AVL Tree\n";
    std::cout << "0. Keluar\n";
}

void print_tree_menu() {
    std::cout << "\n--- MENU OPERASI ---\n";
    std::cout << "1. Insert buku\n";
    std::cout << "2. Delete buku (by title)\n";
    std::cout << "3. Search buku (by title)\n";
    std::cout << "4. View all (in-order)\n";
    std::cout << "5. Export data to notepad (.txt)\n";
    std::cout << "9. Kembali ke pilihan tree\n";
}

int main() {
    BSTNode* bst_root = nullptr;
    AVLNode* avl_root = nullptr;
    // insert sample data into both trees
    for (auto &b : sample_books) {
        bst_root = bst_insert(bst_root, b);
        avl_root = avl_insert(avl_root, b);
    }

    while (true) {
        print_main_menu();
        int choice = readInt("Pilih: ");
        if (choice == 0) break;
        if (choice == 1 || choice == 2) {
            bool isBST = (choice == 1);
            while (true) {
                print_tree_menu();
                int op = readInt("Operasi: ");
                if (op == 9) break;
                if (op == 1) {
                    Book b;
                    b.id = readInt("Masukkan ID buku: ");
                    readLine("Masukkan judul: ", b.title);
                    readLine("Masukkan penulis: ", b.author);
                    b.year = readInt("Masukkan tahun terbit: ");
                    if (isBST) bst_root = bst_insert(bst_root, b);
                    else avl_root = avl_insert(avl_root, b);
                    std::cout << "Buku berhasil ditambahkan.\n";
                } else if (op == 2) {
                    std::string title;
                    readLine("Masukkan judul buku yang ingin dihapus: ", title);
                    bool removed = false;
                    if (isBST) bst_root = bst_remove(bst_root, title, removed);
                    else avl_root = avl_remove(avl_root, title, removed);
                    if (removed) std::cout << "Buku dihapus.\n";
                    else std::cout << "Buku tidak ditemukan.\n";
                } else if (op == 3) {
                    std::string title;
                    readLine("Masukkan judul buku yang dicari: ", title);
                    if (isBST) {
                        BSTNode* res = bst_search(bst_root, title);
                        if (res) std::cout << "Ditemukan: [" << res->data.id << "] " << res->data.title << " - " << res->data.author << " (" << res->data.year << ")\n";
                        else std::cout << "Tidak ditemukan.\n";
                    } else {
                        AVLNode* res = avl_search(avl_root, title);
                        if (res) std::cout << "Ditemukan: [" << res->data.id << "] " << res->data.title << " - " << res->data.author << " (" << res->data.year << ")\n";
                        else std::cout << "Tidak ditemukan.\n";
                    }
                } else if (op == 4) {
                    std::cout << "\n-- Daftar buku (in-order) --\n";
                    if (isBST) bst_inorder_print(bst_root);
                    else avl_inorder_print(avl_root);
                } else if (op == 5) {
                    std::string fname;
                    readLine("Masukkan nama file (contoh: data_buku.txt): ", fname);
                    if (isBST) bst_export_to_file(bst_root, fname);
                    else avl_export_to_file(avl_root, fname);
                    std::cout << "Ekspor selesai -> " << fname << "\n";
                } else {
                    std::cout << "Pilihan tidak valid.\n";
                }
            }
        } else {
            std::cout << "Pilihan tidak valid.\n";
        }
    }

    bst_free(bst_root);
    avl_free(avl_root);
    std::cout << "Keluar. Terima kasih.\n";
    return 0;
}