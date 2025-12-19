#ifndef TYPES_H
#define TYPES_H

#include <string.h>

#define MAX_ID_LEN 64 

typedef struct AVL_Plant_Node {
    char id[MAX_ID_LEN];            
    long long max_capacity;         
    long long total_captured;       
    long long real_treated;         
    struct AVL_Plant_Node *left;
    struct AVL_Plant_Node *right;
    int height;
} AVL_Plant_Node_t;

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
    TYPE_PLANT,
    TYPE_INTAKE,        
    TYPE_STORAGE_IN, 
    TYPE_DISTRIB_MAIN,  
    TYPE_DISTRIB_SEC,  
    TYPE_BRANCHMENT     
} LineType_e;

typedef struct {
    char plant_id[MAX_ID_LEN];       
    char upstream_id[MAX_ID_LEN];       
    char downstream_id[MAX_ID_LEN];        
    float volume_or_capacity;        
    float leak_percentage;           
    LineType_e type;                 
} CSV_Section_t;

#endif
