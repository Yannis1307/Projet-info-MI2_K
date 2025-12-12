# Configuration de la sortie (Image PNG)
set terminal pngcairo size 800,600 enhanced font 'Verdana,10'
set output outputname

# Style de l'histogramme
set style data histograms
set style fill solid 1.0 border -1
set boxwidth 0.9
set grid ytics

# Titres et Axes
set title "Analyse WildWater"
set xlabel "Usines"
set ylabel "Volume (m3)"

# Rotation des noms pour lisibilité
set xtics rotate by -45
set datafile separator " "

# Dessin (utilise la colonne 2 pour la hauteur, colonne 1 pour le nom)
plot inputname using 2:xtic(1) title "Données" lc rgb "#0060ad"