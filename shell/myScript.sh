#!/bin/bash

# 1. Vérification globale (TD 09 Ex 1.3 et 3.2)
if [ $# -lt 1 ]; then
    echo "Erreur : pas assez d'arguments"
    echo "Usage : $0 histo [max|src|real] <fichier_csv>"
    echo "Usage : $0 leaks \"<id_usine>\" <fichier_csv>"
    exit 1 # Code d'erreur (TD 09 Ex 3.4)
fi

cmd="$1"

# 2. Aiguillage
if [ "$cmd" = "histo" ]; then
    # histo <mode> <csv> -> Il faut donc 3 arguments au total ($1, $2, $3)
    if [ $# -ne 3 ]; then
        echo "Erreur : histo attend 2 arguments après la commande : [max|src|real] <fichier_csv>"
        exit 1
    fi

    # On utilise directement $2 et $3 au lieu de décaler
    mode="$2"
    csv="$3"

    # Vérification du mode (TD 09 Ex 3)
    if [ "$mode" != "max" ] && [ "$mode" != "src" ] && [ "$mode" != "real" ]; then
        echo "Erreur : mode invalide pour histo : $mode"
        echo "Modes possibles : max, src, real"
        exit 1
    fi

    echo "--- MODE HISTOGRAMME ---"
    echo "Mode : $mode"
    echo "Fichier : $csv"
    
    # Appel futur (commenté)
    # ./wildwater "$cmd" "$mode" "$csv"

elif [ "$cmd" = "leaks" ]; then
    # leaks <id> <csv> -> Il faut aussi 3 arguments au total
    if [ $# -ne 3 ]; then
        echo "Erreur : leaks attend 2 arguments après la commande : \"<id_usine>\" <fichier_csv>"
        exit 1
    fi

    facility="$2"
    csv="$3"

    echo "--- MODE FUITES ---"
    echo "Usine : $facility"
    echo "Fichier : $csv"

    # Appel futur (commenté)
    # ./wildwater "$cmd" "$facility" "$csv"

else
    echo "Erreur : commande inconnue : $cmd"
    exit 1
fi

# Note : Le sujet demande d'afficher la durée à la fin (Page 9), on verra ça plus tard.