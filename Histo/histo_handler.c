#include "../headers/histo_handler.h"
#include "../headers/csv_parser.h" 

#define BUFFER_SIZE 1024

/*
 * Data Aggregation Logic
 * Identifies whether a CSV section refers to a Plant or an Intake,
 * retrieves/creates the corresponding node in the AVL tree, and updates
 * its metrics (Capacity or Captured Volume).
 */
int update_plant_metrics(AVL_Plant_Node_t **root, CSV_Section_t *section) {
    AVL_Plant_Node_t *plant = NULL;
    char *plant_id_key = NULL;
    
    // Determine the relevant Plant ID based on section type
    if (section->type == TYPE_PLANT) {
        plant_id_key = section->upstream_id;
    } else if (section->type == TYPE_INTAKE) {
        plant_id_key = section->downstream_id;
    } else {  
        return 0; // Ignore irrelevant types (pipes, etc.)
    }
    
    if (plant_id_key[0] == '\0') {
        return 0; 
    }
    
    // AVL Node Retrieval
    // Ensures the node exists in the tree before updating data.
    *root = insert_plant(*root, plant_id_key);
    plant = search_plant(*root, plant_id_key);
    
    if (plant == NULL) {
        fprintf(stderr, "Error: Could not retrieve plant node after insertion.\n");
        return -1;
    }
    
    // Metrics Update Logic
    // TYPE_PLANT: Sets the defined maximum capacity.
    // TYPE_INTAKE: Accumulates captured volume and calculates real treated volume (accounting for leaks).
    if (section->type == TYPE_PLANT) {
        if (section->volume_or_capacity > 0) {
            plant->max_capacity = (long long)section->volume_or_capacity;
        }
    } else if (section->type == TYPE_INTAKE) {
        if (section->volume_or_capacity > 0) {
            long long volume_captured = (long long)section->volume_or_capacity;
            plant->total_captured += volume_captured;
            
            // Calculate real volume: Total - Leak Percentage
            if (section->leak_percentage >= 0) {
                float leak_percent = section->leak_percentage / 100.0f;
                plant->real_treated += (long long)(volume_captured * (1.0f - leak_percent));
            } else {
                 plant->real_treated += volume_captured;
            }
        }
    }
    return 0;
}

/*
 * Recursive Tree Traversal
 * Traverses the AVL tree (Right -> Root -> Left) to output data in descending order.
 * Outputs the selected metric (max, src, or real) converted to Mm3.
 */
void traverse_and_write(AVL_Plant_Node_t *node, FILE *fp, char *mode) {
    if (node == NULL) {
        return;
    }
    traverse_and_write(node->right, fp, mode);
    
    long long value_km3 = 0;
    
    // Metric Selection based on mode
    if (strcmp(mode, "max") == 0) {
        value_km3 = node->max_capacity;
    } else if (strcmp(mode, "src") == 0) {
        value_km3 = node->total_captured;
    } else if (strcmp(mode, "real") == 0) {
        value_km3 = node->real_treated;
    }
    
    // Unit Conversion (km3 to Mm3) and Output
    double volume_Mm3 = (double)value_km3 / 1000.0;
    fprintf(fp, "%s;%.3f\n", node->id, volume_Mm3);
    
    traverse_and_write(node->left, fp, mode);
}

/*
 * Output Generation
 * Writes the CSV header appropriate for the selected mode and initiates
 * the recursive tree traversal to write the body of the data.
 */
int write_histo_results(AVL_Plant_Node_t *root, char *mode) {
    FILE *fp = stdout; // Default output to standard output for piping
    fprintf(fp, "identifier;");
    
    // Header Selection
    if (strcmp(mode, "max") == 0) {
        fprintf(fp, "max volume (M.m3.year-1)\n"); 
    } else if (strcmp(mode, "src") == 0) {
        fprintf(fp, "source volume (M.m3.year-1)\n"); 
    } else if (strcmp(mode, "real") == 0) {
        fprintf(fp, "real volume (M.m3.year-1)\n"); 
    } else {
        fprintf(stderr, "Error: Invalid histogram mode.\n");
        return 1;
    }

    traverse_and_write(root, fp, mode);
    return 0;
}

/*
  * Main Handler for 'histo' Command
 * Coordinates file opening (or stdin), CSV parsing, data aggregation via AVL tree,
 * result writing, and memory cleanup.
 */
int handle_histo_data(char *mode, char *data_filename) {
    FILE *file;
    char line_buffer[MAX_LINE_LENGTH];
    AVL_Plant_Node_t *plant_root = NULL;
    int return_code = 0; 
    
    //Input Stream Setup
    if (strcmp(data_filename, "-") == 0) {
        file = stdin;
    } else {
        file = fopen(data_filename, "r");
        if (file == NULL) {
            perror("Error: Cannot open data file");
            return 1; 
        }
    }
    
    //Line-by-Line Processing
    //parses CSV lines and updates the corresponding AVL node metrics.
    while (fgets(line_buffer, MAX_LINE_LENGTH, file) != NULL) {
        CSV_Section_t section;
        
        if (parse_csv_line(line_buffer, &section) == 0) {
            if (update_plant_metrics(&plant_root, &section) != 0) {
                return_code = 2; 
                break;
            }
        }
    }
    
    if (file != stdin && file != NULL) {
        fclose(file);
    }
    
    //result Output and Cleanup
    if (return_code == 0) {
        return_code = write_histo_results(plant_root, mode);
    }
    
    free_plant_avl(plant_root);
    
    return return_code;
}
