#define MAX_ID_LEN 32
typedef struct AVL_Node_Usine {
    char id[MAX_ID_LEN];            
    long long max_capacity;         
    long long total_captured;       
    long long real_treated;         
    
    
    struct AVL_Node_Usine *left;
    struct AVL_Node_Usine *right;
    int height;
} AVL_Node_Usine_t;
#define MAX_ID_LEN 32  // Longueur suffisante pour les identifiants (ex: "Facility complex #RH400057F")


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
    
  
    float volume_or_capacity;        // Col 4 (Volume capté ou Capacité, peut être '-'. Utiliser -1.0f si '-')
    float leak_percentage;           // Col 5 (Pourcentage de fuite, peut être '-'. Utiliser -1.0f si '-')
    
    
    LineType_e type;                 // Le type de tronçon identifié
} Troncon_CSV_t;
