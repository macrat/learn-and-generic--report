#!/usr/bin/gnuplot

set term png size 800,600

set xlabel "noise level [%]"
set ylabel "success rate [%]"

set xtic 10
set ytic 10

set grid

set output "error.png"
plot 100-100/(1+exp(-(x-49.97798491)/4.10077344)) w l, "error.txt" w p t "real data"
