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
  //initialise the function. 
  //printf("Initialised program : BST..\n"); // Instead of this, optimise for puts 
  puts("Initialised program : BST ... \n");
}


//==========================================[FUNCTION DEFINITIONS]
