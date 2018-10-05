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
/* Checks if a bit has been set. Returns true if it has, false if it hasn't. */
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
void set_start_end();
bool is_corner(int row, int col);


/*
 * Handles running and displaying the maze problem
 *
 * Author: Sabrina Flemming
 * TODO: Potential improvements
 * - Take flags from command line that modify execution (eg: higher amount of logging)
 */
int main(int argc, char* argv[]) {

    // Validate inputs for correct number of arguments, convertability, 0, negative, and size
    if (!validateInputs(argc, argv)) return -1;

    // Initialize rows and columns; atoi is safe because we handled checking our inputs above
    rows = atoi(argv[1]);
    columns = atoi(argv[2]);

    // Initialize the maze
    if (maze_init(rows, columns)) return -1;

    // Apply a Depth First Search to generate a maze from initialized data
    dfs();

    // Set start and end to maze solution
    set_start_end();

    print_maze();

    return 0;
}

/*
 * Handles validating inputs from the command line.  Returns true if invalid input is encountered.
 *
 * TODO: Potential improvements
 * - Provide warnings for non int/long style data types
 * - Handle for non standard input/mixed data types
 * - Use better function than atoi to perform conversion
 */
bool validateInputs(int argc, char* argv[]) {

    // Check for number of arguments
    if (argc != 3) {
        printf("Please pass 2 command line arguments.\n");
        return false;
    }

    // Convert numbers
    int rows = atoi(argv[1]);
    int columns = atoi(argv[2]);

    // Validate conversion produced valid values
    if (rows <=0 || columns <=0) {
        printf("Invalid input, please input two positive and odd numbers.\n");
        return false;
    }

    if ((rows % 2) == 0 || (columns % 2) == 0) {
        printf("Invlaid input, please input an odd number.\n");
        return false;
    }

    return true;
}

/*
 * Initializes the maze, and all the nodes in it.
 */
