#include "csv_parser.h"
int is_usine_prefix_valid(const char *id) {
    if (strstr(id, "Plant ") == id) return 1;
    if (strstr(id, "Module ") == id) return 1;
    if (strstr(id, "Unit ") == id) return 1;
    if (strstr(id, "Facility complex ") == id) return 1;
    return 0;
}
float parse_float(const char *token) {
    if (token == NULL || strcmp(token, "-") == 0) {
        return -1.0f; // Valeur sentinelle pour 'manquant'
    }
    return (float)atof(token);
}
int parse_csv_line(const char *line, Troncon_CSV_t *troncon) {
    char line_copy[MAX_LINE_LENGTH];
    char *token;
    int col_index = 0;
    if (line == NULL || strlen(line) >= MAX_LINE_LENGTH || strlen(line) == 0) {
        return -1; 
    }
    troncon->usine_id[0] = '\0';
    troncon->amont_id[0] = '\0';
    troncon->aval_id[0] = '\0';
    strncpy(line_copy, line, MAX_LINE_LENGTH - 1);
    line_copy[MAX_LINE_LENGTH - 1] = '\0'; 
    line_copy[strcspn(line_copy, "\r\n")] = '\0';
    token = strtok(line_copy, FIELD_DELIMITER);
    while (token != NULL) {
        while (*token == ' ') token++; 
        switch (col_index) {
            case 0: // Col 1: Usine ID (si '-' -> '\0')
                if (strcmp(token, "-") != 0) 
                    strncpy(troncon->usine_id, token, MAX_ID_LEN - 1);
                break;
            case 1: // Col 2: Amont ID (si '-' -> '\0')
                if (strcmp(token, "-") != 0) 
                    strncpy(troncon->amont_id, token, MAX_ID_LEN - 1);
                break;
            case 2: // Col 3: Aval ID (si '-' -> '\0')
                if (strcmp(token, "-") != 0) 
                    strncpy(troncon->aval_id, token, MAX_ID_LEN - 1);
                break;
            case 3: // Col 4: Volume/Capacité
                troncon->volume_or_capacity = parse_float(token);
                break;
            case 4: // Col 5: Fuites %
                troncon->leak_percentage = parse_float(token);
                break;
            default:
                break;
        }
        col_index++;
        token = strtok(NULL, FIELD_DELIMITER);
    }
    if (col_index <= 4) troncon->leak_percentage = -1.0f;
    if (col_index <= 3) troncon->volume_or_capacity = -1.0f;
    if (col_index <= 2) troncon->aval_id[0] = '\0';
if (troncon->volume_or_capacity != -1.0f && troncon->aval_id[0] == '\0' && troncon->leak_percentage == -1.0f) {
    if (is_usine_prefix_valid(troncon->amont_id)) {
        // C'est une Usine (Plant, Module, Unit, etc.)
        troncon->type = TYPE_USINE;
    } else {
        troncon->type = TYPE_UNKNOWN; 
    }
} else if (troncon->volume_or_capacity != -1.0f && troncon->leak_percentage != -1.0f) {
    troncon->type = TYPE_CAPTAGE;
} else {
     troncon->type = TYPE_UNKNOWN;
}
return 0;
    }
    
   
