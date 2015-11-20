#!/usr/bin/gnuplot

set term png size 800,600

set output "result.png"
plot "result.log" w l t "average", "" u 1:3 w l t "best"
