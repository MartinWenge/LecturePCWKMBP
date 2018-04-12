#!/bin/bash

pwd

#prüfe ob .cpp datei da ist
if [ -s "main_programming.cpp" ] && [ -s "calculator.h" ] && [ -s "calculator.cpp" ]; then
  #prüfe, ob cmake file da ist
  if [ -s "CMakeLists.txt" ]; then
    #prüfe, ob build Ordner schon da ist
    if [ ! -d "build" ]; then
      mkdir build
    fi
    cd build/
    
    #führe cmake aus, erzeuge die ausführbare Datei und führe sie aus
    if [ ! -s "Calculator" ]; then
      cmake ../
      make
    fi
    ./Calculator
    
    cd ../
  fi
fi

if [ ! -d "eva" ]; then
  mkdir eva
fi
cd eva/

if [ -s "../plot_linear_function.plt" ] && [ -s "../build/test_linear_function.dat" ]; then
  cp ../plot_linear_function.plt .
  cp ../build/test_linear_function.dat .
  gnuplot 'plot_linear_function.plt'
  gwenview linear_function.png &
fi

echo "Beispiel Skript Übung0 Projektorganisation beendet"
