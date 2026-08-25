#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Node {
  int x;
  struct Node *left;
  struct Node *right;
};

struct BST {
  struct Node *root;
}; 
