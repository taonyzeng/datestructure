// RedblackTreeChatGPT.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>

#include <stdio.h>
#include <stdlib.h>

using namespace std;

// Define the colors for Red-Black Tree
typedef enum { RED, BLACK } Color;

// Node structure
typedef struct Node {
    int data;
    int sum_left;
    Color color;
    struct Node *left, *right, *parent;
} Node;

// Red-Black Tree structure
typedef struct RBTree {
    Node *root;
} RBTree;

// Function prototypes
Node* createNode(int data, Color color, Node* left, Node* right, Node* parent);
RBTree* createTree();
void leftRotate(RBTree *tree, Node *x);
void rightRotate(RBTree *tree, Node *x);
void insertFixup(RBTree *tree, Node *z);
void insert(RBTree *tree, int data);
Node* search(RBTree *tree, int data);
void transplant(RBTree *tree, Node *u, Node *v);
Node* minimum(Node *node);
void deleteFixup(RBTree *tree, Node *x);
void removeNode(RBTree *tree, Node *z);
void deleteNode(RBTree *tree, int data);
void inorder(Node *node);
void destroyTree(Node *node);

Node* createNode(int data, Color color, Node* left, Node* right, Node* parent) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = data;
    newNode->color = color;
    newNode->left = left;
    newNode->right = right;
    newNode->parent = parent;
    newNode->sum_left = 0;
    return newNode;
}

RBTree* createTree() {
    RBTree* tree = (RBTree*)malloc(sizeof(RBTree));
    tree->root = NULL;
    return tree;
}

int getSumSubTree(Node* root) {
    if (root == NULL) {
        return 0;
    }

    return root->data + root->sum_left + getSumSubTree(root->right);

}

void leftRotate(RBTree *tree, Node *x) {
    Node *y = x->right;
    x->right = y->left;
    if (y->left != NULL) y->left->parent = x;
    y->parent = x->parent;
    if (x->parent == NULL) tree->root = y;
    else if (x == x->parent->left) x->parent->left = y;
    else x->parent->right = y;
    y->left = x;
    x->parent = y;
    y->sum_left += x->data + x->sum_left;
}

void rightRotate(RBTree *tree, Node *x) {
    Node *y = x->left;
    x->left = y->right;

    int new_sum_left = 0;
    if (y->right != NULL) {
        y->right->parent = x;
        new_sum_left = getSumSubTree(y->right);
    }
    x->sum_left = new_sum_left;
    y->parent = x->parent;
    if (x->parent == NULL) tree->root = y;
    else if (x == x->parent->right) x->parent->right = y;
    else x->parent->left = y;
    y->right = x;
    x->parent = y;
}

void insertFixup(RBTree *tree, Node *z) {
    while (z->parent != NULL && z->parent->color == RED) {
        if (z->parent == z->parent->parent->left) {
            Node *y = z->parent->parent->right;
            if (y != NULL && y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->right) {
                    z = z->parent;
                    leftRotate(tree, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                rightRotate(tree, z->parent->parent);
            }
        } else {
            Node *y = z->parent->parent->left;
            if (y != NULL && y->color == RED) {
                z->parent->color = BLACK;
                y->color = BLACK;
                z->parent->parent->color = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->left) {
                    z = z->parent;
                    rightRotate(tree, z);
                }
                z->parent->color = BLACK;
                z->parent->parent->color = RED;
                leftRotate(tree, z->parent->parent);
            }
        }
    }
    tree->root->color = BLACK;
}

void insert(RBTree *tree, int data) {
    Node *z = createNode(data, RED, NULL, NULL, NULL);
    Node *y = NULL;
    Node *x = tree->root;
    while (x != NULL) {
        y = x;
        if (z->data < x->data) {
            x->sum_left += z->data;
            x = x->left;
        }
        else {
            x = x->right;
        }
    }

    z->parent = y;
    if (y == NULL) tree->root = z;
    else if (z->data < y->data) y->left = z;
    else y->right = z;
    insertFixup(tree, z);
}

