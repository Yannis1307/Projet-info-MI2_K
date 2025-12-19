# File: shell/histo.gnu

#Output configuration
set terminal png size 1200,800
set output outputname

#Data configuration
set datafile separator ";"

#Histogram chart configuration
set style data histograms
set style fill solid 1.0 border -1
set boxwidth 0.7 relative

#Grid and Axes
set grid ytics linestyle 1 lc rgb "#bbbbbb" lw 1
set ylabel "Volume (M.m³/year)" font ",12"
set xlabel "Factory Identifier" font ",12"

#Rotate labels (IDs) on X axis
set xtics rotate by -45 scale 0 font ",8"

#Adjust margins
set bmargin 10

#title
if (!exists("my_title")) my_title='Volume Histogram'
set title my_title

#plotting command
plot inputname every ::1 using 2:xtic(1) notitle linecolor rgb "#0060ad"
