/*
 * Handles major functionality for the Maze generation problem posed by Ford.
 * A rectangular maze is generated using a depth first search algorithm.
 *
 * High level requirements:
 * 1. Maze must have a solution
 * 2. Maze must have a start and end
 *
 * Author: Sabrina Flemming
 * Date: October 5th, 2018
 *
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
static unsigned int ROWS = 0;   // Number of rows in the maze
static unsigned int COLUMNS = 0;    // Number of columns in the maze

/* Function prototypes */
static bool validateInputs(int argc, char* argv[]);
static int maze_init();
static void print_maze();
static void node_init(unsigned int row, unsigned int column);
static bool is_seed(unsigned int row, unsigned int column);
static void dfs();
static bool equals(struct Node n1, struct Node n2);
static bool get_offsets(unsigned int* row_offset, unsigned int* col_offset, unsigned int rand_val,
        struct Node curr_node);
static int midpoint(int a, int b);
static void set_start_end();
static bool is_corner(int row, int col);
static bool is_even(int num);


/*
 * Handles running and displaying the maze problem. Two command line arguments are expected
 * (number of rows, number of columns).
 * See Compiling_and_Running.org file for further instructions.
 *
 * Potential improvements:
 * - Take flags from command line that modify execution (eg: higher amount of logging)
 * - Handle mazes with an even number of rows and columns
 */
int main(int argc, char* argv[]) {

    // Validate inputs for correct number of arguments, convertability, 0, negative, and size
    if (!validateInputs(argc, argv)) return -1;

    // Initialize rows and columns; atoi is safe because we handled checking our inputs above
    ROWS = atoi(argv[1]);
    COLUMNS = atoi(argv[2]); 

    // Initialize the maze
    if (maze_init(ROWS, COLUMNS)) return -1;

    // Apply a Depth First Search to generate a maze from initialized data
    dfs();

    // Set start and end to maze solution
    set_start_end();

    print_maze();

    return 0;
}

/*
 * Function: validateInputs
 * ----------------------------------------------------------------------
 * Handles validating inputs from the command line.  Returns true if invalid input is encountered.
 * The command line input is inputted and a bool is returned signifying data has been validated
 * (true) or inputted data is not correct (false).
 *
 * Potential improvements:
 * - Provide warnings for non int/long style data types
 * - Handle for non standard input/mixed data types
 * - Use better function than atoi to perform conversion (atoi does not have an error call back func)
 */
static bool validateInputs(int argc, char* argv[]) {

    // Check for number of arguments
    if (argc != 3) {
        printf("Please pass 2  command line arguments: the number of rows and columns in the maze.\n");
        return false;
    }

    // Convert numbers
    int rows = atoi(argv[1]);
    int columns = atoi(argv[2]);

    // Validate conversion produced valid values
    if (rows <= 1 || columns <= 1) {
        printf("Invalid input, please input two positive, odd numbers greater than 1.\n");
        return false;
    }

    // Number of rows and columns of maze must be odd
    if ((rows % 2) == 0 || (columns % 2) == 0) {
        printf("Invlaid input, please input two positive, odd numbers greater than 1.\n");
        return false;
    }

    return true;
}

/*
 * Function: maze_init
 * ----------------------------------------------------------------------
 * Initializes the maze, and then calls node_init to initialize all the nodes within it.
 */
static int maze_init() {

    // Allocate memory for storing nodes
    // This memory is not freed because it is required until completion of the program
    maze_ptr = (struct Node*)malloc((ROWS * COLUMNS) * sizeof(struct Node));

    // Ensure malloc call was sucessful. Return -1 if it was unsuccessful.
    if (maze_ptr == NULL) return -1;

    // Create maze to treat maze_ptr as a multi dimensional array
    struct Node (*maze)[COLUMNS] = (struct Node (*)[COLUMNS])(maze_ptr);

    // Initialize all Nodes in the maze array
    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLUMNS; c++)  {
            node_init(r, c);
        }
    }

    return 0;
}

