#include "../headers/leaks_handler.h"
#include "../headers/csv_parser.h"
#include <string.h>
#include <ctype.h>
#include <math.h> 
// Find the max of 2 numbers
#define max(a, b) ((a > b) ? a : b)

//String cleaning to remove space at the beginning and the end of a string
void trim_string(char *str) {
    if (str == NULL) return;
    int len = strlen(str);
    //remove at the end
    while (len > 0 && isspace((unsigned char)str[len - 1])) {
        str[len - 1] = '\0';
        len--;
    }
    //Remove at the start
    int start = 0;
    while (str[start] != '\0' && isspace((unsigned char)str[start])) {
        start++;
    }
    //Fill the space
    if (start > 0) {
        int i = 0;
        while (str[start + i] != '\0') {
            str[i] = str[start + i];
            i++;
        }
        str[i] = '\0';
    }
}

//AVL management
// creates a new station node in memory

Station_Node_t *new_station_node(char *id) {
    Station_Node_t *node = (Station_Node_t *)malloc(sizeof(Station_Node_t));
    if (!node) {
        fprintf(stderr, "\nFatal Error: Out of memory (malloc failed).\n");
        exit(1); 
    }
    strncpy(node->id, id, MAX_ID_LEN - 1);
    node->id[MAX_ID_LEN - 1] = '\0';
    node->capacity = 0;
    node->adj_head = NULL;
    node->left = NULL;
    node->right = NULL;
    node->height = 1;
    return node;
}

int get_station_height(Station_Node_t *n) { return (n == NULL) ? 0 : n->height; }
int get_station_balance(Station_Node_t *n) { return (n == NULL) ? 0 : get_station_height(n->right) - get_station_height(n->left); }

Station_Node_t *rotate_station_right(Station_Node_t *y) {
    if (y == NULL || y->left == NULL) return y;
    Station_Node_t *x = y->left;
    Station_Node_t *T2 = x->right;
    //perform rotation
    x->right = y;
    y->left = T2;
    //update
    y->height = max(get_station_height(y->left), get_station_height(y->right)) + 1;
    x->height = max(get_station_height(x->left), get_station_height(x->right)) + 1;
    return x;
}

Station_Node_t *rotate_station_left(Station_Node_t *x) {
    if (x == NULL || x->right == NULL) return x;
    Station_Node_t *y = x->right;
    Station_Node_t *T2 = y->left;
    y->left = x;
    x->right = T2;
    x->height = max(get_station_height(x->left), get_station_height(x->right)) + 1;
    y->height = max(get_station_height(y->left), get_station_height(y->right)) + 1;
    return y;
}
//Inserts a station into the AVL tree

Station_Node_t *insert_station(Station_Node_t *node,char *id) {
    if (node == NULL) return new_station_node(id);
    
    int cmp = strcmp(id, node->id);
    if (cmp < 0) node->left = insert_station(node->left, id);
    else if (cmp > 0) node->right = insert_station(node->right, id);
    else return node; //node already exist 

    //update height    
    node->height = 1 + max(get_station_height(node->left), get_station_height(node->right));
    int balance = get_station_balance(node);
    //If unbalanced:
    if (balance < -1 && node->left != NULL) {
        if (strcmp(id, node->left->id) < 0) return rotate_station_right(node);
        if (strcmp(id, node->left->id) > 0) {
            node->left = rotate_station_left(node->left);
            return rotate_station_right(node);
        }
    }
    if (balance > 1 && node->right != NULL) {
        if (strcmp(id, node->right->id) > 0) return rotate_station_left(node);
        if (strcmp(id, node->right->id) < 0) {
            node->right = rotate_station_right(node->right);
            return rotate_station_left(node);
        }
    }
    return node;
}

//searches for a station by ID
Station_Node_t *search_station(Station_Node_t *root,char *id) {
    if (root == NULL) return NULL;
    int cmp = strcmp(id, root->id);
    if (cmp == 0) return root;//found
    if (cmp < 0) return search_station(root->left, id);
    return search_station(root->right, id);
}

//recursively frees the graph memory
void free_station_graph(Station_Node_t *root) {
    if (root == NULL) return;
    free_station_graph(root->left);
    free_station_graph(root->right);
    AdjNode_t *current = root->adj_head;
    while (current != NULL) {
        AdjNode_t *temp = current;
        current = current->next;
        free(temp);
    }
    free(root);
}

