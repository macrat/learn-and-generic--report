#!/usr/bin/gnuplot

set term png size 800,600
set output "error.png"

set ylabel "error level"
set xlabel "learn step"
plot "learning.log" w l t ""
