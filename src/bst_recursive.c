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
typedef struct Node {
  // a int field for value in this node. 
  int value; 
  // a struct node pointer to left node.
  struct Node *left;
  // a struct node pointer to right node. 
  struct Node *right;
} Node;

// prototype the create node function 
// we need this function to take a value and return a pointer to node after creating in the heap memory.
// we use malloc for this purpose. 
Node *create_node(int value);

// prototype the search node function 
// we need this function to take a value, the pointer to the root node, and return a boolean value. 
// boolean? yes, now c23 officially support boolean and what was previously in stdbool.h was merged into stdlib.h, essentially a bool type. previously, we do it even without bool via returning int value 0 false, 1 true and comparing them to evaluate it. 
bool search_node(Node *node, int value);


// prototype the insert node function 
// we use the same criteria as the search node, but instead of returning a bool value, we return the node pointer iteself. 
// we need the function to return a Node * type and take input : a node pointer and int value to insert. 
Node *insert_node(Node *node, int value);


// prototype for the find value function 
// it returns nothing, but prints the statements. 
// it should take a pointer to node and int value to be found. 
void find_value(Node *node, int value);



// prototype for the vind values function 
// instead of taking int value, it takes an array of int values to loop through them, finding the values in array. Alongside with it, it should also take the size of array. 
void find_values(Node *node, int *arr, int size);

// prototype for free_tree function. 
// this deletes entire subtrees from the given point. 
// pseudo code looks like this : 
//free_tree(node):

//    if node is NULL:
//        return              ← base case, nothing to free

//    free_tree(node.left)    ← free entire left subtree first
//    free_tree(node.right)   ← free entire right subtree next
//    free(node)              ← now safe to free this node

// Obviously, we are not going to return anything. 
void free_tree(Node *node);

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

  // now that we need to search for the value 31, which is our first value. 
  // we know that search_node returns a bool type. we can use it directly in if. 
  // ah, lot of if blocks may be messy, so let's do a wrapper function : find_value.
  // if (search_node(root, 31)) {
  //  printf("Value Found! %d is in the BST\n", 31);
  // } else {
  //  printf("Value Not Found! %d is not in the BST\n", 31);
  // }
  find_value(root, 31);

  // now that we have implemented the insert logic, we need to insert a value.
  root = insert_node(root, 35); // greater than 31, will be in right. 
  root = insert_node(root, 25); // lesser than 31, will be in the left. 
  root = insert_node(root, 40);
  root = insert_node(root, 7);
  root = insert_node(root, 91);
  root = insert_node(root, 1);

  find_value(root, 1);
  find_value(root, 7);
  find_value(root, 32);
  find_value(root, 100);
  find_value(root, 40);
  find_value(root, 201);

  int arr[] = {1, 2, 5, 7, 31, 32, 33, 35, 40, 100, 91, 25};
  find_values(root, arr, sizeof(arr)/sizeof(arr[0]));

  // now let us free the left subtree;
  free_tree(root->left);
  root->left = nullptr; // mandatory; else it is a dangling pointer. 
  find_value(root, 25); // will not be found due to deletion of tree. 
  // now that we need to free our tree; put it out of existence and assign nullptr;
  free_tree(root);
  root = nullptr;




}


//==========================================[FUNCTION DEFINITIONS]


//--------------------------------------------------------------------------------------
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

//---------------------------------------------------------------------------------------
// define a search node function which searches the BST using recursive algorigthm as mentioned in the readme.md 
bool search_node(Node *node, int value) {
  // step one : if the node is null, return false 
  if (node == nullptr) return false;
  // step two : if the value is less than node's value, recurse into left subtree.
  if (value < node->value) {
    return search_node(node->left, value);
    // fix 1 : fixed critical error : missing return : if(value < node->value) search_node(node->left, value)
    // step three : if the value is greater than the node's value, recurse into right subtree.
  } else if (value > node->value) {
    return search_node(node->right, value);
    // fix 2 : fixed critical error : missing return : if(value > node->value) search_node(node->right, value)
    // Both, fix 1 and fix 2 : without return, the tree never traverses forward and it always returns true what so ever. 
    // finally return true ; but based on fix 1 and fix 2, it is safer for us to compare value == node->value and return true; else return false. 
  } else if (value == node->value) {
    return true;
  } else {
    return false;
  }
}
// migrated to if else instead of continious if blocks. 

