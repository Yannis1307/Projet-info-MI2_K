#include "csv_parser.h"

// Aide à la conversion numérique.
float parse_float(const char *token) {
    // Vérifie si le token est NULL ou le tiret de valeur manquante
    if (token == NULL || strcmp(token, "-") == 0) {
        return -1.0f; // Valeur sentinelle pour 'manquant'
    }
    return (float)atof(token);
}
int parse_csv_line(const char *line, Troncon_CSV_t *troncon) {
    char line_copy[MAX_LINE_LENGTH];
    char *token;
    int col_index = 0;
    
    // --- Phase Préparatoire ---
    if (line == NULL || strlen(line) >= MAX_LINE_LENGTH || strlen(line) == 0) {
        return -1; 
    }
    
    // Initialisation des IDs à des chaînes vides pour faciliter la vérification (ID == '\0' si '-')
    troncon->usine_id[0] = '\0';
    troncon->amont_id[0] = '\0';
    troncon->aval_id[0] = '\0';

    strncpy(line_copy, line, MAX_LINE_LENGTH - 1);
    line_copy[MAX_LINE_LENGTH - 1] = '\0'; 
    line_copy[strcspn(line_copy, "\r\n")] = '\0';

    /* --- 1. Découpage et Remplissage des Champs --- */
    token = strtok(line_copy, FIELD_DELIMITER);
    
    while (token != NULL) {
        // Enlève l'espace en début de token si présent (ex: " Unit #...")
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

    // Gérer les colonnes 3, 4 et 5 si elles manquent à la fin de la ligne (même après filtrage)
    if (col_index <= 4) troncon->leak_percentage = -1.0f;
    if (col_index <= 3) troncon->volume_or_capacity = -1.0f;
    if (col_index <= 2) troncon->aval_id[0] = '\0';

    /* --- 2. Classification Minimaliste dans le C --- */
    // Le C vérifie seulement s'il s'agit d'un Nœud Usine ou d'un Tronçon
    if (troncon->volume_or_capacity != -1.0f && troncon->aval_id[0] == '\0' && troncon->leak_percentage == -1.0f) {
        // C'est la signature unique du Nœud USINE pour la Capacité (Ex: -;Unit #NM000000T;-;3442;-)
        troncon->type = TYPE_USINE;
    } else {
        // Tous les autres sont des tronçons (Captage, Stockage, Distribution...)
        troncon->type = TYPE_CAPTAGE; // On peut mettre TYPE_CAPTAGE par défaut,
                                      // ou mieux, le type sera déterminé par la fonction appelante du C.
    }
    
    return 0;
}
