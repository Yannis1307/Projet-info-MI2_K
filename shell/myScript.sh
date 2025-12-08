#!/bin/bash

# --- 1. Chronométrage : Début ---
start_time=$(date +%s)

# Nom de l'exécutable
executable="./wildwater"

# --- 2. Vérification de l'exécutable (Sujet Page 9) ---
if [ ! -x "$executable" ]; then
    echo "Attention : L'exécutable $executable est introuvable."
    echo "Tentative de compilation..."
    make
    # On revérifie après la compilation
    if [ ! -x "$executable" ]; then
        echo "Erreur fatale : Impossible de compiler ou de trouver wildwater."
        exit 1
    fi
    echo "Compilation réussie."
fi

# --- 3. Vérification des arguments (TD 09) ---
if [ $# -lt 1 ]; then
    echo "Erreur : pas assez d'arguments"
    echo "Usage : $0 histo [max|src|real] <fichier_csv>"
    echo "Usage : $0 leaks \"<id_usine>\" <fichier_csv>"
    exit 1
fi

cmd="$1"

# --- 4. Aiguillage et Traitement ---

if [ "$cmd" = "histo" ]; then
    # Vérification arguments spécifiques histo
    if [ $# -ne 3 ]; then
        echo "Erreur : histo attend 2 arguments : [max|src|real] <fichier_csv>"
        exit 1
    fi
    mode="$2"
    csv="$3"

    # Vérification fichier CSV existe
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
    
    # Appel filtré (Optimisation TD 08)
    # On filtre (grep) puis on envoie au programme C via le pipe (|)
    # Le tiret (-) dit au C de lire l'entrée standard
    
    # COMMANDE (À décommenter quand le C sera prêt) :
    # grep -E "Facility|Spring" "$csv" | $executable "histo" "$mode" - > "histo_$mode.dat"
    
    # Vérification du code retour du C (Sujet Page 9)
    if [ $? -ne 0 ]; then
        echo "Erreur lors de l'exécution du programme C."
        exit 1
    fi

    # Génération Graphique (À décommenter plus tard)
    # gnuplot -e "inputname='histo_$mode.dat'; outputname='histo_$mode.png'" shell/histo.gnu
    
    echo "Traitement Histo terminé."

elif [ "$cmd" = "leaks" ]; then
    # Vérification arguments spécifiques leaks
    if [ $# -ne 3 ]; then
        echo "Erreur : leaks attend 2 arguments : \"<id_usine>\" <fichier_csv>"
        exit 1
    fi
    facility="$2"
    csv="$3"

    # Vérification fichier CSV existe
    if [ ! -f "$csv" ]; then
        echo "Erreur : Le fichier $csv n'existe pas."
        exit 1
    fi

    echo "--- MODE FUITES ---"

    # Appel filtré (Optimisation TD 08)
    # On ne garde que les lignes de l'usine concernée
    
    # COMMANDE (À décommenter quand le C sera prêt) :
    # grep "$facility" "$csv" | $executable "leaks" "$facility" - > "leaks.dat"

    # Vérification du code retour du C
    if [ $? -ne 0 ]; then
        echo "Erreur lors de l'exécution du programme C."
        exit 1
    fi

    echo "Traitement Leaks terminé."

else
    echo "Erreur : commande inconnue : $cmd"
    exit 1
fi

# --- 5. Chronométrage : Fin et Affichage (Sujet Page 9) ---
end_time=$(date +%s)
duration=$((end_time - start_time))
echo "Durée totale du traitement : $duration secondes."
