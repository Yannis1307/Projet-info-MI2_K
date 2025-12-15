#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../headers/histo_handler.h"
#include "../headers/leaks_handler.h" 

int main(int argc, char *argv[]) {
    // Vérification arguments
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <commande> [args...]\n", argv[0]);
        return 1;
    }
    
    const char *command = argv[1];
    //HISTO
    if (strcmp(command, "histo") == 0) {
        if (argc < 4) {
            fprintf(stderr, "Erreur: Arguments manquants pour histo (mode ou fichier).\n");
            return 1;
        }
        const char *mode = argv[2];
        const char *filename = argv[3];
        return handle_histo_data(mode, filename);
    } 
    
    //LEAKS
    else if (strcmp(command, "leaks") == 0) {
        
        if (argc < 4) {
            fprintf(stderr, "Erreur: Arguments manquants pour leaks (ID usine ou fichier).\n");
            return 1;
        }
        const char *facility_id = argv[2];
        const char *filename = argv[3];
        int ret = handle_leaks_data(facility_id, filename);
        return ret;
    }
    else {
        fprintf(stderr, "Erreur: Commande '%s' inconnue.\n", command);
        return 1;
    }
    return 0;
}
