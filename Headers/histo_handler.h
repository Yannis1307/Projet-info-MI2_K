#ifndef HISTO_HANDLER_H
#define HISTO_HANDLER_H

#include <stdio.h>
#include <stdlib.h>
#include "types.h"
#include "avl_usine.h" 

int handle_histo_data(const char *mode, const char *data_filename);
int write_histo_results(AVL_Node_Usine_t *root, const char *mode);

#endif 
