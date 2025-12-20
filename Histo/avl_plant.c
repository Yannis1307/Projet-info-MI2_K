#include "../headers/avl_plant.h" 
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#ifndef max
#define max(a,b) (((a) > (b)) ? (a) : (b))
#endif

//Utility Functions: Node creation and safe height retrieval
int get_height(AVL_Plant_Node_t *node) {
    if (node == NULL)
        return 0;
    return node->height;
}

AVL_Plant_Node_t *new_plant_node(char *plant_id) {
    AVL_Plant_Node_t *node = (AVL_Plant_Node_t *)malloc(sizeof(AVL_Plant_Node_t));
    if (node == NULL) {
        perror("Error: Memory allocation failed for AVL Node");
        exit(EXIT_FAILURE); 
    }
    strncpy(node->id, plant_id, MAX_ID_LEN - 1);
    node->id[MAX_ID_LEN - 1] = '\0'; 
    node->max_capacity = 0;
    node->total_captured = 0;
    node->real_treated = 0;
    node->left = NULL;
    node->right = NULL;
    node->height = 1; 
    return node;
}

// AVL Balancing Primitives: Balance factor calculation and rotations
int get_balance(AVL_Plant_Node_t *node) {
    if (node == NULL){
    	return 0;
    }
    return get_height(node->right) - get_height(node->left); 
}

AVL_Plant_Node_t *rotate_right(AVL_Plant_Node_t *y) {
    if (y == NULL || y->left == NULL) {
        return y;
    }
    AVL_Plant_Node_t *x = y->left;
    AVL_Plant_Node_t *a = x->right; 
    x->right = y;
    y->left = a;
    y->height = 1 + max(get_height(y->left),get_height(y->right));
    x->height = 1 + max(get_height(x->left),get_height(x->right));
    return x;
}

AVL_Plant_Node_t *rotate_left(AVL_Plant_Node_t *x) {
    if (x == NULL || x->right == NULL) {
        return x; 
    }
    AVL_Plant_Node_t *y = x->right;
    AVL_Plant_Node_t *a = y->left; 
    y->left = x;
    x->right = a; 
    x->height = 1 + max(get_height(x->left),get_height(x->right));                    
    y->height = 1 + max(get_height(y->left),get_height(y->right));
    return y;
}

// Tree Operations: Recursive search and memory deallocation
AVL_Plant_Node_t *search_plant(AVL_Plant_Node_t *root, char *plant_id) {
    if (root == NULL || strcmp(plant_id, root->id) == 0) {
        return root;
    }
    if (strcmp(plant_id, root->id) < 0) {   
        return search_plant(root->left, plant_id);
    } 
    else {  
        return search_plant(root->right, plant_id);
    }
}

void free_plant_avl(AVL_Plant_Node_t *root) {   
    if (root == NULL) {
        return;
    }
    free_plant_avl(root->left);
    free_plant_avl(root->right);
    free(root);
}

// Insertion Logic: Recursive insertion with automatic rebalancing
AVL_Plant_Node_t *insert_plant(AVL_Plant_Node_t *node, char *plant_id) {
    //Standard BST Insertion
    if (node == NULL) {
        return new_plant_node(plant_id);
    }
    int cmp_result = strcmp(plant_id, node->id);
    if (cmp_result < 0) {
        node->left = insert_plant(node->left, plant_id);
    } 
    else if (cmp_result > 0) {
        node->right = insert_plant(node->right, plant_id);
    } 
    else {
        return node; 
    }
    
    // Height Update & Rebalancing
    node->height = 1 + max(get_height(node->left), get_height(node->right));
    int balance = get_balance(node); 
    
    // Left Heavy
    if (balance < -1 && get_balance(node->left) <= 0) {
        return rotate_right(node);
    }
    // Right Heavy
    if (balance > 1 && get_balance(node->right) >= 0) {
        return rotate_left(node);
    }
    // Left-Right Case
    if (balance < -1 && get_balance(node->left) > 0) {
        node->left = rotate_left(node->left); 
        return rotate_right(node);            
    }
    // Right-Left Case
    if (balance > 1 && get_balance(node->right) < 0) {
        node->right = rotate_right(node->right); 
        return rotate_left(node);                 
    }
    return node;
}
