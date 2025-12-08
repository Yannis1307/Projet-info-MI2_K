#include "histo_handler.h"
#include "csv_parser.h" // Pour la fonction parse_csv_line

#define BUFFER_SIZE 1024

// Fonction d'aide pour insérer/rechercher l'usine et mettre à jour ses volumes.
// Note: Cette fonction simplifie la logique d'accumulation dans la boucle principale.
int update_usine_metrics(AVL_Node_Usine_t **root, Troncon_CSV_t *troncon) {
    
    AVL_Node_Usine_t *usine = NULL;
    const char *usine_id_key = NULL;

    // Déterminer la clé d'usine et le type d'opération
    if (troncon->type == TYPE_USINE) {
        // Cas 1 : Ligne décrivant le nœud USINE (Col 2 contient l'ID)
        usine_id_key = troncon->amont_id;
    } else if (troncon->type == TYPE_CAPTAGE) {
        // Cas 2 : Ligne décrivant un tronçon SOURCE -> USINE (Col 3 contient l'ID Usine)
        usine_id_key = troncon->aval_id;
    } else {
        // Nous ne devrions pas voir d'autres types si le Shell a bien filtré.
        return 0; 
    }

    // Sécurité A-Z : Ignorer si l'ID est manquant (devrait être géré par le Shell/parser, mais sécurité supplémentaire)
    if (usine_id_key[0] == '\0') {
        return 0; 
    }

    // Insérer/retrouver l'usine dans l'AVL (la fonction insert gère l'insertion si nécessaire)
    *root = insert_usine(*root, usine_id_key);
    
    // Rechercher le nœud (nécessaire car insert_usine retourne la nouvelle racine, pas le nœud inséré/trouvé)
    usine = search_usine(*root, usine_id_key);

    if (usine == NULL) {
        // Cas d'erreur très improbable si insert_usine/search_usine fonctionnent
        fprintf(stderr, "Erreur: Impossible de retrouver le nœud usine après insertion.\n");
        return -1;
    }

    // Accumulation des volumes (Logique Métier)
    if (troncon->type == TYPE_USINE) {
        // Mettre à jour la capacité maximale
        if (troncon->volume_or_capacity > 0) {
            usine->max_capacity = (long long)troncon->volume_or_capacity;
        }
    } else if (troncon->type == TYPE_CAPTAGE) {
        // Accumuler les volumes captés et traités
        if (troncon->volume_or_capacity > 0) {
            long long volume_capté = (long long)troncon->volume_or_capacity;
            
            usine->total_captured += volume_capté;
            
            // Calcul du volume réel traité après fuites (Tâche 151)
            // Fuites en % sont en Col 5
            if (troncon->leak_percentage >= 0) {
                float fuite_percent = troncon->leak_percentage / 100.0f;
                usine->real_treated += (long long)(volume_capté * (1.0f - fuite_percent));
            } else {
                 // Si fuite_percentage est '-', on suppose 0% de perte ou on applique la règle spécifique du projet.
                 // Ici, on considère que si le pourcentage est inconnu, on prend le volume capté par défaut.
                 usine->real_treated += volume_capté;
            }
        }
    }
    
    return 0;
}



void traverse_and_write(AVL_Node_Usine_t *node, FILE *fp, const char *mode) {
    if (node == NULL) {
        return;
    }

    // 1. Parcours à droite (pour ordre alphabétique inverse)
    traverse_and_write(node->right, fp, mode);

    // 2. Traitement du Nœud courant : PAS DE VÉRIFICATION > 0
    long long value_km3 = 0;

    // Récupérer la valeur à afficher
    if (strcmp(mode, "max") == 0) {
        value_km3 = node->max_capacity;
    } else if (strcmp(mode, "src") == 0) {
        value_km3 = node->total_captured;
    } else if (strcmp(mode, "real") == 0) {
        value_km3 = node->real_treated;
    }
    
    // Conversion critique : k.m3 -> M.m3
    double volume_Mm3 = (double)value_km3 / 1000.0;
    
    // Écriture de la ligne de données (écrit toujours la ligne)
    fprintf(fp, "%s;%.3f\n", node->id, volume_Mm3);

    // 3. Parcours à gauche
    traverse_and_write(node->left, fp, mode);
}


int write_histo_results(AVL_Node_Usine_t *root, const char *mode) {
    // Écrire directement vers la sortie standard (redirigée par le Shell)
    FILE *fp = stdout; 
    
    // 1. Écriture de l'en-tête (M.m3)
    fprintf(fp, "identifier;");

    if (strcmp(mode, "max") == 0) {
        fprintf(fp, "max volume (M.m3.year-1)\n"); // Unités corrigées
    } else if (strcmp(mode, "src") == 0) {
        fprintf(fp, "source volume (M.m3.year-1)\n"); // Unités corrigées
    } else if (strcmp(mode, "real") == 0) {
        fprintf(fp, "real volume (M.m3.year-1)\n"); // Unités corrigées
    } else {
        fprintf(stderr, "Erreur: Mode d'histogramme invalide.\n");
        return 1;
    }

    // 2. Parcours de l'AVL (utilise traverse_and_write corrigé ci-dessous)
    traverse_and_write(root, fp, mode);

    // Retour 0, le Shell gère le message de succès et la fermeture de stdout.
    return 0;
}
int handle_histo_data(const char *mode, const char *data_filename) {
    FILE *file;
    char line_buffer[MAX_LINE_LENGTH];
    AVL_Node_Usine_t *usine_root = NULL;
    int return_code = 0; // 0 = Succès

    // 1. Ouvrir le fichier
    // Sécurité A-Z : Ouvrir le fichier ou l'entrée standard
    if (strcmp(data_filename, "-") == 0) {
        // Simuler la lecture depuis stdin (pipe du Shell)
        file = stdin;
    } else {
        file = fopen(data_filename, "r");
        if (file == NULL) {
            perror("Erreur: Impossible d'ouvrir le fichier de données");
            return 1; // Code d'erreur strictement positif
        }
    }
    
    // 2. Boucle de lecture et traitement
    while (fgets(line_buffer, MAX_LINE_LENGTH, file) != NULL) {
        Troncon_CSV_t troncon;
        
        if (parse_csv_line(line_buffer, &troncon) == 0) {
            // Utiliser le parseur sur la ligne lue
            if (update_usine_metrics(&usine_root, &troncon) != 0) {
                return_code = 2; // Erreur de traitement interne
                break;
            }
        }
        // Ignorer les lignes trop longues ou mal formées
    }
    
    // 3. Fermer le flux (Sécurité A-Z)
    if (file != stdin && file != NULL) {
        fclose(file);
    }
    
    // 4. Écrire les résultats et nettoyer la mémoire
    if (return_code == 0) {
        // Tâche 3.3 : Écrire les résultats
        return_code = write_histo_results(usine_root, mode);
    }

    // 5. Libérer l'AVL (Sécurité A-Z)
    free_usine_avl(usine_root);
    
    return return_code;
}