//adds a pipe connection from station to dest_id
void add_pipe(Station_Node_t *station,char *dest_id, float leak) {
    if (station == NULL) return;
    AdjNode_t *new_adj = (AdjNode_t *)malloc(sizeof(AdjNode_t));
    if (!new_adj) {
         fprintf(stderr, "\nFatal Error: RAM saturated in add_pipe.\n");
         exit(1);
    }
    strncpy(new_adj->dest_id, dest_id, MAX_ID_LEN - 1);
    new_adj->leak_percentage = leak;
    new_adj->next = station->adj_head; 
    station->adj_head = new_adj;
}

//Recursive calculation

double calculate_downstream_loss(Station_Node_t *root, Station_Node_t *current_node, double input_volume) {
    //Stop condition is Node doesn't exist or no water left
    if (current_node == NULL || input_volume <= 0.0) return 0.0;

    double total_loss_here = 0.0;
    
    //Count children
    int children_count = 0;
    AdjNode_t *curr = current_node->adj_head;
    while (curr) {
        children_count++;
        curr = curr->next;
    }

    if (children_count == 0) return 0.0;

    //Fair split
    double volume_per_child = input_volume / (double)children_count;

    //visit children
    curr = current_node->adj_head;
    while (curr) {
        //calculate the leak on this pipe
        double pipe_loss = 0.0;
        if (curr->leak_percentage > 0) {
            pipe_loss = volume_per_child * (curr->leak_percentage / 100.0);
        }
        
        double remaining_volume = volume_per_child - pipe_loss;
        //find the next station
        Station_Node_t *next_node = search_station(root, curr->dest_id);
        //Recursion
        double downstream_loss = calculate_downstream_loss(root, next_node, remaining_volume);
        total_loss_here += pipe_loss + downstream_loss;

        curr = curr->next;
    }
    return total_loss_here;
}

//Main function for the leak command
int handle_leaks_data(char *target_factory_id,char *input_source) {
    //Open the CSV file
    FILE *file = (strcmp(input_source, "-") == 0) ? stdin : fopen(input_source, "r");
    if (!file) { perror("Error opening file"); return 1; }

    char line[MAX_LINE_LENGTH];
    Station_Node_t *graph_root = NULL;//root
    //spaces removing
    char clean_target_id[MAX_ID_LEN];
    strncpy(clean_target_id, target_factory_id, MAX_ID_LEN - 1);
    clean_target_id[MAX_ID_LEN-1] = '\0';
    trim_string(clean_target_id);

    int line_count = 0; 
    fprintf(stderr, "Loading network into memory...\n");
    //loop through the file till the end of the file
    while (fgets(line, MAX_LINE_LENGTH, file)) {
        
        line_count++;
        if (line_count % 200000 == 0) {
            fprintf(stderr, "\rLines processed: %d...", line_count);//progress display
        }

        CSV_Section_t s;
        //Parse the CSV line into a structure
        if (parse_csv_line(line, &s) == 0) {
            trim_string(s.plant_id);
            trim_string(s.upstream_id);
            trim_string(s.downstream_id);

            //Source Management
            if (s.volume_or_capacity > 0) {
                if (s.type == TYPE_PLANT) {
                    graph_root = insert_station(graph_root, s.plant_id);
                    Station_Node_t *st = search_station(graph_root, s.plant_id);
                    if (st) st->capacity = (long long)s.volume_or_capacity;
                }
                else if (strcmp(s.downstream_id, clean_target_id) == 0) {
                     graph_root = insert_station(graph_root, s.downstream_id);
                     Station_Node_t *st = search_station(graph_root, s.downstream_id);
                     if (st) st->capacity += (long long)s.volume_or_capacity;
                }
            }

            //Pipe Management
            if (s.upstream_id[0] != '\0' && s.downstream_id[0] != '\0') {
                graph_root = insert_station(graph_root, s.upstream_id);
                Station_Node_t *src = search_station(graph_root, s.upstream_id);
                //add pipe connection
                if (src) {
                    float leak = (s.leak_percentage < 0) ? 0.0f : s.leak_percentage;
                    add_pipe(src, s.downstream_id, leak);
                }
                graph_root = insert_station(graph_root, s.downstream_id);
            }
        }
    }
    fprintf(stderr, "\nLoading finished (%d lines). Starting calculation...\n", line_count);
    if (file != stdin) fclose(file);

    Station_Node_t *target = search_station(graph_root, clean_target_id);//find the factory
    //run calculation
    if (target == NULL || target->capacity == 0) {
        printf("-1\n"); 
    } else {
        double initial_volume = (double)target->capacity;
        double total_loss = calculate_downstream_loss(graph_root, target, initial_volume);
        
        //result in M.m3
        printf("%.6f\n", total_loss / 1000000.0);
    }

    free_station_graph(graph_root);//cleaning
    return 0;
}
