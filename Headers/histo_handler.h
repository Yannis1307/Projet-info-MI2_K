#ifndef LEAKS_HANDLER_H
#define LEAKS_HANDLER_H

#include <stdio.h>
#include <stdlib.h>
#include "types.h"

// Main entry point for the 'leaks' command logic
int handle_leaks_data(const char *target_factory_id, const char *input_source);

#endif
