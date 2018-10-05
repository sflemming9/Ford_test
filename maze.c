/* 
 * Handles major functionality for the Maze problem posed by Ford.
 *
 * High level requirements: 
 * 1. Maze must have a solution
 * 2. Maze must have a start and end
 */

/*  Headers */
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "node.h"

/* Macros */
/* 
 * Checks if a bit has been set. Returns true if it has, false if it hasn't*/
#define CHECK_BIT(var, pos) (var & (1 << pos)) 

/* Global variables */
static struct Node* maze_ptr; // Stores all the nodes that comprise the maze
static unsigned int rows = 0;
static unsigned int columns = 0;

/* Function prototypes */
bool validateInputs(int argc, char* argv[]);
int maze_init();
void print_maze();
void node_init(unsigned int row, unsigned int column);
bool is_seed(unsigned int row, unsigned int column);
void dfs();
bool equals(struct Node n1, struct Node n2);
bool get_offsets(unsigned int* row_offset, unsigned int* col_offset, unsigned int rand_val,
        struct Node curr_node);
int midpoint(int a, int b);


/* Handles running and displaying the maze problem
 *
 * Author: Sabrina Flemming
 * TODO: Potential improvements
 * - Take flags from command line that modify execution (eg: higher amount of logging)
 */
int main(int argc, char* argv[]) {

    // Validate inputs for correct number of arguments, convertability, 0, negative, and size
    if(!validateInputs(argc, argv)) return -1;

    // Initialize rows and columns; atoi is safe because we handled checking our inputs above
    rows = atoi(argv[1]);
    columns = atoi(argv[2]);

    // Initialize the maze
    if (maze_init(rows, columns)) return -1;

    // Apply a Depth First Search to generate a maze from initialized data
    dfs();

    print_maze();

    return 0;
}

/* Handles validating inputs from the command line.  Returns true if invalid input is encountered.
 *
 * TODO: Potential improvements
 * - Provide warnings for non int/long style data types
 * - Handle for non standard input/mixed data types 
 * - Use better function than atoi to perform conversion
 */
bool validateInputs(int argc, char* argv[]) {
    // Check for number of arguments
    if(argc != 3) {
        printf("Please pass 2 command line arguments\n");
        return true;
    }

    // Convert numbers
    int rows = atoi(argv[1]);
    int columns = atoi(argv[2]);

    // Validate conversion produced valid values
    if(rows <=0 || columns <=0) {
        printf("Invalid input, please input two positive and even numbers");
        return false;
    }

    if((rows % 2) == 0 || (columns % 2) == 0) {
        printf("Invlaid input, please input an odd number");
        return false;
    }

    return true;
}

/* Initializes the maze, and all the nodes in it
 */
int maze_init() {

    // Allocate memory for storing nodes
    maze_ptr = (struct Node*)malloc((rows * columns) * sizeof(struct Node));

    // Ensure malloc call was sucessful. Return -1 if it was unsuccessful
    if (maze_ptr == NULL) return -1;

    // Create maze to treat maze_ptr as a multi dimensional array
    struct Node (*maze)[columns] = (struct Node (*)[columns])(maze_ptr);

    // Initialize all Nodes in the maze array
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < columns; c++)  {
            node_init(r, c);
        }
    }

    // Return success
    return 0;
}

/*
 *  This function prints out the maze. A wall is represented by a '|' and a path is represented
 *  by a space.
 */
void print_maze() {
    // Create maze to treat maze_ptr as a multi dimensional array
    struct Node (*maze)[columns] = (struct Node (*)[columns])(maze_ptr);

    // Iterate through maze and print the character associated with is_path
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < columns; c++)  {
            if(maze[r][c].is_path) {
                // Print path character
                printf(" ");
            } else {
                // Print wall character
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
    
    // Create maze to treat maze_ptr as a multi dimensional array
    struct Node (*maze)[columns] = (struct Node (*)[columns])(maze_ptr);

    // Check if a Node is a seed, and assign values accordingly
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
    struct Node (*maze)[columns] = (struct Node (*)[columns])(maze_ptr);

    // Declare start
    struct Node start = maze[1][1];     // Start node will always be in location (1, 1)
    struct Node curr_node = start;

    // Seed time for rand function
    srand(time(NULL));

    do {

        while(curr_node.bit_directs){

            // Randomly choose a direction (up, down, right, left)
            unsigned int rand_val = rand() % 4;     // Random value between 0 and 3


            // If this direction explored
            if (CHECK_BIT(curr_node.bit_directs, rand_val)) {

            } else {
                // This direction has not been explored 
                //
                // Set as explored
                //
                // Check for path
                //      if path: connect nodes
                //      else: curr_node = curr_node.parent
            }
        }
    } while (!equals(curr_node, start));




    // Randomly choose start
    // While end != start
    //      Check to see if path can be made between two nodes separated by an adjacent node
    //      If path can be made, connect the two nodes and assign end = node that was connected to
    //      else assign end = node that came before the current node to backtrack

}

/* 
 * Checks for locational equality between nodes
 * */
bool equals(struct Node n1, struct Node n2) {
    return n1.row == n2.row && n1.col == n2.col;
}
