/**
 * @brief Effectue une rotation droite (pour déséquilibre de type LL ou LR).
 * @param y Le nœud déséquilibré.
 * @return La nouvelle racine du sous-arbre après rotation (x).
 */
AVL_Node_Usine_t *rotate_right(AVL_Node_Usine_t *y) {
    // 1. Définir les pointeurs
    AVL_Node_Usine_t *x = y->left;
    AVL_Node_Usine_t *a = x->right; // 'a' est l'ancien T2 (le sous-arbre de transition)

    // 2. Effectuer la rotation
    x->right = y;
    y->left = a; // Remplacer T2 par a

    // 3. Mettre à jour les hauteurs (de bas en haut)
    // get_height est nécessaire ici (doit être implémentée dans avl_usine.c)
    y->height = 1 + max(get_height(y->left), 
    get_height(y->right));
    x->height = 1 + max(get_height(x->left), 
    get_height(x->right));

    // x devient la nouvelle racine
    return x;
}
/**
 * @brief Effectue une rotation gauche (pour déséquilibre de type RR ou RL).
 * @param x Le nœud déséquilibré.
 * @return La nouvelle racine du sous-arbre après rotation (y).
 */
AVL_Node_Usine_t *rotate_left(AVL_Node_Usine_t *x) {
    // 1. Définir les pointeurs
    AVL_Node_Usine_t *y = x->right;
    AVL_Node_Usine_t *a = y->left; // 'a' est l'ancien T2 (le sous-arbre de transition)

    // 2. Effectuer la rotation
    y->left = x;
    x->right = a; // Remplacer T2 par a

    // 3. Mettre à jour les hauteurs (de bas en haut)
    // get_height est nécessaire ici (doit être implémentée dans avl_usine.c)
    x->height = 1 + max(get_height(x->left), 
    get_height(x->right));
    y->height = 1 + max(get_height(y->left), 
    get_height(y->right));

    // y devient la nouvelle racine
    return y;
}
