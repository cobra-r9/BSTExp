# Introduction

Binary Search Tree or simply BST is a Binary Tree program that operates with a set of rules.

- Each node has at max two child nodes
- The values of the left subtrees are lesser than the current node
- The values of the right subtrees are greater than the current node
- And these must hold recursively at every node, not just the root node

## Structure of a Node

We define each node as a data structure, which has the following :

- A value — `int` or whatever, say a `data` to represent with an integer
- A pointer to the left child node
- A pointer to the right child node

```
[ data | *left | *right ]
```

---

## Example

```
        50
       /  \
      30   70
     / \   / \
    20  40 60  80
```

- 50 is the **root** — no parent
- 20, 40, 60, 80 are **leaves** — no children
- Every node on the left of 50 is lesser than 50
- Every node on the right of 50 is greater than 50
- This holds at every node — at 30, its left (20) is lesser, its right (40) is greater

---

## Advantage

At each level, we eliminate half the tree — that's where the O(log n) comes from. Only true on a balanced tree though. A degenerate BST becomes a linked list with O(n).

| Operation | Average Case | Worst Case (degenerate) |
|-----------|-------------|------------------------|
| Search    | O(log n)    | O(n)                   |
| Insert    | O(log n)    | O(n)                   |
| Delete    | O(log n)    | O(n)                   |

---

## Three Core Operations

- **Insertion** — place a new value in the correct position
- **Search** — find whether a value exists
- **Delete** — remove a value while keeping the BST property intact

All of which are executed starting from a node — usually the root.

---

## Degenerate Case

This happens when the sequential numbers provided are already sorted. The tree degenerates into a linked list instead of spreading out into a proper tree.

```
Insert : 10, 20, 30, 40, 50

10
  \
   20
     \
      30
        \
         40
           \
            50
```

Every node only has a right child. You get no branching, no halving — just a straight chain. Search becomes O(n) because you traverse every node one by one, just like a linked list.

---

# Node Initialisation Logic

Before any operation, we need to create a node. A node is just a chunk of memory holding three things : its value, a pointer to the left child, and a pointer to the right child.

## How it works

1. Allocate memory for a new node
2. Assign the given value to `data`
3. Set `left` to NULL — no left child yet
4. Set `right` to NULL — no right child yet
5. Return the newly created node

## Logic

```
create_node(value):
    node = allocate memory for a Node
    node.data  = value
    node.left  = NULL
    node.right = NULL
    return node
```

## Why NULL matters

Setting both pointers to NULL is not optional. Without it, both pointers hold garbage values — random memory addresses left over from whatever was there before. Any traversal or operation that checks `if node == NULL` would then behave unpredictably, reading garbage memory as if it were a real node.

NULL is the sentinel. It is what every traversal checks to know it has hit a dead end. Every node starts as a leaf — no children, both pointers explicitly NULL.

---

# Find Logic

Finding a value means walking the tree using the BST property as your guide. At every node, the BST property tells you exactly which direction to go. You never guess, you never backtrack.

## The Core Idea

At every node, ask one question :

- Value equals current node → **found it**
- Value is less than current node → **go left**
- Value is greater than current node → **go right**
- Hit NULL → **not in the tree**

---

## Recursive Model

### How it works

The function calls itself on a smaller subtree at each step. Each call reduces the problem — instead of searching the whole tree, you're now searching just the left or right subtree. The base cases stop the recursion.

**Base case 1 :** Node is NULL. You've walked off the tree. Value doesn't exist. Return NULL.

**Base case 2 :** Node's data equals value. You found it. Return the node.

**Recursive case :** Value is less → recurse into left subtree. Value is greater → recurse into right subtree.

### Logic

```
find(node, value):
    if node is NULL       → return NULL          // not found
    if value == node.data → return node          // found
    if value < node.data  → return find(node.left,  value)
    if value > node.data  → return find(node.right, value)
```

### Trace : find 60

```
        50
       /  \
      30   70
     / \   / \
    20  40 60  80
```

```
find(50, 60)
  → 60 > 50, recurse right : find(70, 60)
      → 60 < 70, recurse left : find(60, 60)
          → 60 == 60 → return node(60)
      ← returns node(60)
  ← returns node(60)
```

