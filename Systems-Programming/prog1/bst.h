#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Node {
  char x[20];
  struct Node *left;
  struct Node *right;
};

struct BST {
  struct Node *root;
}; 
