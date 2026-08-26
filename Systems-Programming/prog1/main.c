#include "bst.c"
#include <stdio.h>
#include <string.h>

// main function
int main(){
    //assigning variables
    struct BST *b = makeBST();
    char ans[20];

    // Introduction text
    printf("Please enter a series of words to store in a BST\n");
    printf("Type 'STOP' to end the series and output the BST in postorder\n");

    // Runs until "Stop" is detected
    while (1) {
      //Request an input
      printf("Word = ");
      scanf("%19s", ans);

      // If input is "Stop" then the loop breaks
      if (!strcmp("STOP", ans)) {
        break;
      }

      // Inserts the value into the BST
      insert(b, ans);
    }

    // prints the BST in postorder
    printf("The BST output in postorder is \n");
    postOrder(b);

    dealloc(b);
}