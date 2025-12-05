#ifndef HISTO_HANDLER_H
#define HISTO_HANDLER_H

#include <stdio.h>
#include <stdlib.h>
#include "types.h"
#include "avl_usine.h" // Nécessaire pour utiliser les fonctions AVL

// Prototype de la fonction principale de lecture/calcul
// Prend en entrée le mode d'histogramme (ex: "max", "src", "real")
int handle_histo_data(const char *mode, const char *data_filename);// remplace datafilename par stdin pour le shell 

// Prototype de la fonction d'écriture (à implémenter plus tard)
int write_histo_results(AVL_Node_Usine_t *root, const char *mode);

#endif // HISTO_HANDLER_H
