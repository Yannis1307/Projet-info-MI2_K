#!/bin/bash
#Starts the timer and ensures the C executable is present (compiles if needed).
start_time=$(date +%s%N)
executable="./wildwater"
csv="$1"
cmd="$2"
if [ ! -x "$executable" ]; then
    echo "Compiling..."
    make
    if [ ! -x "$executable" ]; then
        echo "Error: Compilation failed."
        exit 1
    fi
fi
#Checks if enough arguments are provided and if the input file exists.
if [ $# -lt 2 ]; then
    echo "Usage: $0 <csv_file> <command> [arguments]"
    exit 1
fi

if [ ! -f "$csv" ]; then
    echo "Error: File '$csv' not found."
    exit 1
fi

#Filters data, runs the C program, sorts output, and generates Gnuplot charts.
if [ "$cmd" = "histo" ]; then
    if [ $# -ne 3 ]; then
        echo "Error: histo expects mode argument [max|src|real]."
        exit 1
    fi
    mode="$3"
    
    echo "--- HISTOGRAM MODE: $mode ---"
    
   #Filter data and execute C program
    awk -F";" '$1=="-" {print $0}' "$csv" | $executable "histo" "$mode" - > "histo_$mode.dat"
    
    if [ $? -ne 0 ]; then
        echo "Error during Histo execution."
        exit 1
    fi

    #sorts results and generathe the graphs using Gnuplot
    if [ -f "shell/histo.gnu" ] && [ -s "histo_$mode.dat" ]; then
        echo "Generating charts..."
        
        head -n 1 "histo_$mode.dat" > header.tmp
        
        #50 smallest
        cat header.tmp > "histo_${mode}_min50.dat"
        tail -n +2 "histo_$mode.dat" | sort -t";" -k2,2n | head -n 50 >> "histo_${mode}_min50.dat"
        
        #10 largest
        cat header.tmp > "histo_${mode}_max10.dat"
        tail -n +2 "histo_$mode.dat" | sort -t";" -k2,2nr | head -n 10 >> "histo_${mode}_max10.dat"

        gnuplot -e "inputname='histo_${mode}_min50.dat'; outputname='histo_${mode}_min50.png'; my_title='Histogram: 50 smallest factories ($mode)'" shell/histo.gnu
        gnuplot -e "inputname='histo_${mode}_max10.dat'; outputname='histo_${mode}_max10.png'; my_title='Histogram: 10 largest factories ($mode)'" shell/histo.gnu
        
        rm -f header.tmp "histo_${mode}_min50.dat" "histo_${mode}_max10.dat"
        echo "Charts generated."
    fi
    echo "Histogram processing finished."

#Filters data for a specific factory, calculates leaks via C, and updates history.
elif [ "$cmd" = "leaks" ]; then
    if [ $# -ne 3 ]; then
        echo "Error: leaks expects factory ID argument."
        exit 1
    fi
    
    facility="$3"
    output_file="leaks.dat"

    echo "--- LEAKS MODE: $facility ---"
    
    #Filter specific station data and run calculation 
    awk -F";" -v id="$facility" '$1==id || $2==id || $3==id {print $0}' "$csv" | $executable "leaks" "$facility" - > "leaks_output.tmp"

    if [ $? -ne 0 ]; then
        echo "Error during C program execution."
        rm -f "leaks_output.tmp"
        exit 1
    fi

    #read result and update history file
    result=$(cat "leaks_output.tmp")
    rm -f "leaks_output.tmp"

    if [ "$result" = "-1" ]; then
        echo "Warning: Plant not found or invalid data."
        echo "$facility;-1" >> "$output_file"
    else
        echo "Calculated leak volume: $result M.m3"
        echo "$facility;$result" >> "$output_file"
        echo "Result added to history: $output_file"
    fi

    echo "Leaks processing finished."

else
    echo "Unknown command: $cmd"
    exit 1
fi

#Calculate and print the total execution time.
end_time=$(date +%s%N)
duration=$(( (end_time - start_time) / 1000000 ))
echo "Total duration: $duration ms"
