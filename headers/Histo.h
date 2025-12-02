
typedef struct AVL_Node_Usine {
    char id[MAX_ID_LEN];            
    long long max_capacity;         
    long long total_captured;       
    long long real_treated;         
    
    
    struct AVL_Node_Usine *left;
    struct AVL_Node_Usine *right;
    int height;
} AVL_Node_Usine_t;
