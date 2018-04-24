#!/bin/bash

echo "Simulations with Ising Model"

pwd

#prüfe ob .cpp datei da ist
if [ -s "ising_simulator.h" ] && [ -s "ising_simulator.cpp" ]; then
  #prüfe, ob cmake file da ist
  if [ -s "CMakeLists.txt" ]; then
    #prüfe, ob build Ordner schon da ist
    if [ ! -d "build" ]; then
      mkdir build
    fi
    cd build/
    
    #führe cmake aus, erzeuge die ausführbare Datei und führe sie aus
    if [ -s "Ising" ]; then rm Ising; fi
    
    cmake ../
    make
    
    #./Ising, use: -time, -size, -field, -temperature_/start/end/intervall <- INVERSE TEMPERATURE!!!!
    ./Ising -time 200 -temperature_start 0.02 -temperature_end 1.5 -temperature_intervall 0.02
    
    cd ../
  fi
fi

if [ ! -d "eva" ]; then
  mkdir eva
fi
cd eva/
mv ../build/*.dat .

GNUPLOT_FILE="plot_ising.gpi"
if [ -s "../$GNUPLOT_FILE" ]; then
  cp ../$GNUPLOT_FILE .
  gnuplot "$GNUPLOT_FILE"
  eog equilibration.png &
fi

echo "Beispiel Skript Übung2 Ising Modell beendet"