int getSumLessThan(Node* root, int key){
    if (root == NULL) {
        return 0;
    }
    if (key == root->data) {
        return root->sum_left;
    }
    else if (key < root->data) {
        return getSumLessThan(root->left, key);
    }
    else {
        return  root->data + root->sum_left + getSumLessThan(root->right, key);
    }

}


Node* search(RBTree *tree, int data) {
    Node *current = tree->root;
    while (current != NULL && current->data != data) {
        if (data < current->data) current = current->left;
        else current = current->right;
    }
    return current;
}

void transplant(RBTree *tree, Node *u, Node *v) {
    if (u->parent == NULL) tree->root = v;
    else if (u == u->parent->left) u->parent->left = v;
    else u->parent->right = v;
    if (v != NULL) v->parent = u->parent;
}

Node* minimum(Node *node) {
    while (node->left != NULL) {
        node = node->left;
    }
    return node;
}

void deleteFixup(RBTree *tree, Node *x) {
    while (x != tree->root && (x == NULL || x->color == BLACK)) {

        if (x == NULL){
            break;
        }

        if ( x == x->parent->left) {
            Node *w = x->parent->right;
            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                leftRotate(tree, x->parent);
                w = x->parent->right;
            }
            if ((w->left == NULL || w->left->color == BLACK) && (w->right == NULL || w->right->color == BLACK)) {
                w->color = RED;
                x = x->parent;
            } else {
                if (w->right == NULL || w->right->color == BLACK) {
                    if (w->left != NULL) w->left->color = BLACK;
                    w->color = RED;
                    rightRotate(tree, w);
                    w = x->parent->right;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                if (w->right != NULL) w->right->color = BLACK;
                leftRotate(tree, x->parent);
                x = tree->root;
            }
        } else {
            Node *w = x->parent->left;
            if (w->color == RED) {
                w->color = BLACK;
                x->parent->color = RED;
                rightRotate(tree, x->parent);
                w = x->parent->left;
            }
            if ((w->right == NULL || w->right->color == BLACK) && (w->left == NULL || w->left->color == BLACK)) {
                w->color = RED;
                x = x->parent;
            } else {
                if (w->left == NULL || w->left->color == BLACK) {
                    if (w->right != NULL) w->right->color = BLACK;
                    w->color = RED;
                    leftRotate(tree, w);
                    w = x->parent->left;
                }
                w->color = x->parent->color;
                x->parent->color = BLACK;
                if (w->left != NULL) w->left->color = BLACK;
                rightRotate(tree, x->parent);
                x = tree->root;
            }
        }
    }
    if (x != NULL) x->color = BLACK;
}

void removeNode(RBTree *tree, Node *z) {
    Node *y = z;
    Node *x = NULL;
    Color y_original_color = y->color;

    if ( z->left != NULL && z->right != NULL ) {
        y = minimum(z->right);

        int diff = y->data - z->data;
        Node* cur = z->right;
        while (cur != y){
            cur->sum_left -= diff;
            cur = cur->left;
        }

        int remove_data = z->data;
        z->data = y->data;
        y->data = remove_data;

        removeNode( tree, y );
        return;
    }

    //now the node to be removed has only no children
    //or only one child, update the sum_left value of
    //all the node along the path from root to z.
    Node* current = z;
    Node* p = current->parent;
    while (p != NULL) {
        if (p->left == current)
        {
            p->sum_left -= z->data;
        }
        current = p;
        p = current->parent;
    }

    if (z->left == NULL) {
        x = z->right;
        transplant(tree, z, z->right);

    } else if (z->right == NULL) {
        x = z->left;
        transplant(tree, z, z->left);

    } /*else {
        y = minimum(z->right);
        y_original_color = y->color;
        x = y->right;
        if (y->parent == z) {
            if (x != NULL) x->parent = y;
        } else {
            transplant(tree, y, y->right);
            y->right = z->right;
            z->right->parent = y;
        }
        transplant(tree, z, y);
        y->left = z->left;
        z->left->parent = y;
        y->color = z->color;
    }*/

    free(z);
    if (y_original_color == BLACK) {
        deleteFixup(tree, x);
    }

}

