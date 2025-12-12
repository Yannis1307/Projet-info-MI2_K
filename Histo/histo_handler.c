#include "histo_handler.h"
#include "csv_parser.h" 

#define BUFFER_SIZE 1024


int update_usine_metrics(AVL_Node_Usine_t **root, Troncon_CSV_t *troncon) {
    AVL_Node_Usine_t *usine = NULL;
    const char *usine_id_key = NULL;
    if (troncon->type == TYPE_USINE) {
        usine_id_key = troncon->amont_id;
    } else if (troncon->type == TYPE_CAPTAGE) {
        usine_id_key = troncon->aval_id;
    } else {  
        return 0; 
    }
    if (usine_id_key[0] == '\0') {
        return 0; 
    }
    *root = insert_usine(*root, usine_id_key);
    usine = search_usine(*root, usine_id_key);
    if (usine == NULL) {
        
        fprintf(stderr, "Erreur: Impossible de retrouver le nœud usine après insertion.\n");
        return -1;
    }
    if (troncon->type == TYPE_USINE) {
        if (troncon->volume_or_capacity > 0) {
            usine->max_capacity = (long long)troncon->volume_or_capacity;
        }
    } else if (troncon->type == TYPE_CAPTAGE) {
        if (troncon->volume_or_capacity > 0) {
            long long volume_capte = (long long)troncon->volume_or_capacity;
            usine->total_captured += volume_capte;
            if (troncon->leak_percentage >= 0) {
                float fuite_percent = troncon->leak_percentage / 100.0f;
                usine->real_treated += (long long)(volume_capte * (1.0f - fuite_percent));
            } else {
                 usine->real_treated += volume_capte;
            }
        }
    }
    return 0;
}
void traverse_and_write(AVL_Node_Usine_t *node, FILE *fp, const char *mode) {
    if (node == NULL) {
        return;
    }
    traverse_and_write(node->right, fp, mode);
    long long value_km3 = 0;
    if (strcmp(mode, "max") == 0) {
        value_km3 = node->max_capacity;
    } else if (strcmp(mode, "src") == 0) {
        value_km3 = node->total_captured;
    } else if (strcmp(mode, "real") == 0) {
        value_km3 = node->real_treated;
    }
    double volume_Mm3 = (double)value_km3 / 1000.0;
    fprintf(fp, "%s;%.3f\n", node->id, volume_Mm3);
    traverse_and_write(node->left, fp, mode);
}


int write_histo_results(AVL_Node_Usine_t *root, const char *mode) {
    
    FILE *fp = stdout; 
    fprintf(fp, "identifier;");

    if (strcmp(mode, "max") == 0) {
        fprintf(fp, "max volume (M.m3.year-1)\n"); 
    } else if (strcmp(mode, "src") == 0) {
        fprintf(fp, "source volume (M.m3.year-1)\n"); 
    } else if (strcmp(mode, "real") == 0) {
        fprintf(fp, "real volume (M.m3.year-1)\n"); 
    } else {
        fprintf(stderr, "Erreur: Mode d'histogramme invalide.\n");
        return 1;
    }

    
    traverse_and_write(root, fp, mode);
    return 0;
}
int handle_histo_data(const char *mode, const char *data_filename) {
    FILE *file;
    char line_buffer[MAX_LINE_LENGTH];
    AVL_Node_Usine_t *usine_root = NULL;
    int return_code = 0; 
    if (strcmp(data_filename, "-") == 0) {
        file = stdin;
    } else {
        file = fopen(data_filename, "r");
        if (file == NULL) {
            perror("Erreur: Impossible d'ouvrir le fichier de données");
            return 1; 
        }
    }
    while (fgets(line_buffer, MAX_LINE_LENGTH, file) != NULL) {
        Troncon_CSV_t troncon;
        
        if (parse_csv_line(line_buffer, &troncon) == 0) {
            if (update_usine_metrics(&usine_root, &troncon) != 0) {
                return_code = 2; 
                break;
            }
        }
    }
    
    if (file != stdin && file != NULL) {
        fclose(file);
    }
    if (return_code == 0) {
        return_code = write_histo_results(usine_root, mode);
    }
    free_usine_avl(usine_root);
    
    return return_code;
}
