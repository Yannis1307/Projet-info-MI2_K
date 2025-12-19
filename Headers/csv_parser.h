#ifndef CSV_PARSER_H
#define CSV_PARSER_H

#include "types.h"
#include <string.h>
#include <stdlib.h>

#define MAX_LINE_LENGTH 1024
#define FIELD_DELIMITER ";"

float parse_float(const char *token);
int parse_csv_line(const char *line, CSV_Section_t *section);

#endif

