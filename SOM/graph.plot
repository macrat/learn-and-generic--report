#!/usr/bin/gnuplot

set term png size 800,600

set output "/dev/null"
plot for [i=2:17] "distance.log" u 1:i w l t sprintf("unit: %d", i-1)
set output "distance.png"
replot "< awk '{ sum=0; for(i=2; i<18; i++){ sum += $i }; print $1, sum/16 }' distance.log" w l lw 3 t "average"

set output "position.png"
set yrange [-1:101]
plot for [i=2:17] "position.log" u 1:i w l t sprintf("unit: %d", i-1)
