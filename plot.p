set xlabel "Iteration" font "Helvetica, 12"
set ylabel "Fitness" font "Helvetica, 12"
set title "Genetic Algorithm" font "Helvetica, 15"
set xrange [0:100]
set yrange [0:1]
set grid
set xtics 1
plot "tempos" using 1:2 title "Best" with lines lc "green" lw 2,\
"tempos" using 1:3 title "Worst" with lines lc "red" lw 2,\
"tempos" using 1:4 title "Average" with lines lc "blue" lw 2
