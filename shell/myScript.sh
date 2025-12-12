#!/bin/bash

# --- 1. Chronométrage : Début (En nanosecondes pour précision ms) ---
start_time=$(date +%s%N)

# Nom de l'exécutable
executable="./wildwater"

# --- 2. Vérification de l'exécutable ---
if [ ! -x "$executable" ]; then
    echo "Attention : L'exécutable $executable est introuvable."
    echo "Tentative de compilation..."
    make
    if [ ! -x "$executable" ]; then
        echo "Erreur fatale : Impossible de compiler ou de trouver wildwater."
        exit 1
    fi
    echo "Compilation réussie."
fi

# --- 3. Vérification des arguments ---
# On vérifie qu'il y a au moins 2 arguments (Fichier + Commande)
if [ $# -lt 2 ]; then
    echo "Erreur : pas assez d'arguments"
    echo "Usage : $0 <fichier_csv> histo [max|src|real]"
    echo "Usage : $0 <fichier_csv> leaks \"<id_usine>\""
    exit 1
fi

# Selon la consigne v1.1 : $1=Fichier, $2=Commande
csv="$1"
cmd="$2"

# Vérification globale du fichier avant d'aller plus loin
if [ ! -f "$csv" ]; then
    echo "Erreur : Le fichier '$csv' n'existe pas."
    exit 1
fi

# --- 4. Aiguillage et Traitement ---

if [ "$cmd" = "histo" ]; then
    # Il faut 3 arguments : Fichier Histo Mode
    if [ $# -ne 3 ]; then
        echo "Erreur : histo attend l'argument mode : [max|src|real]"
        exit 1
    fi
    mode="$3"

    if [ "$mode" != "max" ] && [ "$mode" != "src" ] && [ "$mode" != "real" ]; then
        echo "Erreur : mode invalide ($mode). Attendus : max, src, real."
        exit 1
    fi

    echo "--- MODE HISTOGRAMME : $mode ---"
    
    # Filtrage : On garde les lignes dont la Col 1 est "-" (Usines & Captages)
    # On ignore l'en-tête (NR>1)
    awk -F";" '$1=="-" && NR>1 {print $0}' "$csv" | $executable "histo" "$mode" - > "histo_$mode.dat"
    
    if [ $? -ne 0 ]; then
        echo "Erreur lors de l'exécution du programme C."
        exit 1
    fi

    # Génération graphique
    if [ -f "shell/histo.gnu" ] && [ -s "histo_$mode.dat" ]; then
        gnuplot -e "inputname='histo_$mode.dat'; outputname='histo_$mode.png'" shell/histo.gnu
        echo "Graphique généré : histo_$mode.png"
    fi
    
    echo "Traitement Histo terminé."

elif [ "$cmd" = "leaks" ]; then
    # Il faut 3 arguments : Fichier Leaks ID
    if [ $# -ne 3 ]; then
        echo "Erreur : leaks attend l'argument ID usine."
        exit 1
    fi
    
    # CORRECTION ICI : L'ID est en $3, le fichier est déjà dans $csv ($1)
    facility="$3"

    echo "--- MODE FUITES : $facility ---"

    # Filtrage Leaks (Colonne 3 = ID et Colonne 5 non vide)
    awk -F";" -v id="$facility" '$3==id && $5!="-" && $5!="" {print $0}' "$csv" | $executable "leaks" "$facility" - > "leaks.dat"

    if [ $? -ne 0 ]; then
        echo "Erreur lors de l'exécution du programme C."
        exit 1
    fi

    echo "Traitement Leaks terminé."

else
    echo "Erreur : commande inconnue : $cmd"
    echo "Commandes disponibles : histo, leaks"
    exit 1
fi

# --- 5. Chronométrage : Fin (Calcul en millisecondes) ---
end_time=$(date +%s%N)
duration=$(( (end_time - start_time) / 1000000 ))
echo "Durée totale du traitement : $duration ms"