### Trace : find 55 (not in tree)

```
find(50, 55)
  → 55 > 50, recurse right : find(70, 55)
      → 55 < 70, recurse left : find(60, 55)
          → 55 < 60, recurse left : find(NULL, 55)
              → node is NULL → return NULL
          ← returns NULL
      ← returns NULL
  ← returns NULL
```

### What's happening under the hood

Each recursive call pushes a new frame onto the call stack. The call stack holds the state of each call — which node it was at, which direction it went. When a base case is hit, the return value bubbles back up through every frame. For a balanced tree of n nodes, the maximum depth of recursion is O(log n). For a degenerate tree, it's O(n) — and with a large enough tree, that risks a stack overflow.

---

## Iterative Model

### How it works

Instead of the call stack managing state, you manage it yourself with a single pointer. Start at root. Walk down. Move the pointer left or right based on the comparison. Stop when you find the value or fall off the tree.

No recursion. No call stack growth. Safe on any depth of tree.

### Logic

```
find(root, value):
    current = root

    while current is not NULL:
        if value == current.data → return current     // found
        if value <  current.data → current = current.left
        if value >  current.data → current = current.right

    return NULL                                        // not found
```

### Trace : find 40

```
        50
       /  \
      30   70
     / \
    20  40
```

| Step | current | comparison        |
|------|---------|-------------------|
| 1    | 50      | 40 < 50 → go left |
| 2    | 30      | 40 > 30 → go right|
| 3    | 40      | 40 == 40 → found  |

### Why prefer iterative

- No function call overhead
- No risk of stack overflow on deep or degenerate trees
- Easier to reason about memory behaviour in C
- In practice, iterative find is what you'd use in production C code

---

# Insert Logic

Insertion follows the exact same path as find. The only difference : instead of returning when you hit NULL, you place the new node there.

## The Core Idea

Walk down the tree using the BST property. The moment you hit a NULL pointer — that gap is exactly where the new node belongs. Attach it there.

---

## Recursive Model

### How it works

The function calls itself on a subtree until it hits NULL. At NULL, it returns a newly created node. On the way back up, every parent reassigns its left or right pointer to what was returned. This is the key mechanism — the return value rewires the tree.

**Base case :** Node is NULL. Create a new node and return it. This returned node is what gets wired into the tree by the parent call.

**Recursive case :** Value is less → recurse left, reassign `node.left` to result. Value is greater → recurse right, reassign `node.right` to result. Return current node unchanged.

### Logic

```
insert(node, value):
    if node is NULL → return create_node(value)     // base case : new node born here

    if value < node.data → node.left  = insert(node.left,  value)
    if value > node.data → node.right = insert(node.right, value)
    // if value == node.data → duplicate, do nothing

    return node                                      // return self, unchanged
```

### Trace : insert 45

```
        50
       /  \
      30   70
     / \
    20  40
```

```
insert(50, 45)
  → 45 < 50 : 50.left = insert(30, 45)
      → 45 > 30 : 30.right = insert(40, 45)
          → 45 > 40 : 40.right = insert(NULL, 45)
              → NULL hit → return create_node(45)
          ← 40.right = node(45), return node(40)
      ← 30.right = node(40), return node(30)
  ← 50.left  = node(30), return node(50)
```

Result :

```
        50
       /  \
      30   70
     / \
    20  40
           \
           45
```

### Why every node returns itself

Each call does `node.left = insert(node.left, value)` and then `return node`. Even if nothing changed in the subtree, the node returns itself. This means the parent's pointer always gets correctly reassigned — whether a new node was created deep below or not. The entire path from root to insertion point gets its pointers explicitly rewired on the way back up.

---

## Iterative Model

### How it works

Recursive insert uses the call stack to remember the parent implicitly. Iterative insert has no call stack — so you track the parent yourself with an explicit pointer. Walk down with two pointers : `current` to probe ahead, `parent` to remember where you were. When `current` hits NULL, `parent` is the node you attach to.

**Key difference from recursive :** You create the node first, then find where to attach it. In recursive, the node is created at the bottom of the call chain and wired in on the way back up.

