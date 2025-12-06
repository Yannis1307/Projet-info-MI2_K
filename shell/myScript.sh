#!/bin/bash

echo "Nom du script : $0"
echo "Nombre d'arguments : $#"

if [ $# -lt 1 ]; then
    echo "Erreur : pas assez d'arguments"
    echo "Usage :"
    echo "  $0 histo [max|src|real] <fichier_csv>"
    echo "  $0 leaks \"<id_usine>\" <fichier_csv>"
    exit 1
fi

cmd="$1"
shift

if [ "$cmd" = "histo" ]; then
    # histo [max|src|real] <fichier_csv>
    if [ $# -ne 2 ]; then
        echo "Erreur : histo attend 2 arguments : [max|src|real] <fichier_csv>"
        exit 1
    fi

    mode="$1"
    csv="$2"

    if [ "$mode" != "max" ] && [ "$mode" != "src" ] && [ "$mode" != "real" ]; then
        echo "Erreur : mode invalide pour histo : $mode"
        echo "Modes possibles : max, src, real"
        exit 1
    fi

    echo "--- MODE HISTOGRAMME ---"
    echo "Mode choisi : $mode"
    echo "Fichier CSV : $csv"

    # Plus tard : ../wildwater histo "$mode" "$csv" "histo_${mode}.dat"

elif [ "$cmd" = "leaks" ]; then
    # leaks "<id_usine>" <fichier_csv>
    if [ $# -ne 2 ]; then
        echo "Erreur : leaks attend 2 arguments : \"<id_usine>\" <fichier_csv>"
        exit 1
    fi

    facility="$1"
    csv="$2"

    echo "--- MODE FUITES ---"
    echo "Usine demandée : $facility"
    echo "Fichier CSV    : $csv"

    # Plus tard : ../wildwater leaks "$facility" "$csv" "leaks.dat"

else
    echo "Erreur : commande inconnue : $cmd (attendu : histo ou leaks)"
    exit 1
fi