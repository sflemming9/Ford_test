/* The Node struct is used for holding information about information associated with a cell.
 * It holds the row, column, wether something is a wall or not (is_path) and the directions that
 * have been visted from this particular Node.
 *
 * bit_directs documentation:
 * 1 << 0 = right
 * 1 << 1 = down
 * 1 << 2 = left
 * 1 << 3 = up 
 *
 * Author: Sabrina Flemming
 * */
struct Node {
    struct Node* prev;  // Represents the parent node
    int row; 
    int col;
    unsigned int is_path;   // 1: yes it is part of the path; 0: no it is a wall
    unsigned int bit_directs : 5; // Bit packed field representing directions visted
};
