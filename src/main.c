#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../headers/histo_handler.h"
#include "../headers/leaks_handler.h" 

int main(int argc, char *argv[]) {
    //validate minimum argument count 
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <command> [args...]\n", argv[0]);
        return 1;
    }
    
    char *command = argv[1];
    
    // process "histo" command: check the args and call specific handler
    if (strcmp(command, "histo") == 0) {
        if (argc < 4) {
            fprintf(stderr, "Error: Missing arguments for histo (mode or file).\n");
            return 1;
        }
        char *mode = argv[2];
        char *filename = argv[3];
        return handle_histo_data(mode, filename);
    } 
    
    //process "leaks" command:check args and call specific handler 
    else if (strcmp(command, "leaks") == 0) {
        if (argc < 4) {
            fprintf(stderr, "Error: Missing arguments for leaks (Factory ID or file).\n");
            return 1;
        }
        char *facility_id = argv[2];
        char *filename = argv[3];
        int ret = handle_leaks_data(facility_id, filename);
        return ret;
    }
        //handle unknown commands 
    else {
        fprintf(stderr, "Error: Unknown command '%s'.\n", command);
        return 1;
    }
    return 0;
}
