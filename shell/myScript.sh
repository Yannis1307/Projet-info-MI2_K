#!/bin/bash

start_time=$(date +%s%N)
executable="./wildwater"
csv="$1"
cmd="$2"

#Compilation automatique
if [ ! -x "$executable" ]; then
    echo "Compilation en cours..."
    make
    if [ ! -x "$executable" ]; then
        echo "Erreur : Compilation échouée."
        exit 1
    fi
fi

if [ $# -lt 2 ]; then
    echo "Usage: $0 <fichier_csv> <commande> [arguments]"
    exit 1
fi

if [ ! -f "$csv" ]; then
    echo "Erreur : Fichier '$csv' introuvable."
    exit 1
fi

# HISTO 
if [ "$cmd" = "histo" ]; then
    if [ $# -ne 3 ]; then
        echo "Erreur : histo attend l'argument mode [max|src|real]."
        exit 1
    fi
    mode="$3"
    
    echo "____MODE HISTOGRAMME : $mode ---"

    awk -F";" '$1=="-" {print $0}' "$csv" | $executable "histo" "$mode" - > "histo_$mode.dat"
    
    if [ $? -ne 0 ]; then
        echo "Erreur de l'exécution Histo."
        exit 1
    fi

    # Génération des graphiques
    if [ -f "shell/histo.gnu" ] && [ -s "histo_$mode.dat" ]; then
        echo "Génération des graphiques..."
        
    
        head -n 1 "histo_$mode.dat" > header.tmp
        
        # 50 plus petites
        cat header.tmp > "histo_${mode}_min50.dat"
        tail -n +2 "histo_$mode.dat" | sort -t";" -k2,2n | head -n 50 >> "histo_${mode}_min50.dat"
        
        # 10 plus grandes
        cat header.tmp > "histo_${mode}_max10.dat"
        tail -n +2 "histo_$mode.dat" | sort -t";" -k2,2nr | head -n 10 >> "histo_${mode}_max10.dat"

        # appel Gnuplot pour les 2 graphiques
        gnuplot -e "inputname='histo_${mode}_min50.dat'; outputname='histo_${mode}_min50.png'; my_title='Histogramme : 50 plus petites ($mode)'" shell/histo.gnu
        gnuplot -e "inputname='histo_${mode}_max10.dat'; outputname='histo_${mode}_max10.png'; my_title='Histogramme : 10 plus grandes ($mode)'" shell/histo.gnu
        
        rm -f header.tmp "histo_${mode}_min50.dat" "histo_${mode}_max10.dat"
        echo "Graphiques générés."
    fi
    echo "Traitement de l'histogramme terminé."

# LEAKS
elif [ "$cmd" = "leaks" ]; then
    if [ $# -ne 3 ]; then
        echo "Erreur : leaks attend l'argument ID usine."
        exit 1
    fi
    
    facility="$3"
    output_file="leaks.dat"

    echo "--- MODE FUITES : $facility ---"
    
  
    
    awk -F";" -v id="$facility" '$1==id || $2==id || $3==id {print $0}' "$csv" | $executable "leaks" "$facility" - > "leaks_output.tmp"

    if [ $? -ne 0 ]; then
        echo "Erreur lors de l'exécution du programme C."
        rm -f "leaks_output.tmp"
        exit 1
    fi

    # Lecture et Historique
    result=$(cat "leaks_output.tmp")
    rm -f "leaks_output.tmp"

    if [ "$result" = "-1" ]; then
        echo "Attention : Usine non trouvée ou données invalides."
        echo "$facility;-1" >> "$output_file"
    else
        echo "Volume de perte calculé : $result M.m3"
        echo "$facility;$result" >> "$output_file"
        echo "Résultat ajouté à l'historique : $output_file"
    fi

    echo "Traitement Leaks terminé."

else
    echo "Commande inconnue : $cmd"
    exit 1
fi

# Chrono
end_time=$(date +%s%N)
duration=$(( (end_time - start_time) / 1000000 ))
echo "Durée totale : $duration ms"