void deleteNode(RBTree *tree, int data) {
    Node *z = search(tree, data);
    if (z != NULL) {
        removeNode(tree, z);
    }
}

void inorder(Node *node) {
    if (node != NULL) {
        inorder(node->left);
        printf("%d ", node->data);
        inorder(node->right);
    }
}

void destroyTree(Node *node) {
    if (node != NULL) {
        destroyTree(node->left);
        destroyTree(node->right);
        free(node);
    }
}

// Helper function to print the tree
void printTree(Node* root, int space) {
    if (root != nullptr) {
        space += 10;
        printTree(root->right, space);
        std::cout << std::endl;
        for (int i = 10; i < space; i++)
            std::cout << " ";
        std::cout << root->data << std::endl;
        printTree(root->left, space);
    }
}


int main() {
    cout << "============Test the program from ChatGPT===============" << endl;

    RBTree *tree = createTree();
    int count = 64;

    int arr[] = {21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1 }; //{ 1, 2, 3, 4, 5, 6, 7, 8, 9 };

    for (int i = count; i >= 1; --i)
        {
        insert( tree, i);
        }

    printTree( tree->root, 0 );

    cout << "===============================================" << endl;
    for (int i = 1; i <= count; i++) {
        printf("Sum of values less than %d: %d\n", i, getSumLessThan(tree->root, i) );
    }

    cout << "============delete the value 10===============" << endl;
    deleteNode(tree, 10);
    printTree( tree->root, 0 );

    for (int i = 1; i <= count; i++) {
        printf("Sum of values less than %d: %d\n", i, getSumLessThan(tree->root, i));
    }

    cout << "============delete the value 6===============" << endl;
    deleteNode(tree, 6);
    printTree( tree->root, 0 );

    for (int i = 1; i <= count; i++) {
        printf("Sum of values less than %d: %d\n", i, getSumLessThan(tree->root, i));
    }

    cout << "============delete the value 5===============" << endl;
    deleteNode(tree, 5);
    printTree( tree->root, 0 );

    for (int i = 1; i <= count; i++) {
        printf("Sum of values less than %d: %d\n", i, getSumLessThan(tree->root, i));
    }


    cout << "============delete the value 8===============" << endl;
    deleteNode(tree, 8);
    printTree( tree->root, 0 );

    for (int i = 1; i <= count; i++) {
        printf("Sum of values less than %d: %d\n", i, getSumLessThan(tree->root, i));
    }

    cout << "============delete the root value 4===============" << endl;
    deleteNode(tree, 4);
    printTree( tree->root, 0 );

    for (int i = 1; i <= count; i++) {
        printf("Sum of values less than %d: %d\n", i, getSumLessThan(tree->root, i));
    }

    cout << "============insert the root value 4===============" << endl;
    insert(tree, 4);
    printTree( tree->root, 0 );

    cout << "============insert the root value 8===============" << endl;
    insert(tree, 8);
    printTree( tree->root, 0 );

    cout << "============insert the root value 5===============" << endl;
    insert(tree, 5);
    printTree( tree->root, 0 );

    for (int i = 1; i <= count; i++) {
        printf("Sum of values less than %d: %d\n", i, getSumLessThan(tree->root, i));
    }

    cout << "============delete the value 1,2,3,4,5,6===============" << endl;
    for (int k = 1; k <= 6; ++k){
        deleteNode(tree, k);
    }

    printTree( tree->root, 0 );

    /*cout << "============insert some new random values: 22, 4, 97, 35, 11, 47===============" << endl;
    int arr2[] = { 22, 4, 97, 35, 11, 47 };
    for ( int i = 0; i < sizeof(arr2) / sizeof(int); ++i ){
        insert( tree, arr2[i]);
    }

    printTree( tree->root, 0 );*/

    destroyTree(tree->root);
    free(tree);

    return 0;
}


// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started:
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