### Logic

```
insert(root, value):
    new_node = create_node(value)

    if root is NULL → return new_node               // empty tree, new node is root

    current = root
    parent  = NULL

    while current is not NULL:
        parent  = current                           // remember current before moving
        if value < current.data → current = current.left
        if value > current.data → current = current.right
        if value == current.data → return root      // duplicate, ignore

    // loop ends : parent is the node to attach to, current is NULL
    if value < parent.data → parent.left  = new_node
    else                   → parent.right = new_node

    return root
```

### Trace : insert 45

```
        50
       /  \
      30   70
     / \
    20  40
```

| Step | current | parent | comparison         |
|------|---------|--------|--------------------|
| 1    | 50      | NULL   | 45 < 50 → go left  |
| 2    | 30      | 50     | 45 > 30 → go right |
| 3    | 40      | 30     | 45 > 40 → go right |
| 4    | NULL    | 40     | loop exits         |

`45 > 40` → `parent.right = new_node(45)`

### Why you must track parent

When `current` becomes NULL, you've found the gap. But NULL gives you nothing — you can't set `NULL.left`. You need the node just above that NULL, the one whose pointer you'll update. That's `parent`. Without it, you'd know *where* in the tree the node belongs conceptually, but you'd have no pointer to actually attach it.

---

# Inorder Traversal

**Rule : Left → Root → Right**

Applied recursively at every node. The defining property : inorder traversal of a BST always produces values in **sorted ascending order**. This is a direct consequence of the BST property.

---

## Recursive Model

### How it works

At every node, recurse into the left subtree completely first, then process the current node, then recurse into the right subtree. The base case is NULL — do nothing and return.

The print happens sandwiched between the two recursive calls. That sandwich position is what makes it inorder — you process a node only after everything to its left has been processed.

### Logic

```
inorder(node):
    if node is NULL → return                   // base case

    inorder(node.left)                         // go all the way left first
    print(node.data)                           // process current
    inorder(node.right)                        // then go right
```

### Trace

```
        50
       /  \
      30   70
     / \   / \
    20  40 60  80
```

```
inorder(50)
  → inorder(30)
      → inorder(20)
          → inorder(NULL) return
          → print 20
          → inorder(NULL) return
      → print 30
      → inorder(40)
          → inorder(NULL) return
          → print 40
          → inorder(NULL) return
  → print 50
  → inorder(70)
      → inorder(60)
          → inorder(NULL) return
          → print 60
          → inorder(NULL) return
      → print 70
      → inorder(80)
          → inorder(NULL) return
          → print 80
          → inorder(NULL) return
```

Output : `20  30  40  50  60  70  80` — sorted. ✓

---

## Iterative Model

### How it works

Recursive inorder uses the call stack to remember where it left off at each node. Iteratively, you simulate this with an explicit stack. The pattern : push every left node until you hit NULL. Pop. Print. Then try to go right and repeat.

The stack is doing what the call stack did implicitly — holding all the nodes you've visited on the way down left, waiting to be printed once you've fully processed everything below them.

### Logic

```
inorder_iterative(root):
    stack   = empty stack
    current = root

    while current is not NULL OR stack is not empty:

        while current is not NULL:          // go left as far as possible
            push current onto stack
            current = current.left

        current = pop from stack            // back up to the last unprocessed node
        print(current.data)                 // process it
        current = current.right             // now try its right subtree
```

### Trace

```
        50
       /  \
      30   70
     / \
    20  40
```

```
current=50 → push 50, go left
current=30 → push 30, go left
current=20 → push 20, go left
current=NULL → inner while exits

pop 20 → print 20 → current = 20.right = NULL

inner while skipped (current is NULL)
pop 30 → print 30 → current = 30.right = 40

current=40 → push 40, go left
current=NULL → inner while exits

pop 40 → print 40 → current = 40.right = NULL

inner while skipped
pop 50 → print 50 → current = 50.right = 70

current=70 → push 70, go left
current=NULL → inner while exits

pop 70 → print 70 → current = 70.right = NULL

stack empty, current NULL → outer while exits
```

Output : `20  30  40  50  70` ✓