//---------------------------------------------------------------------------------------
// define a insert node function which inserts the BST using recursive algorighm, just subtle from search_node 
Node *insert_node(Node *node, int value) {
  // step one : if the node is null, create the node and return it. IT can be null only if it is end of leaf node; ie it is now becoming the child of a leaf node after creation.
  if (node == nullptr) return create_node(value);
  // step two : if it is not null, then we would have to compare the values of the node to recurse into the left or right subtree. 
  // if the value is less than the node's value, then we recurse into the left subtree. 
  if (value < node->value) {
    node->left = insert_node(node->left, value);
    // never forget to do node->left = assignemnt; because without it, not matter what, the value is never inserted and the tree is never traversed. 
    // if the value is greater than the node's value, then we recurse into the right subtree. 
  } else if (value > node->value) { 
    node->right = insert_node(node->right, value);
    // similarly, never forget to assign node->right = assignemnt for the same reasons. 
    // if the value and node's value are equal, it implies there is a duplicate. We just don't consider the duplicates. But if we want to track duplicates also, then we implement a datastructue in place of int value and count the duplicates for each int value. So instead of node->value, we will be doing something like :
    // node->data.value in case the structure is embedded into the Node itself.
    // node->data->value in case the Node has a pointer to the data structure. (ie, additionally we might need functions to manage the data struture iteself, which I am not going to be implementing in this first example)
  }
  
  // finally, we will be returning the node. As mentioned in readme, the recursive call returns the pointer to node after each call finishes, now that we are in the final call, needing to return the node itself, so that the insert_node in step 2 can function properly. 
  return node; 
}

//---------------------------------------------------------------------------------------
// define a fucntion to free_tree from a given node recursively. 

void free_tree(Node *node) {
  // we free it if it is not null;
  if (node != nullptr) {
    // free the left or right subtree, then free the node. 
    // I choose to free left subtree first 
    free_tree(node->left);
    // then right subtree. 
    free_tree(node->right);
    // finally free the current node. 
    free(node);
  }
}

// it is the responsibility of the caller to set the pointer to nullptr or NULL; whatever. 

// for the AUTO NULLPTR approach, to set each node to null, we would need a different approach. 
// see, when a pointer is passed, it is only a copy of pointer. but deref it, you get the same value because both the original pointer and copied pointer, both points to the same address;
// but if you need to modify the pointer itself, then, we need to pass teh pointer to the pointer;
// ie, istead of passing node (type = Node *); we pass a &node (type = Node **)
// dereferencing this once *(&node) will give us the node (ie the address which the pointer holds)
// dereferencing this twice **(&node) will give us the value of the address the pointer holds, ie the original node in heap;
// so, essentially 
// void free_tree(Node **node) {
//     if (*node != nullptr) {
//         free_tree(&((*node)->left));
//         // this is because (*node)->left is a type Node *;
//         // but free_tree requires Node **, so we reference it again using & to pass it to the function. Same goes below too. 
//         free_tree(&((*node)->right));
//         free(*node);
//         *node = nullptr;
//     }
// }
// node : type Node **
// *node : type Node *
// **node : type Node 
// we free Node * type as expected by free. 
// the free_tree expects Node ** as per definition. 
// each Node type has a left, right whose types are Node *;
// we  (**node).left is having a type of Node *, ie, the left. or right.
// or (*node)->left is having the type Node *
// or &((*node)->left) has type Node **, which is what free_tree expects to be passed. 
// if you follow this reasonling, everything feels understandable. 


// uncomment these stuffs if you want the function to set null instead of caller setting the null; 

//--------------------------------------------------------------------------------------
// the function definition for the wrapper function  :find_value. Nothing to explain, you get if only via if blocks .
void find_value(Node *node, int value) {

  // to make sure than the node provided is valid node, we use a top level if statement to monitor this. 
  if (node == nullptr) {
    puts("WARN: Unable to search in an invalid node");
  } else {
    if (search_node(node, value)) {
      printf("Value Found! %d is in the BST\n", value);
    } else {
      printf("Value Not Found! %d is not in the BST\n", value);
    }
  }
}

//---------------------------------------------------------------------------------------
//the function definition for the wrapper function find_values.
void find_values(Node *node, int *arr, int size) {
  if (node == nullptr || arr == nullptr) {
    puts("WARN: Unable to search in an invalid node (or) array");
  } else {
    for (int i = 0; i < size; i++) find_value(node, arr[i]);
  }
}


