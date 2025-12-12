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

# --- 3. Vérification des arguments (Ordre : Fichier CSV en premier) ---
# Adapté à la logique de ton équipe
if [ $# -lt 2 ]; then
    echo "Erreur : pas assez d'arguments"
    echo "Usage : $0 <fichier_csv> histo [max|src|real]"
    echo "Usage : $0 <fichier_csv> leaks \"<id_usine>\""
    exit 1
fi

csv="$1"
cmd="$2"

# --- 4. Aiguillage et Traitement ---

if [ "$cmd" = "histo" ]; then
    if [ $# -ne 3 ]; then
        echo "Erreur : histo attend 3 arguments au total."
        exit 1
    fi
    mode="$3"

    if [ ! -f "$csv" ]; then
        echo "Erreur : Le fichier $csv n'existe pas."
        exit 1
    fi

    if [ "$mode" != "max" ] && [ "$mode" != "src" ] && [ "$mode" != "real" ]; then
        echo "Erreur : mode invalide ($mode). Attendus : max, src, real."
        exit 1
    fi

    echo "--- MODE HISTOGRAMME : $mode ---"
    
    # CORRECTION DU FILTRE (Le point critique du rapport) :
    # On filtre sur la Colonne 1 ($1) qui doit être égale à "-"
    # Cela capture : Les USINES ET les SOURCES
    
    awk -F";" '$1=="-" && NR>1 {print $0}' "$csv" | $executable "histo" "$mode" - > "histo_$mode.dat"
    
    if [ $? -ne 0 ]; then
        echo "Erreur lors de l'exécution du programme C."
        exit 1
    fi

    # Génération Graphique
    if [ -f "shell/histo.gnu" ] && [ -s "histo_$mode.dat" ]; then
        gnuplot -e "inputname='histo_$mode.dat'; outputname='histo_$mode.png'" shell/histo.gnu
        echo "Graphique généré : histo_$mode.png"
    fi
    
    echo "Traitement Histo terminé."

elif [ "$cmd" = "leaks" ]; then
    if [ $# -ne 3 ]; then
        echo "Erreur : leaks attend 3 arguments au total."
        exit 1
    fi
    facility="$3"

    if [ ! -f "$csv" ]; then
        echo "Erreur : Le fichier $csv n'existe pas."
        exit 1
    fi

    echo "--- MODE FUITES ---"

    # Filtrage Leaks (Colonne 3 = ID et Colonne 5 non vide)
    awk -F";" -v id="$facility" '$3==id && $5!="-" && $5!="" {print $0}' "$csv" | $executable "leaks" "$facility" - > "leaks.dat"

    if [ $? -ne 0 ]; then
        echo "Erreur lors de l'exécution du programme C."
        exit 1
    fi

    echo "Traitement Leaks terminé."

else
    echo "Erreur : commande inconnue : $cmd"
    exit 1
fi

# --- 5. Chronométrage : Fin (Calcul en millisecondes) ---
end_time=$(date +%s%N)
# Calcul : (Fin - Début) / 1 000 000
duration=$(( (end_time - start_time) / 1000000 ))
echo "Durée totale du traitement : $duration ms"