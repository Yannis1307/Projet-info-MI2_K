#!/bin/bash

# 1. Vérification des arguments (TD 09)
if [ $# -lt 1 ]; then
    echo "Erreur : pas assez d'arguments"
    echo "Usage : $0 histo [max|src|real] <fichier_csv>"
    echo "Usage : $0 leaks \"<id_usine>\" <fichier_csv>"
    exit 1
fi

cmd="$1"

# 2. Aiguillage et Filtrage (TD 08)

if [ "$cmd" = "histo" ]; then
    if [ $# -ne 3 ]; then
        echo "Erreur : histo attend 2 arguments : [max|src|real] <fichier_csv>"
        exit 1
    fi
    mode="$2"
    csv="$3"

    # Vérification mode
    if [ "$mode" != "max" ] && [ "$mode" != "src" ] && [ "$mode" != "real" ]; then
        echo "Erreur : mode invalide"
        exit 1
    fi

    echo "--- MODE HISTOGRAMME ---"
    
    
    # Pour l'instant, on laisse l'appel commenté en attendant le programme C :
    # grep -E "Facility|Spring" "$csv" | ./wildwater "histo" "$mode" - > "histo_$mode.dat"
    
    # On lance gnuplot 
    # gnuplot -e "inputname='histo_$mode.dat'; outputname='histo_$mode.png'" shell/histo.gnu

    echo "Commande prête (filtrage configuré)."


elif [ "$cmd" = "leaks" ]; then
    if [ $# -ne 3 ]; then
        echo "Erreur : leaks attend 2 arguments..."
        exit 1
    fi
    facility="$2"
    csv="$3"

    echo "--- MODE FUITES ---"

    # APPEL AU PROGRAMME C AVEC FILTRAGE (TD 08)
    # On filtre uniquement l'usine demandée ($facility)
    
    # Pour l'instant, on laisse l'appel commenté :
    # grep "$facility" "$csv" | ./wildwater "leaks" "$facility" - > "leaks.dat"
    
    echo "Commande prête (filtrage configuré sur $facility)."

else
    echo "Erreur : commande inconnue : $cmd"
    exit 1
fi