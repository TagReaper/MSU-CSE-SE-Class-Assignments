#include "bst.h"

struct Node *makeNode(int x) {
  struct Node *tmp = NULL;
  tmp = (struct Node *)malloc(sizeof(struct Node));

  if (tmp == NULL) {
    return NULL;
  }

  tmp->x = x;
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

int rInsert(struct Node **node, int x) {
  if (*node == NULL) {
    *node = makeNode(x);
    return 1;
  }
  if (x < (*node)->x) {
    return rInsert(&((*node)->left), x);
  };
  if (x > (*node)->x) {
    return rInsert(&((*node)->left), x);
  };
};

int insert(struct BST *b, int x) {
  if (b == NULL) {
    return 0;
  }
  return rInsert(&(b->root), x);
}