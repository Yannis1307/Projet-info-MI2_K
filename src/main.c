#include <stdio.h>
#include <string.h>
#include "histo_handler.h" 


int main(int argc, char *argv[]) {
    // 1. Vérification minimale
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <commande> [args...]\n", argv[0]);
        return 1;
    }

    const char *command = argv[1];

    // 2. Aiguillage vers HISTO
    if (strcmp(command, "histo") == 0) {
        // Le Shell envoie : ./wildwater histo <mode> <fichier>
        // argv[0]=prog, argv[1]=histo, argv[2]=mode, argv[3]=fichier
        if (argc < 4) {
            fprintf(stderr, "Erreur: Arguments manquants pour histo (mode ou fichier).\n");
            return 1;
        }
        const char *mode = argv[2];
        const char *filename = argv[3];

        return handle_histo_data(mode, filename);
    } 
    // 3. Aiguillage vers LEAKS (Structure vide pour l'instant)
    else if (strcmp(command, "leaks") == 0) {
        printf("Mode 'leaks' en cours de construction.\n");
        return 0; 
    }
    else {
        fprintf(stderr, "Erreur: Commande '%s' inconnue.\n", command);
        return 1;
    }

    return 0;
}
