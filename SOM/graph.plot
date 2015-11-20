#!/usr/bin/gnuplot

set term png size 800,600

set output "distance.png"
set yrange [0:1]
plot for [i=2:17] "distance.log" u 1:i w l t sprintf("unit: %d", i-1)

set output "position.png"
set yrange [-1:101]
plot for [i=2:17] "position.log" u 1:i w l t sprintf("unit: %d", i-1)
