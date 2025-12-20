#ifndef LEAKS_HANDLER_H
#define LEAKS_HANDLER_H

#include <stdio.h>
#include <stdlib.h>
#include "types.h"

// Main entry point for the 'leaks' command logic
int handle_leaks_data(char *target_factory_id,  char *input_source);

#endif
