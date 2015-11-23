#!/usr/bin/gnuplot

set term png size 800,600

set yrange [0:*]

set output "result.png"
plot "result.log" w l t "average", "" u 1:3 w l t "best"

set output "advance.png"
set key bottom right
set yrange [0:100]
set ylabel "fitneses[%]"
set xlabel "generation"
plot "< awk '{ print $1*100, $2*100, $3*100, $4*100 }' advance.log" u 0:2:3 w filledcurves t "" lc rgb "#eeeeee" \
,	"" u 0:3:4 w filledcurves t "" lc rgb "#e0e0e0" \
,	"" u 0:2 w l t "best" lc rgb "green" \
,	"" u 0:3 w l t "worst" lc rgb "blue" \
,	"" u 0:4 w l t "median" lc rgb "orange" \
,	"" u 0:1 w l t "average" lc rgb "red"