/*
 * Function: print_maze
 * ----------------------------------------------------------------------
 * This function prints out the maze. A wall is represented by a '|' and a path is represented
 * by a space.
 */
static void print_maze() {
    // Create maze to treat maze_ptr as a multi dimensional array
    struct Node (*maze)[COLUMNS] = (struct Node (*)[COLUMNS])(maze_ptr);

    // Iterate through maze and print the character associated with is_path
    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLUMNS; c++)  {
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
 * Function: node_init
 * ----------------------------------------------------------------------
 * This function initializes a node at location row,column with the appropriate value. The nodes are
 * differentiated on whether or not they are a 'seed node'. A seed node is a node that has odd
 * numbers for its row,column and is also not along the edges of the maze. This creates a
 * checkerboard type maze to allow for dfs to generate a full maze.
 */
static void node_init(unsigned int row, unsigned int column) {

    // Create maze to treat maze_ptr as a multi dimensional array
    struct Node (*maze)[COLUMNS] = (struct Node (*)[COLUMNS])(maze_ptr);

    // Assign the location of the node
    maze[row][column].row = row;
    maze[row][column].col = column;

    maze[row][column].bit_directs = 15;     // Initialize all directions as visited

    // Check if a Node is a seed, and assign values accordingly
    if (is_seed(row, column)) {
        maze[row][column].is_path = 1;
    } else {
        maze[row][column].is_path = 0;
    }
}

/*
 * Function: is_seed
 * ----------------------------------------------------------------------
 * A seed node is a node located on an odd row and odd column. It is necessary to seed the
 * matrix in this manner so that a DFS can be performed without creating large open blocks in the
 * maze, and ensuring that a solution can always be found for a given start and end.
 */
static bool is_seed(unsigned int row, unsigned int column) {
    return ((row % 2) && (column % 2));
}

/*
 * Function: dfs
 * ----------------------------------------------------------------------
 * Handles making a Depth First Search through the maze and nodes to determine a path in order to
 * generate a maze. Depth First Search starts at a single node and explores a path with one of its
 * neighbours. If a path is possible in this direction, it will move forward resulting in the
 * neighbour becoming the new start point. This cycle is continued until a dead end is reached (all
 * of the neighbours are either edges or have already been explored). The algorithm then backtracks
 * all the way back to the original start node and checks if there are any other directions in which
 * it can pursue a path.
 *
 * Potential improvements:
 * - Make dfs take arguments for where you want the dfs to start, and validate those arguments
 * - Keep track of the optimal path for a given start and end coord (assuming they're on the) edges
 * - Further decomposition
 * - Structure function in such a way that makes it easier to test
 */
static void dfs() {
    // Create maze to treat maze_ptr as a multi dimensional array
    struct Node (*maze)[COLUMNS] = (struct Node (*)[COLUMNS])(maze_ptr);

    // Declare start and current nodes
    struct Node *start = &maze[1][1];     // Start node will always be in location (1, 1)
    start->prev = start;
    struct Node *curr_node = start;

    // Seed time for rand function
    srand(time(NULL));

    do {
        unsigned int* row_offset = malloc(sizeof(unsigned int));
        unsigned int* col_offset= malloc(sizeof(unsigned int));

        // While the current node being examined has unexplored directions
        while (curr_node->bit_directs) {

            // Randomly choose a direction (up, down, right, left)
            int rand_val = rand() % 4;     // Random value between 0 and 3

            // Check if this direction has already been explored - if so, continue
            if (!CHECK_BIT(curr_node->bit_directs, rand_val)) continue;

            // This direction has not been explored, therefore now set as explored
            curr_node->bit_directs ^= (1 << rand_val);

            // Get new row and column coordinates for next node in the direction of exploration
            // If getOfsset encounters a wall (returns false), continue
            if (!get_offsets(row_offset, col_offset, rand_val, *curr_node)) continue;

            // Fetch next node from maze array
            struct Node* next_node = &maze[*row_offset][*col_offset];

            if (next_node->is_path) {

                // If the next node has alreay been added to the path do not process further
                if (next_node->prev != NULL) continue;

                // Set prev of the next node to be the curr_node
                next_node->prev = curr_node;

                // Add connecting node to path
                if (curr_node->row - next_node->row == 0) {
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

    } while (!equals(*curr_node, *start));  // Continue until we have backtracked all the way to start
}

/*
 * Function: midpoint
 * ----------------------------------------------------------------------
 * Returns the midpoint between two integers, a and b.
 */
static int midpoint(int a, int b) {
    return a + ((b - a) / 2);
}

/*
 * Function: get_offsets
 * ----------------------------------------------------------------------
 * Handles getting offsets for the next node from the curr_node. The direction to explore is based
 * on a random value (between 0 and 3) that provides direction of offset. Populates data into
 * row_offset and col_offset.
 */
static bool get_offsets(unsigned int* row_offset, unsigned int* col_offset, unsigned int rand_val,
        struct Node curr_node) {

    if (rand_val == 2  && ((curr_node.col - 2) >= 0)) {     // Explore Left if possible
        *row_offset = curr_node.row;
        *col_offset = curr_node.col - 2;

    } else if(rand_val == 3 && ((curr_node.row - 2) >= 0)) {    // Explore Up if possible
        *row_offset = curr_node.row - 2;
        *col_offset = curr_node.col;

    } else if(rand_val == 0 && ((curr_node.col + 2) < COLUMNS)) {  // Explore Right if possible
        *row_offset = curr_node.row;
        *col_offset = curr_node.col + 2;

    } else if(rand_val == 1 && ((curr_node.row + 2) < ROWS)) {   // Explore Down if possible
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
 * Function: equals
 * ----------------------------------------------------------------------
 * Checks for locational equality between two nodes. Returns true if they are equal and false
 * otherwise.
 */
static bool equals(struct Node n1, struct Node n2) {
    return n1.row == n2.row && n1.col == n2.col;
}

/*
 * Function: set_start_end
 * ----------------------------------------------------------------------
 * This function assigns the starting and ending nodes, both of which will always be located along
 * an edge but cannot be located in a corner. The start node will always be located along the upper
 * or bottom edge of the maze and must be placed in an odd column (to ensure a solution). The end
 * node will always be located along the left or right edge, and myst be placed in an odd row
 * of the maze (to ensure a solution).
 * */
static void set_start_end() {
    // Create maze to treat maze_ptr as a multi dimensional array
    struct Node (*maze)[COLUMNS] = (struct Node (*)[COLUMNS])(maze_ptr);

    // Seed time for rand function
    srand(time(NULL));

    int start_row;
    int start_col;

    do {
        start_row = (rand() % 2) ? 0 : ROWS - 1;
        start_col = rand() % COLUMNS;
    } while (is_corner(start_row, start_col) || (is_even(start_col)));


    int end_row;
    int end_col;

    do {
        end_row = rand() % ROWS;
        end_col = (rand() % 2) ? 0 : COLUMNS - 1;
    } while (is_corner(end_row, end_col) || (is_even(end_row)));

    // Mark the start and end nodes as part of the path
    maze[start_row][start_col].is_path = 1;
    maze[end_row][end_col].is_path = 1;
}

/*
 * Function: is_corner
 * ----------------------------------------------------------------------
 * This function will return true if the inputted row,col coordinates correspond to a corner in the
 * maze, otherwise it will return false.
 */
static bool is_corner(int row, int col) {
    return (((row == 0) && (col == 0)) || ((row == ROWS - 1) && (col == 0))
            || ((row == 0) && (col == COLUMNS - 1))
            || ((row == ROWS - 1) && (col == COLUMNS - 1)));
}

/*
 * Function: is_even
 * ----------------------------------------------------------------------
 * This function will return true if the inputted number is even, and return false if it is odd.
 * */
static bool is_even(int num) {
    return !(num % 2);
}
