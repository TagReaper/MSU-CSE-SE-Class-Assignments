#include "bst.h"

struct Node *makeNode(char x[20]) {
  struct Node *tmp = NULL;
  tmp = (struct Node *)malloc(sizeof(struct Node));

  if (tmp == NULL) {
    return NULL;
  }

  strcpy(tmp->x,x);
  tmp->left = NULL;
  tmp->right = NULL;

  return tmp;
};

struct BST *makeBST() {
  struct BST *tmp = NULL;

  tmp = (struct BST *)malloc(sizeof(struct BST));

  if (tmp == NULL) {
        return NULL;
  }

  tmp->root = NULL;
  return tmp;
};

int rInsert(struct Node **node, char x[20]) {
  if (*node == NULL) {
    *node = makeNode(x);
    return 1;
  }
  if (x < (*node)->x) {
    return rInsert(&((*node)->left), x);
  }
  if (x > (*node)->x) {
    return rInsert(&((*node)->right), x);
  }
  return 0;
};

int insert(struct BST *b, char x[20]) {
  if (b == NULL) {
    return 0;
  }
  return rInsert(&(b->root), x);
};

int rSearch(struct Node **node, char x[20]) {
  if (*node == NULL){
    return 0;
  }
  if (x < (*node)->x) {
    return rSearch(&((*node)->left), x);
  }
  if (x < (*node)->x) {
    return rSearch(&((*node)->right), x);
  }
  return 1;
};

int search(struct BST *b, char x[20]) {
  if (b == NULL) {
    return 0;
  }
  return rSearch(&(b->root), x);
};

int rPostOrder(struct Node **node) {
  if (*node == NULL) {
    return 0;
  }
  rPostOrder(&((*node)->left));
  rPostOrder(&((*node)->right));
  printf("%s ", (*node)->x);
  return 1;
}

int postOrder(struct BST *b) {
  if (b == NULL) {
    return 0;
  }
  rPostOrder(&(b->root));
  return 1;
}

int rDealloc(struct Node **node) {
  if (*node == NULL) {
    return 0;
  }
  rDealloc(&((*node)->left));
  rDealloc(&((*node)->right));
  free(*node);
  *node = NULL;
  return 1;
}

int dealloc(struct BST *b) {
  if (b == NULL) {
    return 0;
  }
  rDealloc(&(b->root));
  return 1;
}

int rCopy(struct Node **node, struct BST *b) {
  if (*node == NULL) {
    return 0;
  }
  insert(b, (*node)->x);
  rCopy(&((*node)->left), b);
  rCopy(&((*node)->right), b);
  return 1;
}

int copy(struct BST *b) {
  if (b == NULL) {
    return 0;
  }
  rCopy(&(b->root), b);
  return 1;
}