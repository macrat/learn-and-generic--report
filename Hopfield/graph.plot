#!/usr/bin/gnuplot

set term png size 800,600

set xlabel "noise level [%]"
set ylabel "success rate [%]"

set xtic 10
set ytic 10

set output "error.png"
plot 100-100/(1+exp(-(x-50)/4)), "error.txt"
