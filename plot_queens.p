set xlabel "Generation" font "Helvetica, 12"
set ylabel "Fitness" font "Helvetica, 12"
set title "Genetic Algorithm\nNQueens 512 x 512" font "Helvetica, 15"
set xrange [0:2000]
set yrange [0:1.00]
set grid
set xtics 200
set ytics 0.05
set key right bottom
set key spacing 1.25
plot "tempos" using 1:2 title "Best" with lines lc "green" lw 2,\
"tempos" using 1:4 title "Average" with lines lc "blue" lw 2,\
"tempos" using 1:3 title "Worst" with lines lc "red" lw 2
pause 0.5
reread