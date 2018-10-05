#include <stdio.h>
#include <stdlib.h>
#include "node.h"

/* Handles running and displaying the maze problem
 *
 * Author: Sabrina Flemming
 * */
int main(int argc, char *argv[]) {
   
    // TODO Validate inputs for correct number of arguments, convertability, 0, negative, 
    // and size
    
    // Initialize rows and columns; atoi is safe because we handled checking our inputs above
    unsigned int rows = atoi(argv[1]);
    unsigned int columns = atoi(argv[2]);

    printf("%d %d\n", rows, columns);
    return 0;
}
