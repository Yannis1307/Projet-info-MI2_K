#include "avl_usine.h" 

int get_height(AVL_Node_Usine_t *node) {
    if (node == NULL)
        return 0;
    return node->height;
}


AVL_Node_Usine_t *new_usine_node(const char *id_usine) {
    AVL_Node_Usine_t *node = (AVL_Node_Usine_t *)malloc(sizeof(AVL_Node_Usine_t));
    
    if (node == NULL) {
        perror("Erreur: Allocation mémoire pour l'AVL a échoué");
        exit(EXIT_FAILURE); 
    }

   
    strncpy(node->id, id_usine, MAX_ID_LEN - 1);
    node->id[MAX_ID_LEN - 1] = '\0'; 
    
    node->max_capacity = 0;
    node->total_captured = 0;
    node->real_treated = 0;
    
    node->left = NULL;
    node->right = NULL;
    node->height = 1; 

    return node;
}


int get_balance(AVL_Node_Usine_t *node) {
    if (node == NULL)
        return 0;
    
    
    return get_height(node->right) - get_height(node->left); 
}






AVL_Node_Usine_t *rotate_right(AVL_Node_Usine_t *y) {
    
    
    if (y == NULL || y->left == NULL) {
        
        return y;
    }
    
    
    AVL_Node_Usine_t *x = y->left;
    AVL_Node_Usine_t *a = x->right; 
    x->right = y;
    y->left = a;

    
    y->height = 1 + max(get_height(y->left),get_height(y->right));
    
    x->height = 1 + max(get_height(x->left),get_height(x->right));

    
    return x;
}

AVL_Node_Usine_t *rotate_left(AVL_Node_Usine_t *x) {
    

    if (x == NULL || x->right == NULL) {
        return x; 
    }
    
    
    AVL_Node_Usine_t *y = x->right;
    AVL_Node_Usine_t *a = y->left; 

    
    y->left = x;
    x->right = a; 

    x->height = 1 + max(get_height(x->left),get_height(x->right));
                        
    y->height = 1 + max(get_height(y->left),get_height(y->right));

    return y;
}