### Why the outer while has two conditions

`while current is not NULL OR stack is not empty` — both conditions matter.

- `current is not NULL` : there's still tree ahead to push onto the stack
- `stack is not empty` : there are still nodes waiting to be printed

If you only checked one, you'd exit early. When you pop and go right, `current` might briefly be NULL (no right child) while the stack still has nodes above. The OR keeps you going until both are exhausted.

---

# Postorder Traversal

**Rule : Left → Right → Root**

The current node is processed **last** — only after both its subtrees have been fully processed. This makes postorder the natural choice for operations where a node depends on its children being handled first, like freeing memory. You free children before the parent, never the other way around.

---

## Recursive Model

### How it works

At every node, recurse left completely, then recurse right completely, then process the current node. The base case is NULL — return immediately.

The print comes after both recursive calls. You touch a node only once everything below it is done.

### Logic

```
postorder(node):
    if node is NULL → return                   // base case

    postorder(node.left)                       // process entire left subtree
    postorder(node.right)                      // process entire right subtree
    print(node.data)                           // then process current
```

### Trace

```
        50
       /  \
      30   70
     / \   / \
    20  40 60  80
```

```
postorder(50)
  → postorder(30)
      → postorder(20)
          → postorder(NULL) return
          → postorder(NULL) return
          → print 20
      → postorder(40)
          → postorder(NULL) return
          → postorder(NULL) return
          → print 40
      → print 30
  → postorder(70)
      → postorder(60)
          → postorder(NULL) return
          → postorder(NULL) return
          → print 60
      → postorder(80)
          → postorder(NULL) return
          → postorder(NULL) return
          → print 80
      → print 70
  → print 50
```

Output : `20  40  30  60  80  70  50`

Root is always last. Every parent comes after its children.

---

## Iterative Model

### How it works

Postorder is the hardest traversal to do iteratively. The problem : you visit a node twice — once going down, and once coming back up after both subtrees are done. You need a way to know whether you're visiting a node for the first time (still need to process children) or the second time (children done, now process it).

The clean solution : use **two stacks**.

**Stack 1** drives the traversal — it behaves like a modified preorder (Root → Right → Left).
**Stack 2** collects the output in reverse.

Push root onto stack 1. While stack 1 is not empty : pop a node, push it onto stack 2, then push its left child, then its right child onto stack 1. When stack 1 is empty, pop everything from stack 2 — that's your postorder output.

### Why two stacks work

Normal preorder visits : Root → Left → Right.
If you flip it to : Root → Right → Left, and then reverse the whole output, you get : Left → Right → Root — which is exactly postorder.

Stack 2 is the reversal mechanism. You push into it during traversal, pop from it at the end.

### Logic

```
postorder_iterative(root):
    if root is NULL → return

    stack1 = empty stack
    stack2 = empty stack

    push root onto stack1

    while stack1 is not empty:
        node = pop from stack1
        push node onto stack2

        if node.left  is not NULL → push node.left  onto stack1
        if node.right is not NULL → push node.right onto stack1

    while stack2 is not empty:
        node = pop from stack2
        print(node.data)
```

### Trace

```
        50
       /  \
      30   70
     / \
    20  40
```

**Phase 1 — fill stack2 :**

| stack1 action     | stack1 state | node popped | stack2 state         |
|-------------------|-------------|-------------|----------------------|
| push 50           | [50]        | —           | []                   |
| pop 50, push L,R  | [30, 70]    | 50          | [50]                 |
| pop 70, push L,R  | [30]        | 70          | [50, 70]             |
| pop 30, push L,R  | [20, 40]    | 30          | [50, 70, 30]         |
| pop 40, no child  | [20]        | 40          | [50, 70, 30, 40]     |
| pop 20, no child  | []          | 20          | [50, 70, 30, 40, 20] |

**Phase 2 — pop stack2 :**

`20  40  30  70  50`

Output : `20  40  30  70  50` ✓

### Why push left before right onto stack1

Stack is LIFO. You want right to be popped before left in the Root→Right→Left traversal. So you push left first (it'll be popped later) and right second (it'll be popped sooner). Pushing left before right means right gets processed first — which is what you want.

