#!/usr/bin/gnuplot

set term png size 800,600

set output "/dev/null"
plot for [i=2:17] "distance.log" u 1:i w l t sprintf("unit: %d", i-1)
set output "distance.png"
replot "< awk '{ sum=0; for(i=2; i<18; i++){ sum += $i }; print $1, sum/16 }' distance.log" w l lw 3 t "average"


set term png size 800,1024

set output "position.png"
set yrange [-1:11]
set key below
set xlabel "time"
set multiplot layout 2,1
set ylabel "i"
plot for [i=2:33:2] "position.log" u 1:i w l t ""
set ylabel "j"
plot for [i=3:34:2] "position.log" u 1:i w l t sprintf("unit: %d", i/2)
