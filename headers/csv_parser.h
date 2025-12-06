#ifndef CSV_PARSER_H
#define CSV_PARSER_H

#include "types.h"
#include <string.h>
#include <stdlib.h>

#define MAX_LINE_LENGTH 1024
#define FIELD_DELIMITER ";"

// Prototypes
float parse_float(const char *token);
int parse_csv_line(const char *line, Troncon_CSV_t *troncon);

#endif // CSV_PARSER_H
