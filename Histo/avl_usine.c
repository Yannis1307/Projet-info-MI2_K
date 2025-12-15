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

AVL_Node_Usine_t *search_usine(AVL_Node_Usine_t *root, const char *id_usine) {
    if (root == NULL || strcmp(id_usine, root->id) == 0) {
        return root;
    }
    if (strcmp(id_usine, root->id) < 0) {   
        return search_usine(root->left, id_usine);
    } 
    else {  
        return search_usine(root->right, id_usine);
    }
}
void free_usine_avl(AVL_Node_Usine_t *root) {   
    if (root == NULL) {
        return;
    }
    free_usine_avl(root->left);
    free_usine_avl(root->right);
    free(root);
}
AVL_Node_Usine_t *insert_usine(AVL_Node_Usine_t *node, const char *id_usine) {
    if (node == NULL) {
        return new_usine_node(id_usine);
    }
    int cmp_result = strcmp(id_usine, node->id);
    if (cmp_result < 0) {
        node->left = insert_usine(node->left, id_usine);
    } 
    else if (cmp_result > 0) {
        node->right = insert_usine(node->right, id_usine);
    } 
    else {
        return node; 
    }
    node->height = 1 + max(get_height(node->left), get_height(node->right));
    int balance = get_balance(node); 
    if (balance < -1 && get_balance(node->left) <= 0) {
        return rotate_right(node);
    }
    if (balance > 1 && get_balance(node->right) >= 0) {
        return rotate_left(node);
    }
    if (balance < -1 && get_balance(node->left) > 0) {
        node->left = rotate_left(node->left); 
        return rotate_right(node);           
    }
    if (balance > 1 && get_balance(node->right) < 0) {
        node->right = rotate_right(node->right); 
        return rotate_left(node);                
    }
    return node;
}