int maze_init() {

    // Allocate memory for storing nodes
    // This memory is not freed because it is required until the program completes
    maze_ptr = (struct Node*)malloc((rows * columns) * sizeof(struct Node));

    // Ensure malloc call was sucessful. Return -1 if it was unsuccessful.
    if (maze_ptr == NULL) return -1;

    // Create maze to treat maze_ptr as a multi dimensional array
    struct Node (*maze)[columns] = (struct Node (*)[columns])(maze_ptr);

    // Initialize all Nodes in the maze array
    for (int r = 0; r < rows; r++) {
        for (int c = 0; c < columns; c++)  {
            node_init(r, c);
        }
    }

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
            if (maze[r][c].is_path) {
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
 * This function initializes a node at location row,column with the appropriate value.
 */
void node_init(unsigned int row, unsigned int column) {

    // Create maze to treat maze_ptr as a multi dimensional array
    struct Node (*maze)[columns] = (struct Node (*)[columns])(maze_ptr);

    // Assign the location of the node
    maze[row][column].row = row;
    maze[row][column].col = column;

    // Check if a Node is a seed, and assign values accordingly
    if (is_seed(row, column)) {
        maze[row][column].is_path = 1;
        maze[row][column].bit_directs = 15;     // Initialize all directions as unvisited
    } else {
        maze[row][column].is_path = 0;
        maze[row][column].bit_directs = 15;     // Initialize all directions as visited
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

/*
 * Handles making a Depth First Search through the maze and nodes to determine a path.
 *
 * TODO: Potential improvements
 * - Make dfs take arguments for where you want the dfs to start, and validate those arguments
 * - Keep track of the optimal path for a given start and end coord (assuming they're on the) edges
 */
void dfs() {
    // Create maze to treat maze_ptr as a multi dimensional array
    struct Node (*maze)[columns] = (struct Node (*)[columns])(maze_ptr);

    // Declare start and current nodes
    struct Node *start = &maze[1][1];     // Start node will always be in location (1, 1)
    start->prev = start;
    struct Node *curr_node = start;

    // Seed time for rand function
    srand(time(NULL));

    do {
        // Check for path
        unsigned int* row_offset = malloc(sizeof(unsigned int));
        unsigned int* col_offset= malloc(sizeof(unsigned int));

        // Whle the current node being examined has unexplored directions
        while (curr_node->bit_directs) {

            // Randomly choose a direction (up, down, right, left)
            int rand_val = rand() % 4;     // Random value between 0 and 3

            // Check If this direction explored
            if (!CHECK_BIT(curr_node->bit_directs, rand_val)) continue;

            // This direction has not been explored. Set as explored
            curr_node->bit_directs &= ~(1 << rand_val);

            // Get new row and column coordinates for next node in the direction of exploration
            // If getOfsset encounters a wall (returns false), continue
            if (!get_offsets(row_offset, col_offset, rand_val, *curr_node)) continue;

            // Fetch next node from maze array
            struct Node* next_node = &maze[*row_offset][*col_offset];

            if (next_node->is_path) {

                // If the next node has alreay been added to the path do not process further
                if(next_node->prev != NULL) continue;

                // Set prev of the next node to be the curr_node
                next_node->prev = curr_node;

                // Add connecting node to path
                if(curr_node->row - next_node->row == 0) {
                    // Row is shared between curr_node and next_node
                    int new_col = midpoint(curr_node->col, next_node->col);
                    maze[*row_offset][new_col].is_path = 1;
                } else if (curr_node->col - next_node->col == 0) {
                    // Column is shared between curr_node and next_node
                    int new_row = midpoint(curr_node->row, next_node->row);
                    maze[new_row][*col_offset].is_path = 1;
                }

                // Assign next node to current node and continue to iterate
                curr_node = &maze[*row_offset][*col_offset];
            }
        }
        // Backtrack by looking at the previous node
        curr_node = curr_node->prev;

    // Cleanup created data
    free(row_offset);
    free(col_offset);

    } while (!equals(*curr_node, *start));
}

/*
 *  Handles finding the midpoint between two integers, a and b.
 */
int midpoint(int a, int b) {
    return a + ((b - a) / 2);
}

/*
 * Handles getting offsets from a curr_node based on a rand val. that provides direction of offset.
 * Populates data into row_offset and col_offset.
 */
bool get_offsets(unsigned int* row_offset, unsigned int* col_offset, unsigned int rand_val,
        struct Node curr_node) {

    if (rand_val == 2  && ((curr_node.col - 2) >= 0)) {
        *row_offset = curr_node.row;
        *col_offset = curr_node.col - 2;
    } else if(rand_val == 3 && ((curr_node.row - 2) >= 0)) {
        *row_offset = curr_node.row - 2;
        *col_offset = curr_node.col;
    } else if(rand_val == 0 && ((curr_node.col + 2) < rows)) {
        *row_offset = curr_node.row;
        *col_offset = curr_node.col + 2;
    } else if(rand_val == 1 && ((curr_node.row + 2) < columns)) {
        *row_offset = curr_node.row + 2;
        *col_offset = curr_node.col;
    } else {
        // Return a failure to populate any offsets
        return false;
    }

    // Return successful population of offsets
    return true;
}

/*
 * Checks for locational equality between nodes
 */
bool equals(struct Node n1, struct Node n2) {
    return n1.row == n2.row && n1.col == n2.col;
}

void set_start_end() {
    // Create maze to treat maze_ptr as a multi dimensional array
    struct Node (*maze)[columns] = (struct Node (*)[columns])(maze_ptr);

    // Seed time for rand function
    srand(time(NULL));

    int start_row;
    int start_col;

    do {
        start_row = (rand() % 2) ? 0 : rows - 1;
        start_col = rand() % columns;
    } while (is_corner(start_row, start_col));    // Do not want to place start in a corner 
        

    int end_row;
    int end_col;

    do {
        end_row = rand() % rows;
        end_col = (rand() % 2) ? 0 : columns - 1;
    } while (is_corner(end_row, end_col));      // Do not want to place end in a corner

    maze[start_row][start_col].is_path = 1;
    maze[end_row][end_col].is_path = 1;
}

/*  
 *  This function will return true if the inputted row,col coordinates correspond to a corner in the
 *  maze.
 */
bool is_corner(int row, int col) {
    return (((row == 0) && (col == 0)) || ((row = rows - 1) && (col == 0)) 
            || ((row == 0) && (col == columns - 1)) 
            || ((rows == rows - 1) && (col == columns - 1)));
}
