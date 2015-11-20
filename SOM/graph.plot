#!/usr/bin/gnuplot

set term png size 800,600
set output "training.png"

set yrange [0:1]
plot for [i=2:17] "training.log" u 1:i w l t sprintf("unit: %d", i-1)
