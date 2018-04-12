# Gnuplot Terminal Optionen setzen
set terminal png
set output 'linear_function.png'

# Fit ausführen
a=1
b=1
f(x)=a * x + b
fit f(x) "test_linear_function.dat" using 1:2 via a, b
fit_title=sprintf("linear fit: a=%.1f, b=%.1f", a, b)

# Daten mit fit plotten
plot "test_linear_function.dat" u 1:2 w lp ,\
     f(x) title fit_title

#
