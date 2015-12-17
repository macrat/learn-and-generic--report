#!/usr/bin/gnuplot

set term png size 800,600

set xlabel "noise level [%]"
set ylabel "success rate [%]"

set xtic 10
set ytic 10

set output "error.png"
plot "error.txt" w lp t ""
