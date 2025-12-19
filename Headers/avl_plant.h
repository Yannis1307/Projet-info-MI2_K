#ifndef AVL_PLANT_H
#define AVL_PLANT_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "types.h" 

// Helper macro for height calculation
#define max(a, b) ((a > b) ? a : b)

// Utility Functions: Memory management and node properties
int get_height(AVL_Plant_Node_t *node);
AVL_Plant_Node_t *new_plant_node(const char *plant_id);
int get_balance(AVL_Plant_Node_t *node);
void free_plant_avl(AVL_Plant_Node_t *root); 

// Balancing Functions: Standard AVL rotations
AVL_Plant_Node_t *rotate_right(AVL_Plant_Node_t *y);
AVL_Plant_Node_t *rotate_left(AVL_Plant_Node_t *x);

// Core Operations: Recursive search and insertion
AVL_Plant_Node_t *search_plant(AVL_Plant_Node_t *root, const char *plant_id);
AVL_Plant_Node_t *insert_plant(AVL_Plant_Node_t *node, const char *plant_id);

#endif
