#!/bin/bash

# --- 1. Chronométrage : Début ---
start_time=$(date +%s)

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

# --- 3. Vérification des arguments (Conforme TD 09) ---
if [ $# -lt 1 ]; then
    echo "Erreur : pas assez d'arguments"
    echo "Usage : $0 histo [max|src|real] <fichier_csv>"
    echo "Usage : $0 leaks \"<id_usine>\" <fichier_csv>"
    exit 1
fi

cmd="$1"

# --- 4. Aiguillage et Traitement ---

if [ "$cmd" = "histo" ]; then
    # Vérification arguments
    if [ $# -ne 3 ]; then
        echo "Erreur : histo attend 2 arguments : [max|src|real] <fichier_csv>"
        exit 1
    fi
    mode="$2"
    csv="$3"

    # Vérification fichier
    if [ ! -f "$csv" ]; then
        echo "Erreur : Le fichier $csv n'existe pas."
        exit 1
    fi

    # Vérification mode valide
    if [ "$mode" != "max" ] && [ "$mode" != "src" ] && [ "$mode" != "real" ]; then
        echo "Erreur : mode invalide ($mode). Attendus : max, src, real."
        exit 1
    fi

    echo "--- MODE HISTOGRAMME ---"
    
    # NOUVELLE CONSIGNE : Filtrage avec AWK (Colonne 5 vide ou "-")
    # NR>1 : On ignore la première ligne (en-tête)
    # $5 : On regarde la 5ème colonne
    # Le tiret "-" final dit au programme C de lire l'entrée standard (le pipe)
    
    awk -F";" 'NR>1 && ($5=="-" || $5=="") {print $0}' "$csv" | $executable "histo" "$mode" - > "histo_$mode.dat"
    
    # Vérification retour C
    if [ $? -ne 0 ]; then
        echo "Erreur lors de l'exécution du programme C."
        exit 1
    fi

    # Génération Graphique (Si le .gnu existe et que le C a généré des données)
    if [ -f "shell/histo.gnu" ] && [ -s "histo_$mode.dat" ]; then
        gnuplot -e "inputname='histo_$mode.dat'; outputname='histo_$mode.png'" shell/histo.gnu
        echo "Graphique généré : histo_$mode.png"
    fi
    
    echo "Traitement Histo terminé."

elif [ "$cmd" = "leaks" ]; then
    # Vérification arguments
    if [ $# -ne 3 ]; then
        echo "Erreur : leaks attend 2 arguments : \"<id_usine>\" <fichier_csv>"
        exit 1
    fi
    facility="$2"
    csv="$3"

    if [ ! -f "$csv" ]; then
        echo "Erreur : Le fichier $csv n'existe pas."
        exit 1
    fi

    echo "--- MODE FUITES ---"

    # NOUVELLE CONSIGNE : Filtrage précis Colonne 3 (ID) et 5 (Fuites)
    # -v id="$facility" : On passe l'argument bash à awk
    # $3==id : La colonne 3 doit correspondre à l'usine
    # $5!="-" && $5!="" : La colonne 5 ne doit pas être vide (car on cherche les fuites)
    
    awk -F";" -v id="$facility" 'NR>1 && $3==id && $5!="-" && $5!="" {print $0}' "$csv" | $executable "leaks" "$facility" - > "leaks.dat"

    # Vérification retour C
    if [ $? -ne 0 ]; then
        echo "Erreur lors de l'exécution du programme C."
        exit 1
    fi

    echo "Traitement Leaks terminé. Fichier : leaks.dat"

else
    echo "Erreur : commande inconnue : $cmd"
    exit 1
fi

# --- 5. Chronométrage : Fin ---
end_time=$(date +%s)
duration=$((end_time - start_time))
echo "Durée totale du traitement : $duration secondes."
