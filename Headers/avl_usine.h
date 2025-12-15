#ifndef AVL_USINE_H
#define AVL_USINE_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "types.h" 


#define max(a, b) ((a > b) ? a : b)
int get_height(AVL_Node_Usine_t *node);
AVL_Node_Usine_t *new_usine_node(const char *id_usine);
int get_balance(AVL_Node_Usine_t *node);
void free_usine_avl(AVL_Node_Usine_t *root); 


AVL_Node_Usine_t *rotate_right(AVL_Node_Usine_t *y);
AVL_Node_Usine_t *rotate_left(AVL_Node_Usine_t *x);

AVL_Node_Usine_t *search_usine(AVL_Node_Usine_t *root, const char *id_usine);
AVL_Node_Usine_t *insert_usine(AVL_Node_Usine_t *node, const char *id_usine);
#endif
