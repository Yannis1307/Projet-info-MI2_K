#ifndef HISTO_HANDLER_H
#define HISTO_HANDLER_H

#include <stdio.h>
#include <stdlib.h>
#include "types.h"
#include "avl_plant.h" 

// Main entry point for the 'histo' command logic
int handle_histo_data(char *mode, char *data_filename);

// Exports sorted statistics from the AVL tree to stdout
int write_histo_results(AVL_Plant_Node_t *root, char *mode);

#endif
