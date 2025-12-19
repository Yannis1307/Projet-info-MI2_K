#include "../headers/csv_parser.h"
// Analyzes the beginning of the 'id' string to check if it matches one of the four production prefixes returns 1 if found, and 0 otherwise.
int is_plant_prefix_valid(char *id) {
    if (strstr(id, "Plant ") == id) return 1;
    if (strstr(id, "Module ") == id) return 1;
    if (strstr(id, "Unit ") == id) return 1;
    if (strstr(id, "Facility complex ") == id) return 1;
    return 0;
}

//If the data is present, it converts the text into a real number using the atof function.

float parse_float(char *token) {
    if (token == NULL || strcmp(token, "-") == 0) {
        return -1.0f; 
    }
    return (float)atof(token);
}

int parse_csv_line(char *line, CSV_Section_t *section) {
    char line_copy[MAX_LINE_LENGTH];
    char *token;
    int col_index = 0;
    //préparation and cleaning
    if (line == NULL || strlen(line) >= MAX_LINE_LENGTH || strlen(line) == 0) {
        return -1; 
    }
    
    section->plant_id[0] = '\0';
    section->upstream_id[0] = '\0';
    section->downstream_id[0] = '\0';
    
    strncpy(line_copy, line, MAX_LINE_LENGTH - 1);
    line_copy[MAX_LINE_LENGTH - 1] = '\0'; 
    line_copy[strcspn(line_copy, "\r\n")] = '\0';
    
    token = strtok(line_copy, FIELD_DELIMITER);
    //Iterates through each column one by one to store the data in the correct fields
    while (token != NULL) {
        while (*token == ' ') token++; 
        
        switch (col_index) {
            case 0: 
                if (strcmp(token, "-") != 0) 
                    strncpy(section->plant_id, token, MAX_ID_LEN - 1);
                break;
            case 1: 
                if (strcmp(token, "-") != 0) 
                    strncpy(section->upstream_id, token, MAX_ID_LEN - 1);
                break;
            case 2: 
                if (strcmp(token, "-") != 0) 
                    strncpy(section->downstream_id, token, MAX_ID_LEN - 1);
                break;
            case 3: 
                section->volume_or_capacity = parse_float(token);
                break;
            case 4: 
                section->leak_percentage = parse_float(token);
                break;
            default:
                break;
        }
        col_index++;
        token = strtok(NULL, FIELD_DELIMITER);
    }
    //This part of the function serves to secure missing data and to categorize the line to determine if it is a plant or an intake section.
    if (col_index <= 4) section->leak_percentage = -1.0f;
    if (col_index <= 3) section->volume_or_capacity = -1.0f;
    if (col_index <= 2) section->downstream_id[0] = '\0';
    
    if (section->volume_or_capacity != -1.0f && section->downstream_id[0] == '\0' && section->leak_percentage == -1.0f) {
        if (is_plant_prefix_valid(section->upstream_id)) {
             
            section->type = TYPE_PLANT;
        } else {
            section->type = TYPE_UNKNOWN; 
        }
    } else if (section->volume_or_capacity != -1.0f && section->leak_percentage != -1.0f) {
        section->type = TYPE_INTAKE; 
    } else {
         section->type = TYPE_UNKNOWN;
    }
    return 0;
}
