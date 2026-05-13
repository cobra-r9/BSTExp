
## Introduction

>[!NOTE]
> **For running the program :**
> Use `make run SRC="./src/filename"`
> where, you can run each program in the src directory by specifing it's name in place of filename. 

### What is a BST?

A BST is a special type of binary tree that maintains elements in sorted order. For every node: all nodes in its left subtree have values less than the node's value, and all nodes in the right subtree have values greater. This property ensures each comparison allows skipping about half the remaining tree, making BST operations much faster than linear structures like arrays or linked lists. [geeksforgeeks](https://www.geeksforgeeks.org/dsa/introduction-to-binary-search-tree/)

**Key properties:**
- Duplicates typically not allowed (unique keys)
- Inorder traversal of a BST gives sorted order of elements [geeksforgeeks](https://www.geeksforgeeks.org/dsa/introduction-to-binary-search-tree/)
- Operations like search, insertion, and deletion work in O(log n) for a balanced BST. In the worst case (unbalanced/skewed), these degrade to O(n). [share](https://share.google/VPD3k5yGF8d5Bs53b)
- Average height: O(log n); worst case height: O(n) when tree becomes skewed [geeksforgeeks](https://www.geeksforgeeks.org/dsa/introduction-to-binary-search-tree/)


### Operations

**Search** — O(h) time
Start at the root. Compare the key with the current node: if equal, done. If smaller, go left. If greater, go right. Repeat until found or null is hit — return false if null. [geeksforgeeks](https://www.geeksforgeeks.org/dsa/binary-search-tree-set-1-search-and-insertion/)

Two variants: recursive (O(h) space due to call stack) and iterative (O(1) space, preferred).

**Insertion** — O(h) time
A new key is inserted at the position that maintains the BST property. Start from the root and move downward: if the key is smaller, go left; if larger, go right. Continue until finding an unoccupied spot and insert the new node as a leaf. [geeksforgeeks](https://www.geeksforgeeks.org/dsa/insertion-in-binary-search-tree/)

Recursive variant returns the modified root; iterative variant uses a `curr` pointer to walk down and then attaches the new node. Both are O(h) time; iterative is O(1) space vs O(h) recursive stack.

**Deletion** — O(h) time
Three cases: node has no children → remove directly. Node has one child → replace node with its child. Node has two children → replace node with inorder successor/predecessor, then delete that successor/predecessor. [geeksforgeeks](https://www.geeksforgeeks.org/dsa/introduction-to-binary-search-tree/)

**Traversals:**
Inorder (Left, Root, Right) gives sorted order. Preorder (Root, Left, Right), Postorder (Left, Right, Root), and Level-order traverses level by level using a queue. [geeksforgeeks](https://www.geeksforgeeks.org/dsa/introduction-to-binary-search-tree/)


### Applications

A Self-Balancing BST is used to maintain a sorted stream of live data — for example, online orders sorted by price, enabling queries like "count items cheaper than X" at any moment. [geeksforgeeks](https://www.geeksforgeeks.org/dsa/applications-of-bst/)

A Self-Balancing BST implements a doubly-ended priority queue. A Binary Heap supports either extractMin or extractMax; if you need both, a Self-Balancing BST does both in O(log n). [geeksforgeeks](https://www.geeksforgeeks.org/dsa/applications-of-bst/)

A BST can sort a large dataset by inserting elements and doing an inorder traversal. The advantage over sorting algorithms: you can later insert/delete in O(log n) time. [geeksforgeeks](https://www.geeksforgeeks.org/dsa/applications-of-bst/)

Variations like B-Tree and B+ Tree are used in database indexing. TreeMap and TreeSet in Java, and `set` and `map` in C++ are internally implemented using self-balancing BSTs — specifically a Red-Black Tree. [geeksforgeeks](https://www.geeksforgeeks.org/dsa/applications-of-bst/)


### Self-Balancing BSTs

To keep height O(log n), self-balancing BSTs like AVL and Red-Black Trees are used in practice, ensuring all operations remain O(log n) instead of degrading to O(n) in skewed cases. [geeksforgeeks](https://www.geeksforgeeks.org/dsa/applications-of-bst/)


### Problem Categories (from the hub page of GFG (geeksforgeeks))

The GFG BST hub organizes problems into three tiers:

**Easy:** Second largest element, Sum of k smallest, BST keys in range, Check for BST, Convert Binary Tree to BST, Sorted Array to Balanced BST.

**Medium:** BST from Preorder, LCA in BST, k-th smallest in BST, Largest BST Subtree, Fix a BST with 2 swapped nodes, 2-Sum in BST.

**Hard:** All possible BSTs for 1..N, Merge two BSTs with limited space, K-th largest without modifying BST, 3-Sum in BST, Pairs with sum from two BSTs.

**Without wasting any further time, let's start with the creation of a node.**

## Creating a Node in C — Core Logic

I am not providing any actual C code here in this write up. It is just a theory - referencing the geek for geeks DSL course on the BST. Just a part of my C learning pathway. If you need a C code explanation, it would be under the explanations directory (or) directly read the inline comments of the code.

### What a node actually is in memory

In C, a node is just a **contiguous block of memory** holding three things:

- An integer (the data/key) : But I say, I have an idea. Instead of giving it the integer, we create a data structure (some struct), each struct has a unique integer field and some data. It can either be a pointer or the struct itself embedded in the Node. But for basics, we start with integer itself. 
- A pointer to another block of the same type (left child)
- A pointer to another block of the same type (right child)

The two pointers don't hold other nodes — they hold **addresses** of other nodes. The nodes themselves live somewhere in the heap. The pointers just connect them.


### The self-referential struct problem

Here's the conceptual challenge: to define a node, you need to say "it contains a pointer to a node." But you're in the middle of defining what a node is. This is a **self-referential struct**.

C resolves this with the **struct tag**. When you write `struct Node`, even before the struct is fully defined, C knows that `struct Node` is a type that will exist. So a pointer to it — `struct Node*` — is valid to declare inside its own definition, because a pointer is always the same size (8 bytes on 64-bit), regardless of what it points to. C doesn't need to know the full size of `struct Node` to hold a pointer to one.

This is why you must use the **struct tag form** (`struct Node*`), not a typedef alias, inside the struct body — the alias doesn't exist yet at that point.


### What malloc actually does here

When you create a node, you're asking the OS (via `malloc`) to give you a raw chunk of bytes on the heap — exactly `sizeof(struct Node)` bytes. At this moment that memory is **uninitialized garbage**. It has no meaning yet.

Your job after `malloc` is to impose structure on those bytes:

- Write the integer value into the data field's offset
- Write `NULL` (which is just `0` in memory) into the left pointer's offset
- Write `NULL` into the right pointer's offset

Only after that does it become a valid, well-formed node. Before that, those pointer fields contain random bytes — if anything ever tried to dereference them, it would follow a garbage address into undefined behavior.


### Why both pointers must be NULL

A freshly created node is always a **leaf** — it has no children yet. In C, "no child" is represented by a null pointer. If you forget to initialize either pointer, that field contains whatever bytes happened to be at that memory address. To the BST traversal logic, a non-NULL pointer means "there's a child here, go follow it" — so garbage bytes in that field would cause the traversal to walk off into random memory. Setting both to NULL is what makes the node safe to insert.


### What the function returns and why

The function returns a **pointer** to the node, not the node itself. This is fundamental to C:

- The node lives on the heap — it survives after the function returns
- If you returned the struct by value, you'd get a copy on the stack, and the heap allocation would be orphaned (memory leak)
- The pointer is small (8 bytes) and cheap to pass around
- Every other BST operation (insert, search, delete) will receive this pointer and use it to reach the actual node

The returned pointer becomes the handle. Whoever calls the function owns that pointer and is responsible for eventually freeing it.


### The malloc failure case

`malloc` can return `NULL` if the system is out of memory. In production C code, you must check for this before touching the pointer. If you skip the check and dereference a `NULL` return from `malloc`, that's a segfault. In competitive/learning code this check is usually skipped, but the logic of why it exists is: heap allocation is a syscall, and syscalls can fail. You can notify yourself with a stderr or use perror, printf or whatever to tell you on the runtime - malloc failed, the node is not created. Though there are several other cases in which malloc fails, here it may be mostly because the heap mem ran out of space - "which is unlikely in high rammed pcs and laps - 16 gb" and you use relatively less ram.

>[!IMPORTANT]
> Next moving on to the operations, seach, insert and delete and traversals, we have it in two ways : way 1 : recursion, way 2 : iteration. Recursion is ok if the tree is balanced and has less height. But in case of million datas, worst case O(n) which means, it can run out of time, buffer overflow or UB, trying to recurse 1 million times. Iteration on the other hand uses a pointer to manage the state of current node being processed. As a result, we don't recurse deeply.

## Recursive Search in BST — Core Logic

Here, implementation of the BST in recursive manner. 

### The Fundamental Insight

A BST's ordering property means at every single node, you get **a binary decision that eliminates half the remaining tree**. You never need to search both subtrees. The structure itself tells you where the answer must be.

This is exactly what makes recursion the natural fit — each recursive call is just "solve the same problem on a strictly smaller tree."

### The Three Questions at Every Node

When you arrive at any node during search, you ask exactly three questions in order:

**Question 1: Am I at NULL?**
If yes — the key doesn't exist in this BST. You've walked off the edge of the tree. Return false. This is the base case.

**Question 2: Is the key equal to this node's data?**
If yes — found it. Return true. This is the success base case.

**Question 3: Which direction do I go?**
If the key is less than current node's data — the key, if it exists, must be in the left subtree. Recurse left.
If the key is greater than current node's data — the key, if it exists, must be in the right subtree. Recurse right.

These three questions are the complete logic. Nothing else.


### Textual Pseudocode

```
search(node, key):

    if node is NULL:
        return FALSE                  ← fell off the tree, not found

    if key == node.data:
        return TRUE                   ← exact match, found

    if key < node.data:
        return search(node.left, key) ← must be in left subtree

    if key > node.data:
        return search(node.right, key) ← must be in right subtree
```

Notice: every path either hits NULL (false) or hits a match (true). There is no path that loops or gets stuck. The tree's finite height guarantees termination.


### Example Tree

```
            10
           /  \
          5    20
         / \   / \
        3   7 15  30
```


### Walkthrough 1 — Searching for 7 (KEY EXISTS)

**Call 1: search(node=10, key=7)**
- Node is not NULL ✓
- 7 == 10? No.
- 7 < 10? Yes → go left
- Recurse into left subtree rooted at 5

**Call 2: search(node=5, key=7)**
- Node is not NULL ✓
- 7 == 5? No.
- 7 < 5? No.
- 7 > 5? Yes → go right
- Recurse into right subtree rooted at 7

**Call 3: search(node=7, key=7)**
- Node is not NULL ✓
- 7 == 7? YES → return TRUE

TRUE bubbles back through Call 2, then Call 1. Final answer: **found**.

Total nodes visited: 3 out of 7. The left subtree of 5 (which contains 3) was never touched. The entire right half of the tree (20, 15, 30) was eliminated at the very first step.


### Walkthrough 2 — Searching for 6 (KEY DOES NOT EXIST)

**Call 1: search(node=10, key=6)**
- 6 < 10 → go left

**Call 2: search(node=5, key=6)**
- 6 > 5 → go right

**Call 3: search(node=7, key=6)**
- 6 < 7 → go left

**Call 4: search(node=NULL, key=6)**
- Node IS NULL → return FALSE

FALSE bubbles back up through all three prior calls. Final answer: **not found**.

This is the critical case to understand — you don't just "check all nodes near where 6 should be." You walk the exact path where 6 would live if it existed, and when you fall off the tree at a NULL, you know with certainty it isn't there.


### Why Recursion is the Natural Model Here

Each recursive call is operating on a **strictly smaller tree**:

```
search(root)          → tree of height h
  search(root.left)   → tree of height h-1
    search(...)       → tree of height h-2
      ...
        search(NULL)  → empty tree, base case
```

The problem shrinks by exactly one level per call. You are guaranteed to hit a base case because the tree has finite height. This is what makes the recursion correct and terminating — no infinite loops are possible as long as the BST property holds.


### The Call Stack Picture

Every recursive call is a **stack frame** sitting on the call stack, waiting for its child call to return. For Walkthrough 1 (searching 7), the stack at deepest point looks like:

```
search(node=7,  key=7)   ← currently executing, about to return TRUE
search(node=5,  key=7)   ← waiting
search(node=10, key=7)   ← waiting
main()                   ← waiting
```

When search(7) returns TRUE, it hands the result to search(5), which immediately returns it to search(10), which returns it to main. No node does anything with the result except pass it upward unchanged — which is what "return search(node.left, key)" means. The return value propagates up the entire stack untouched.


### Complexity

**Time:** O(h), where h is the height of the tree. In a balanced BST, h = log₂n, so O(log n). In a completely skewed tree (degenerate, looks like a linked list), h = n, so O(n).

**Space:** O(h) for the call stack — one frame per level you descend. This is the hidden cost of recursion that the iterative version eliminates entirely.

## Iterative Search in BST — Core Logic

And, here is the iterative implementation of the BST search algorithm.

### The Core Idea Shift

In the recursive version, the **call stack** was doing the work of remembering where you are. Each frame held the current node implicitly.

In the iterative version, you manage that yourself with a **single pointer** called `curr` (or `current`). That pointer is your position in the tree. You move it manually. No call stack. No frames. Just one pointer walking down.

This is possible because of a key observation: in BST search, **you never go back up**. You only ever move downward — left or right. You never need to return to a parent. That means you need zero memory of where you've been. One pointer is sufficient.


### The Mental Model

Think of it as **following a chain of decisions** until one of two things happens:

- You land on the key → found, stop
- You walk off the tree (curr becomes NULL) → not found, stop

Every iteration of the while loop is identical to one recursive call, except instead of the function calling itself, you just **reassign curr** and loop again.


### Textual Pseudocode

```
search(root, key):

    curr = root                     ← start at the root

    while curr is not NULL:         ← while still inside the tree

        if key == curr.data:
            return TRUE             ← found it, stop immediately

        if key < curr.data:
            curr = curr.left        ← move down-left

        else:
            curr = curr.right       ← move down-right

    return FALSE                    ← curr hit NULL, key not in tree
```

The while condition does the job that the NULL base case did in recursion. The reassignment of curr does the job that the recursive call did. Structurally identical, mechanically different.


### Example Tree

```
            10
           /  \
          5    20
         / \   / \
        3   7 15  30
```


### Walkthrough 1 — Searching for 15 (KEY EXISTS)

**Before loop starts:**
```
curr = 10
```

**Iteration 1:**
- curr is not NULL ✓
- 15 == 10? No.
- 15 < 10? No.
- 15 > 10 → curr = curr.right

```
curr = 20
```

**Iteration 2:**
- curr is not NULL ✓
- 15 == 20? No.
- 15 < 20? Yes → curr = curr.left

```
curr = 15
```

**Iteration 3:**
- curr is not NULL ✓
- 15 == 15? YES → return TRUE

Done. Three iterations, found it.


### Walkthrough 2 — Searching for 6 (KEY DOES NOT EXIST)

**Before loop starts:**
```
curr = 10
```

**Iteration 1:**
- 6 < 10 → curr = curr.left

```
curr = 5
```

**Iteration 2:**
- 6 > 5 → curr = curr.right

```
curr = 7
```

**Iteration 3:**
- 6 < 7 → curr = curr.left

```
curr = NULL   ← 7's left child doesn't exist
```

**Iteration 4 — while condition check:**
- curr IS NULL → condition fails → exit loop

Return FALSE.


### What's Happening to curr Over Time

This is the key picture to hold in your head. curr is just a pointer sliding down one path of the tree:

```
Search for 6:

10          ← curr here on iteration 1
 \
  (eliminated — 6 < 10, went left)
 /
5           ← curr here on iteration 2
 \
  (eliminated — 6 > 5, went right)
   \
    7       ← curr here on iteration 3
   /
  NULL      ← curr here on iteration 4, loop exits
```

curr never goes sideways. It never backtracks. It falls straight down one branch, and either hits the answer or falls off.


### The Eliminated Subtrees

Same tree, search for 6. Every node NOT visited:

```
            10
           /  \
          5    20  ← entire right half eliminated at step 1
         / \   / \
        3   7 15  30
        ↑
        never visited — eliminated at step 2
```

Node 3 was never touched. Nodes 20, 15, 30 were never touched. Only the path 10 → 5 → 7 → NULL was walked.


### Side-by-Side: Recursive vs Iterative

| Aspect | Recursive | Iterative |
|---|---|---|
| Position tracking | Implicit via call stack frames | Explicit via `curr` pointer |
| "Move down" mechanism | Function calls itself with child node | `curr = curr.left/right` |
| Base case (not found) | `if node == NULL return false` | While condition fails when curr is NULL |
| Base case (found) | `if node.data == key return true` | `if curr.data == key return true` inside loop |
| Space complexity | O(h) call stack | O(1) — just one pointer |
| Time complexity | O(h) | O(h) — identical |

The logic is **exactly the same**. The iterative version just makes the mechanics explicit instead of hiding them in the call stack.


### Why O(1) Space is Significant

The recursive version pushes one stack frame per level descended. For a balanced tree of 1 million nodes, that's ~20 frames. Harmless. But for a fully skewed tree (worst case), that's 1 million frames — potential stack overflow. The iterative version uses one pointer regardless of tree shape or size. That's why in production BST code, iterative search is always preferred.


### The Exit Invariant

After the while loop exits (either via return or condition failure), this is always true:

- If you returned TRUE inside the loop — curr was pointing at the exact node holding the key
- If you exited because curr became NULL — the key genuinely does not exist in the BST, because you walked the only possible path where it could have been

There's no ambiguity. The while loop has exactly two exits and both are conclusive.

## Recursive Insertion in BST — Core Logic

Now, we are into our first operation, which modifies the BST tree structure. Not inherentily restructuring it completely, but just attaching to a node. Each node inserted is a leaf node, ie it does not have any child. 

### How Insertion Differs From Search Fundamentally

Search is **read-only**. You walk down, return true or false, and the tree is unchanged.

Insertion has to **modify the tree**. Specifically, it has to attach a new node somewhere. This changes what the recursion needs to do — it can't just return a boolean. It needs to return something that allows the tree to be **structurally rewired** as the call stack unwinds.

This is the central idea that makes recursive insertion more subtle than recursive search.


### Where Does a New Node Go?

First, the key insight about placement: **a new node in a BST always becomes a leaf**. Always. You never insert into the middle of the tree. You never shuffle existing nodes around. You follow the exact same decision path as search, and wherever search would have fallen off into NULL — that NULL slot is exactly where the new node goes.

Think of it this way: if you searched for the key you're about to insert, you'd walk down some path and eventually hit NULL (since it doesn't exist yet). That NULL is a **vacancy**. Insertion fills that vacancy with the new node.


### The Problem With Just Walking Down

In search, when you hit NULL you just return false. You don't need to do anything to the tree.

In insertion, when you hit NULL you need to:
- Create a new node
- **Attach it to its parent's left or right pointer**

The problem is: when you're at NULL, you've already moved past the parent. You don't have a reference to the parent anymore. You only have the NULL pointer — which is a value, not a location. You can't modify the parent's child pointer from inside the NULL call.

This is the exact problem that the return-and-rewire pattern solves.


### The Return-and-Rewire Pattern

Instead of trying to modify the parent from the child call, you flip the approach:

Every recursive call **returns the root of the subtree it just operated on**. The parent call then **assigns that return value back into its own child pointer**.

In normal cases (not the insertion point), nothing changes — the node just returns itself. But at the insertion point (the NULL slot), the call returns the **newly created node** instead of NULL. The parent call, receiving this return value and assigning it to `node.left` or `node.right`, effectively **stitches the new node into the tree**.

The rewiring happens automatically as the call stack unwinds. No parent pointer needed. No special handling at the parent level. Every call does the same thing: return whatever this subtree's root is, and let the parent wire it in.


### Textual Pseudocode

```
insert(node, key):

    if node is NULL:
        create a new node with the key     ← found the vacancy
        return new node                    ← hand it up to parent

    if key < node.data:
        node.left = insert(node.left, key) ← rewire left child

    else if key > node.data:
        node.right = insert(node.right, key) ← rewire right child

    return node                            ← return self (unchanged)
```

The line `node.left = insert(node.left, key)` is the entire mechanism. Unpack it:

- `insert(node.left, key)` — recurse into left subtree
- Whatever comes back (either the existing subtree root, or the new node) — assign it back to `node.left`

In 99% of calls, `insert(node.left, key)` returns the same node that was already there, so `node.left = node.left` — no change. But at the one call that hits NULL, it returns the new node, and `node.left = new_node` stitches it in.


### Example Tree — Insert 6

Starting tree:
```
            10
           /  \
          5    20
         / \
        3   7
```

We call `insert(root, 6)`. Root is the node holding 10.


**Call 1: insert(node=10, key=6)**
- node is not NULL ✓
- 6 < 10 → go left
- Execute: `node.left = insert(node.left=5, key=6)`
- Paused. Waiting for the recursive call to return.

```
Call stack:
insert(10, 6)  ← PAUSED at "node.left = ..."
```


**Call 2: insert(node=5, key=6)**
- node is not NULL ✓
- 6 > 5 → go right
- Execute: `node.right = insert(node.right=7, key=6)`
- Paused. Waiting.

```
Call stack:
insert(5,  6)  ← PAUSED at "node.right = ..."
insert(10, 6)  ← PAUSED at "node.left = ..."
```


**Call 3: insert(node=7, key=6)**
- node is not NULL ✓
- 6 < 7 → go left
- Execute: `node.left = insert(node.left=NULL, key=6)`
- Paused. Waiting.

```
Call stack:
insert(7,  6)  ← PAUSED at "node.left = ..."
insert(5,  6)  ← PAUSED at "node.right = ..."
insert(10, 6)  ← PAUSED at "node.left = ..."
```


**Call 4: insert(node=NULL, key=6)**
- node IS NULL → this is the vacancy
- Create new node with data=6, left=NULL, right=NULL
- Return the new node

```
Call stack unwinds now.
```


**Call 3 resumes: insert(node=7, key=6)**
- Received the new node (6) from the recursive call
- Executes: `node.left = new_node(6)`
- 7's left child is now 6 ✓
- Returns node 7 (itself, unchanged root of this subtree)


**Call 2 resumes: insert(node=5, key=6)**
- Received node 7 back from the recursive call
- Executes: `node.right = 7`
- 5's right child is still 7 — no actual change
- Returns node 5


**Call 1 resumes: insert(node=10, key=6)**
- Received node 5 back
- Executes: `node.left = 5`
- 10's left child is still 5 — no actual change
- Returns node 10


### Final Tree

```
            10
           /  \
          5    20
         / \
        3   7
           /
          6    ← inserted here
```

6 landed exactly where search for 6 would have fallen off — the left child of 7. Which was NULL. Which is now filled.


### The Unwinding Picture

The actual structural change happened at exactly one point — Call 3 assigning 6 as the left child of 7. Everything above that was just passing the existing nodes back to themselves, unchanged.

```
Call 4 → returns new_node(6)
  Call 3 → wires 6 as 7.left → returns 7
    Call 2 → receives 7, sets 5.right = 7 (same) → returns 5
      Call 1 → receives 5, sets 10.left = 5 (same) → returns 10
        main → receives 10, sets root = 10 (same)
```

Only one real modification happens. All other assignments are identity assignments. But because every call does `node.child = insert(...)`, the pattern is uniform — no special casing needed anywhere.


### Why Return node At The End

Every non-NULL call must return itself at the end. If it didn't, the parent's assignment `node.left = insert(...)` would receive NULL (or garbage), and the parent would lose its entire subtree. The `return node` at the bottom is what preserves every existing subtree that wasn't modified.


### What The Caller Must Do

The caller — typically `main` — must do:

```
root = insert(root, key)
```

Not just `insert(root, key)`. Because if the tree is **empty** (root is NULL), the insert call returns the brand new node, and that must be captured as the new root. If you discard the return value, root stays NULL and the inserted node is orphaned on the heap — a memory leak, and an empty tree from the caller's perspective.

Even when the tree isn't empty, root = insert(root, key) is safe — it just assigns root back to itself.


### Complexity

**Time:** O(h) — you descend exactly one root-to-leaf path, same as search.

**Space:** O(h) — one call stack frame per level descended. For a balanced tree that's O(log n). For a skewed tree, O(n) — and this is where recursive insertion can stack overflow on degenerate input, which is why self-balancing BSTs exist.

## Iterative Insertion in BST — Core Logic

Here is the iterative implementation of the BST insertion logic. 

### The Core Problem Iterative Insertion Must Solve

In recursive insertion, the return-and-rewire pattern handled the parent pointer problem automatically. As the stack unwound, each call assigned the return value back into its own child pointer — elegant, implicit.

Iterative insertion cannot do this. There is no call stack to unwind. There is no automatic rewiring. When you walk down to the insertion point using a while loop, you arrive at NULL — and from NULL you cannot reach back up to the parent.

So iterative insertion must solve this **explicitly**: you have to remember the parent yourself, while walking down. By the time you reach the insertion point, you need two things in hand — the vacant slot (NULL) and the parent node that owns that slot. Then you directly assign the new node into the correct child pointer of that parent.

This is the entire structural difference between the two approaches.


### Two Pointers Instead of One

Iterative search used one pointer (`curr`) because you never needed to go back up.

Iterative insertion needs **two pointers** walking in tandem:

- `curr` — the node you are currently examining, used to make left/right decisions
- `parent` — the node you were at one step ago, the one whose child pointer you'll eventually modify

They move together, one step at a time, with `parent` always trailing exactly one level behind `curr`. When `curr` finally hits NULL, `parent` is sitting right at the node that needs to adopt the new child.


### How the Two Pointers Move

At every step:

```
Before moving:
    parent = curr
    
Decide direction:
    if key < curr.data → curr = curr.left
    if key > curr.data → curr = curr.right
```

`parent` captures `curr` before `curr` steps forward. So when `curr` becomes NULL, `parent` holds the last real node — the future parent of the new node.


### Which Child to Attach To

When curr hits NULL, you know the new node belongs under `parent`. But which side — left or right?

You need one more comparison at the end:

```
if key < parent.data → parent.left = new_node
else                 → parent.right = new_node
```

You're re-doing the last comparison that caused `curr` to step into NULL. This tells you which side of the parent the new node belongs on.


### Textual Pseudocode

```
insert(root, key):

    create new_node with key, left=NULL, right=NULL

    if root is NULL:
        return new_node              ← empty tree, new node becomes root

    curr = root
    parent = NULL

    while curr is not NULL:          ← walk until falling off the tree
        parent = curr                ← trail one step behind

        if key < curr.data:
            curr = curr.left         ← step left
        else if key > curr.data:
            curr = curr.right        ← step right

    ← curr is NULL here, parent holds the insertion point's owner

    if key < parent.data:
        parent.left = new_node       ← attach as left child
    else:
        parent.right = new_node      ← attach as right child

    return root                      ← root never changes (unless empty)
```


### Example Tree — Insert 6

Starting tree:
```
            10
           /  \
          5    20
         / \
        3   7
```

Call `insert(root, 6)`.

New node created: node(6), left=NULL, right=NULL.
Tree is not empty, so no early return.

```
Initial state:
curr   = 10
parent = NULL
```


**Iteration 1:**
```
parent = curr       → parent = 10
6 < 10              → curr = curr.left = 5

State:
curr   = 5
parent = 10
```


**Iteration 2:**
```
parent = curr       → parent = 5
6 > 5               → curr = curr.right = 7

State:
curr   = 7
parent = 5
```


**Iteration 3:**
```
parent = curr       → parent = 7
6 < 7               → curr = curr.left = NULL

State:
curr   = NULL
parent = 7
```


**While condition check:**
- curr IS NULL → exit loop


**Attach new node:**

```
6 < parent.data (7)?  Yes
→ parent.left = new_node(6)
```

7's left child is now node(6). Done.

Return root (node 10) — unchanged.


### State Trace As a Table

| Iteration | parent | curr | Decision |
|---|---|---|---|
| Start | NULL | 10 | — |
| 1 | 10 | 5 | 6 < 10, go left |
| 2 | 5 | 7 | 6 > 5, go right |
| 3 | 7 | NULL | 6 < 7, go left → fell off |
| Exit | 7 | NULL | attach 6 as 7.left |


### Final Tree

```
            10
           /  \
          5    20
         / \
        3   7
           /
          6    ← inserted here
```

Identical result to the recursive version. Different mechanism entirely.


### The Empty Tree Edge Case

Before the while loop begins, you must handle the empty tree separately:

```
if root is NULL:
    return new_node
```

Why separately? Because if root is NULL, the while loop never executes, and `parent` remains NULL. Then when you try to do `parent.left = new_node`, you're dereferencing NULL — segfault. The empty tree case is the only time the root itself changes, so it must be caught before anything else.


### Side-by-Side: Recursive vs Iterative Insertion

| Aspect | Recursive | Iterative |
|---|---|---|
| Parent tracking | Implicit — call stack holds it | Explicit — `parent` pointer |
| Attachment mechanism | Return value rewires child pointer on unwind | Direct assignment after loop exits |
| Empty tree handling | NULL base case returns new node naturally | Must check before loop, separately |
| Caller responsibility | `root = insert(root, key)` always | `root = insert(root, key)` for empty tree case |
| Space | O(h) call stack | O(1) — just two pointers |
| Time | O(h) | O(h) — identical |
| Stack overflow risk | Yes, on skewed trees | None |


### Why The Root Never Changes (Except Empty Tree)

In the recursive version, every call did `return node` at the end, propagating the root back up. In the iterative version, you just `return root` at the end — the same pointer you started with. Because insertion always adds a new leaf somewhere deep in the tree. The root node itself is never moved, replaced, or reassigned. Its position is fixed. Only some descendant's NULL child pointer gets filled.

The only exception: empty tree. Root was NULL, and now it should point to the new node. That's the one case where the root pointer actually changes, which is why the caller must always do `root = insert(root, key)` and not just `insert(root, key)`.


### The Fundamental Trade-off

Recursive insertion: the mechanism is elegant and compact — the return-and-rewire pattern handles everything implicitly. But you're paying O(h) stack space and risking overflow on degenerate input.

Iterative insertion: more mechanical — you're doing explicitly what the call stack was doing implicitly. Two pointers, a final comparison, a direct assignment. But O(1) space, no stack risk, and for very deep trees, meaningfully faster in practice due to no function call overhead per level.

## Recursive Free in BST — Core Logic

This should be mandatory before finishing a program. 
As we are creating the structrue in heap memory, we are required to free it. If it is not freed after use, it stays in memory no matter what, then we have a memory leak. 

### What "Freeing" Means Here

Every node in your BST was created with `malloc` — meaning it lives on the heap. The heap does not manage itself. When your program is done with the tree, every single node must be explicitly passed to `free()`. If you don't, the memory stays allocated until the OS reclaims it at process exit. That's a memory leak.

Freeing the BST means visiting every node and calling `free()` on it. The question is purely about **order** — which nodes do you free first, and why does that order matter enormously.


### Why Order Is Critical

This is the core constraint that governs everything:

**You cannot free a node before you free its children.**

If you free a node first, you lose the pointers stored inside it — `node->left` and `node->right`. Those are the only handles you have to reach the children. Once the node is freed, that memory is returned to the allocator and may be overwritten at any moment. Reading `node->left` after `free(node)` is undefined behavior. The children become permanently unreachable — orphaned on the heap forever.

So the rule is: **children must be freed before their parent**. Always. Without exception.


### Which Traversal Satisfies This Rule

There are three classic recursive traversals:

**Preorder — Root, Left, Right**
Visits the root first. Violates the rule immediately — you'd free the parent before its children. Wrong.

**Inorder — Left, Root, Right**
Visits left subtree, then root, then right subtree. Frees the root in the middle — right subtree is still attached and unreachable after. Wrong.

**Postorder — Left, Right, Root**
Visits left subtree fully, then right subtree fully, then the root. By the time you reach the root, both subtrees are already completely freed. Root is now a standalone node with no live children. Safe to free. **This is the correct traversal.**

Postorder is the only traversal where a node is visited strictly after all of its descendants.


### The Mental Model

Think of it as the tree being dismantled from the bottom up. You never touch a node until the entire subtree hanging below it has already been demolished. The leaves go first. Then their parents become leaves. Then those get freed. This continues upward until the root itself, which is the very last node freed.

```
         31              ← freed last
        /  \
       25   35           ← freed after their children
      /       \
     7         40        ← freed after their children
    /             \
   1               91    ← freed first (leaves)
```

Actual free order for this tree: 1, 7, 25, 91, 40, 35, 31.


### Textual Pseudocode

```
free_tree(node):

    if node is NULL:
        return              ← base case, nothing to free

    free_tree(node.left)    ← free entire left subtree first
    free_tree(node.right)   ← free entire right subtree next
    free(node)              ← now safe to free this node
```

Three lines of actual logic. The simplest of all BST operations — but the order of those three lines is everything. Swap any two and you get undefined behavior.


### Walkthrough — Freeing the Example Tree

```
         31
        /  \
       25   35
      /       \
     7         40
    /             \
   1               91
```

**Call 1: free_tree(31)**
- Not NULL
- Recurse left → free_tree(25)
- Paused.

**Call 2: free_tree(25)**
- Not NULL
- Recurse left → free_tree(7)
- Paused.

**Call 3: free_tree(7)**
- Not NULL
- Recurse left → free_tree(1)
- Paused.

**Call 4: free_tree(1)**
- Not NULL
- Recurse left → free_tree(NULL) → returns immediately
- Recurse right → free_tree(NULL) → returns immediately
- free(1) ← **first node freed**
- Returns.

**Call 3 resumes: free_tree(7)**
- Left subtree done
- Recurse right → free_tree(NULL) → returns immediately
- free(7) ← **second node freed**
- Returns.

**Call 2 resumes: free_tree(25)**
- Left subtree done
- Recurse right → free_tree(NULL) → returns immediately
- free(25) ← **third node freed**
- Returns.

**Call 1 resumes: free_tree(31)**
- Left subtree done
- Recurse right → free_tree(35)
- Paused.

**Call 5: free_tree(35)**
- Not NULL
- Recurse left → free_tree(NULL) → returns immediately
- Recurse right → free_tree(40)
- Paused.

**Call 6: free_tree(40)**
- Not NULL
- Recurse left → free_tree(NULL) → returns immediately
- Recurse right → free_tree(91)
- Paused.

**Call 7: free_tree(91)**
- Not NULL
- Recurse left → free_tree(NULL) → returns immediately
- Recurse right → free_tree(NULL) → returns immediately
- free(91) ← **fourth node freed**
- Returns.

**Call 6 resumes: free_tree(40)**
- free(40) ← **fifth node freed**
- Returns.

**Call 5 resumes: free_tree(35)**
- free(35) ← **sixth node freed**
- Returns.

**Call 1 resumes: free_tree(31)**
- Both subtrees done
- free(31) ← **last node freed, root**
- Returns.


### The Call Stack Shape

At the deepest point (while freeing node 1), the stack looked like:

```
free_tree(1)   ← executing
free_tree(7)   ← waiting
free_tree(25)  ← waiting
free_tree(31)  ← waiting
main()         ← waiting
```

The stack depth equals the height of the tree — O(h). Same space complexity as search and insert.


### What Happens After free_tree Returns

The nodes are gone. But the `root` pointer in `main` still holds the old address — now pointing at freed memory. This is a **dangling pointer**. Reading or writing through it is undefined behavior.

The caller must set root to nullptr immediately after:

```
free_tree(root)
root = nullptr
```

This is not optional hygiene — it's mandatory. A dangling pointer that looks valid is more dangerous than a null pointer, because dereferencing null segfaults immediately and loudly. Dereferencing a dangling pointer silently reads garbage or corrupts whatever the allocator put there next — harder to debug, more catastrophic.


### Why This Can't Be Done Iteratively Without Extra Memory

Recursive free is naturally elegant because the call stack implicitly tracks where you are in the tree. If you tried to do this iteratively, you'd need an explicit stack data structure (or a queue) to keep track of nodes to visit — because unlike search and insert which only go downward in one direction, postorder traversal needs to visit both subtrees and then come back to the parent. You can't do that with just a `curr` pointer. The iterative version requires O(h) explicit stack space anyway, so recursion is the natural and preferred approach here.


### Complexity

**Time:** O(n) — every node is visited exactly once, no node is skipped.

**Space:** O(h) call stack — O(log n) balanced, O(n) skewed.


### Summary of the Rule

| Traversal | Order | Safe for free? |
|---|---|---|
| Preorder | Root → Left → Right | No — frees parent before children |
| Inorder | Left → Root → Right | No — frees root before right subtree |
| Postorder | Left → Right → Root | Yes — children always freed before parent |

Postorder is the only valid choice. The logic is three lines. The constraint is one rule: children before parent.

Though I hate the traversal stuffs, it is essential in the bst tree, which we should learn before doing the delete_node function. 
