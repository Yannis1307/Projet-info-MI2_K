#ifndef AVL_USINE_H
#define AVL_USINE_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "types.h" // Contient la définition de AVL_Node_Usine_t et Troncon_CSV_t

// Fonction utilitaire max() si non déjà définie (nécessaire pour les hauteurs)
#define max(a, b) ((a > b) ? a : b)
// avl_usine.h - AJOUTER CECI SOUS LA MACRO max(a, b)

// --- Fonctions d'Utilité AVL ---
int get_height(AVL_Node_Usine_t *node);
AVL_Node_Usine_t *new_usine_node(const char *id_usine);
int get_balance(AVL_Node_Usine_t *node);
void free_usine_avl(AVL_Node_Usine_t *root); // Pour la désallocation mémoire

// --- Fonctions de Rotation ---
AVL_Node_Usine_t *rotate_right(AVL_Node_Usine_t *y);
AVL_Node_Usine_t *rotate_left(AVL_Node_Usine_t *x);

// --- Fonctions Principales ---
AVL_Node_Usine_t *search_usine(AVL_Node_Usine_t *root, const char *id_usine);
// Note : insert_usine est la plus complexe, car elle gère la récursivité et l'équilibrage
AVL_Node_Usine_t *insert_usine(AVL_Node_Usine_t *node, const char *id_usine);
#endif
