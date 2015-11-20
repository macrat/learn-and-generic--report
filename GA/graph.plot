#!/usr/bin/gnuplot

set term png size 800,600

set yrange [0:*]

set output "result.png"
plot "result.log" w l t "average", "" u 1:3 w l t "best"

set output "advance.png"
plot "advance.log" u 0:2:3 w filledcurves t "" lc rgb "gray", "" u 0:1 w l t "average" lc rgb "red"
