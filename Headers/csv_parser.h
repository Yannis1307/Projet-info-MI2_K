#ifndef CSV_PARSER_H
#define CSV_PARSER_H

#include "types.h"
#include <string.h>
#include <stdlib.h>

// Configuration constants for file reading
#define MAX_LINE_LENGTH 1024
#define FIELD_DELIMITER ";"

// Helper: Safely converts string tokens to float
float parse_float(const char *token);

// Main Parsing: Extracts data from a raw CSV line into the standard structure
int parse_csv_line(const char *line, CSV_Section_t *section);

#endif
