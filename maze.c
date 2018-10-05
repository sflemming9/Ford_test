#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "node.h"

/* Global variables */
static struct Node* maze_ptr;
static unsigned int rows = 0;
static unsigned int columns = 0;

/* Function prototypes */
int maze_init();
void print_maze();
void node_init(unsigned int row, unsigned int column);
bool is_seed(unsigned int row, unsigned int column);

/* Handles running and displaying the maze problem
 *
 * Author: Sabrina Flemming
 * */
int main(int argc, char *argv[]) {

    // TODO Validate inputs for correct number of arguments, convertability, 0, negative,
    // and size

    // Initialize rows and columns; atoi is safe because we handled checking our inputs above
    rows = atoi(argv[1]);
    columns = atoi(argv[2]);

    // Initialize the maze
    if (maze_init(rows, columns)) return -1;

    print_maze(rows, columns);
    printf("%d %d\n", rows, columns);

    // Apply a Depth First Search to generate a maze from initialized data
    dfs();

    return 0;
}

int maze_init() {

    // Allocate memory for storing nodes
    maze_ptr = (struct Node*)malloc((rows * columns) * sizeof(struct Node));

    // Ensure malloc call was sucessful
    if (maze_ptr == NULL) return -1;

    // Create maze to treat maze_ptr as a multi dimensional array
    struct Node (*maze)[columns] = (struct Node (*)[columns])(maze_ptr);

    // For examining an individual node
    struct Node n;

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++)  {
            node_init(i, j);
        }
    }
    return 0;
}

/*
 *  This function prints out the maze. A wall is represented by a '|' and a path is represented
 *  by a space.
 */
void print_maze() {

    struct Node (*maze)[columns] = (struct Node (*)[columns])(maze_ptr);

    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < columns; j++)  {
            if(maze[i][j].is_path) {
                printf("Z");
            } else {
                printf("|");
            }
        }
        printf("\n");
    }
}

/*
 * This function initializes a node with the appropriate value.
 */
void node_init(unsigned int row, unsigned int column) {
    struct Node (*maze)[columns] = (struct Node (*)[columns])(maze_ptr);
    if (is_seed(row, column)) {
        maze[row][column].row = row;
        maze[row][column].col = column;
        maze[row][column].is_path = 1;
        maze[row][column].bit_directs = 15;
    } else {
        maze[row][column].is_path = 0;
    }
}

/*
 *  A seed node is a node located on an odd row and odd column. It is necessary to seed the
 *  matrix in this manner so that a DFS can be performed without creating large open blocks in the
 *  maze, and ensuring that a solution can always be found for a given start and end.
 */
bool is_seed(unsigned int row, unsigned int column) {
    return ((row % 2) && (column % 2));
}

void dfs() {
    // Declare start and end
    // Randomly choose start
    // While end != start
    //      Check to see if path can be made between two nodes separated by an adjacent node
    //      If path can be made, connect the two nodes and assign end = node that was connected to
    //      else assign end = node that came before the current node to backtrack

}
