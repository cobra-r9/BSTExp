// file : bst_recursive.c 
// date : 12-05-2026
// desc : implement a working model of BST (Binary Search Tree) : recursive function calls, step by step. 
// c standard : c23


//==========================================[HEADER FILES]
// stdio : standard input output lib 
#include <stdio.h> 
// stdlib : standard C library for free, malloc 
#include <stdlib.h> 


//==========================================[PROTOTYPING and STRUCTS]
// define the structure of a node here 
// we use typedef to create a Node type. 
typedef struct [[gnu::packed]] Node {
  // a int field for value in this node. 
  int value; 
  // a struct node pointer to left node.
  struct Node *left;
  // a struct node pointer to right node. 
  struct Node *right;
  // define the typedef type name and use it __attribute__((packed)) : [[gnu::packed]] is the c23 equivalent.
} Node;

// prototype the create node function 
// we need this function to take a value and return a pointer to node after creating in the heap memory.
// we use malloc for this purpose. 
Node *create_node(int value);

//==========================================[MAIN FUNCTION]
// in c23, we no longer need to use void argument passeers. () means just void. 
// but I will use it as a convenient. a habit. 
int main(void) {
  // initialise the function. 
  // printf("Initialised program : BST..\n"); // Instead of this, optimise for puts 
  // note that puts by defaults adds a newline character at the end.
  puts("Initialised program : BST Recursive Model");
  // now that we need to initialise our first root node with some value; we have decent 31 as a value.
  Node *root = create_node(31);
  printf("Initialised root node : value = %d\n", root->value);
}


//==========================================[FUNCTION DEFINITIONS]

// define the create node function which takes a int value and returns a pointer to node in heap mem. 
Node *create_node(int value) {
  // create a node pointer. c23 supports typeof to find type of a variable or function. type cast is not required in c23, but it is a good practice to make it obvious. malloc returns a void * pointer type. we allocate in the heap memory : size of Node amount of space and returns a pointer. 
  Node *node = (typeof(node))malloc(sizeof(Node));
  // if the node is NULL, happens when malloc failed to allocate space, !NULL = 1. 1 implies true. so if true, the following block executes. 
  if (!node) {
    // there exists a nullptr type like in c++ in c23 also. set the node to nullptr. 
    node = nullptr;//NULL
    // optionally, warn to stderr or stdout that malloc failed. 
    printf("WARN: malloc failed to allocate to node\n");
    // this return is essential. because if it does not exist, the remaining code block executes, setting the node->left and node-> right values, which are essentially dereferencing the nullptr -> segfault, UB. 
    return node;
  }

  // but initially, we should never forget to set the both left and right pointers to nullptr
  node->right = nullptr;
  node->left = nullptr;
  // set the value for the node. 
  node->value = value;

  // finally, time to return the pointer to the node. 
  return node;

}
