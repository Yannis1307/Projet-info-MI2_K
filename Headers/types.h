#ifndef TYPES_H
#define TYPES_H

#include <string.h>


#define MAX_ID_LEN 64 

typedef struct AVL_Node_Usine {
    char id[MAX_ID_LEN];            
    long long max_capacity;         
    long long total_captured;       
    long long real_treated;         
    struct AVL_Node_Usine *left;
    struct AVL_Node_Usine *right;
    int height;
} AVL_Node_Usine_t;


typedef struct AdjNode {
    char dest_id[MAX_ID_LEN]; 
    float leak_percentage;    
    struct AdjNode *next;     
} AdjNode_t;

typedef struct Station_Node {
    char id[MAX_ID_LEN];      
    long long capacity;       
    AdjNode_t *adj_head;      
    struct Station_Node *left;
    struct Station_Node *right;
    int height;
} Station_Node_t;

typedef enum {
    TYPE_UNKNOWN = 0,
    TYPE_USINE,
    TYPE_CAPTAGE,        
    TYPE_STOCKAGE_ENTRANT, 
    TYPE_DISTRIB_PRINC,  
    TYPE_DISTRIB_SECON,  
    TYPE_BRANCHEMENT     
} LineType_e;

typedef struct {
    char usine_id[MAX_ID_LEN];       
    char amont_id[MAX_ID_LEN];       
    char aval_id[MAX_ID_LEN];        
    float volume_or_capacity;        
    float leak_percentage;           
    LineType_e type;                 
} Troncon_CSV_t;

#endif
