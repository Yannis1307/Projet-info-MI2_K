# Fichier : shell/histo.gnu

# 1. Configuration de la sortie
set terminal pngcairo size 1200,800 enhanced font 'Verdana,10'
set output outputname

# 2. Configuration des données
set datafile separator ";"

# 3. Configuration du graphique Histogramme
set style data histograms
set style fill solid 1.0 border -1
set boxwidth 0.7 relative

# 4. Grille et Axes
set grid ytics linestyle 1 lc rgb "#bbbbbb" lw 1
set ylabel "Volume (M.m^3.an^-1)" font ",12"
set xlabel "Identifiant Usine" font ",12"

# Rotation des étiquettes (IDs) sur l'axe X
set xtics rotate by -45 scale 0 font ",8"

# On ajuste les marges
set bmargin 10

# 5. Titre
set title "Histogramme des Volumes par Usine" font ",14"

# 6. Commande de traçage
# 'every ::1' : IGNORE la première ligne (l'en-tête)
plot inputname every ::1 using 2:xtic(1) notitle linecolor rgb "#0060ad"