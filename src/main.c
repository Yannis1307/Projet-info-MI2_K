#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../headers/histo_handler.h"
#include "../headers/leaks_handler.h" 

int main(int argc, char *argv[]) {
    // Basic Argument Check
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <command> [args...]\n", argv[0]);
        return 1;
    }
    
    const char *command = argv[1];
    
    // HISTO
    if (strcmp(command, "histo") == 0) {
        if (argc < 4) {
            fprintf(stderr, "Error: Missing arguments for histo (mode or file).\n");
            return 1;
        }
        const char *mode = argv[2];
        const char *filename = argv[3];
        return handle_histo_data(mode, filename);
    } 
    
    // LEAKS
    else if (strcmp(command, "leaks") == 0) {
        if (argc < 4) {
            fprintf(stderr, "Error: Missing arguments for leaks (Factory ID or file).\n");
            return 1;
        }
        const char *facility_id = argv[2];
        const char *filename = argv[3];
        int ret = handle_leaks_data(facility_id, filename);
        return ret;
    }
    else {
        fprintf(stderr, "Error: Unknown command '%s'.\n", command);
        return 1;
    }
    return 0;
}
