set xlabel "Generation" font "Helvetica, 12"
set ylabel "Fitness" font "Helvetica, 12"
set title "NQueens" font "Helvetica, 15"
set xrange [0:2500]
set yrange [0:1.00]
set grid
set xtics 250
set ytics 0.05
set key right bottom
set key spacing 1.25
plot "../../saidas/grafico_convergencia" using 1:2 title "Best" with lines lc "green" lw 2,\
"../../saidas/grafico_convergencia" using 1:3 title "Average" with lines lc "blue" lw 2,\
"../../saidas/grafico_convergencia" using 1:4 title "Worst" with lines lc "red" lw 2
pause 0.5
reread