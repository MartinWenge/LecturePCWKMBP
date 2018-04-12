#!/bin/bash

# kommentieren mit vorangestellter Raute

#### Aufgabe 1 ####
#Anzeige des aktuellen Pfades
pwd

#Ordner im aktuellen Verzeichnis erstellen
mkdir test_ordner

#in ein Verzeichnis wechseln
cd test_ordner

#Testordner zum Löschen erstellen
mkdir test_remove

#Inhalt des aktuellen Verzeichnisses anzeigen lassen 
#  -l Liste; -a alles, auch versteckte Objekte anzeigen; -h human readable, Dateigrößen in kB, MB, GB usw. statt in Byte anzeigen
ls -lah

# Verzeichnis löschen
# -v verbose, Ausgabe der einzelnen Prozessschritte; -r recursive, löscht auch Ordner und Unterordner
#   !!!!!!!!  es wird ohne Rückfrage alles gelöscht, wofür man Schreibberechtigung hat  !!!!!!!!!!!!
rm -vr test_remove

#### Aufgabe 3 ####
# shellvariable definieren
TEXTFILE="test_textfile.bfm"
DATAFILE="test_datafile.dat"
GNUPLOTFILE="test_gnuplot.qpi"

#Überprüfen, ob eine Datei NICHT (!) existiert und Handlungsauswahl mit if/else
if [ ! -s "$TEXTFILE" ]; then
  echo "kein Textfile gefunden, suche Archiv"
  #suche nach Archiv im übergeordneten Verzeichnis und extrahiere hierher falls es existiert
  if [ -s "../test_files.zip" ];then
    echo "Archiv gefunden, entpacke"
    unzip ../test_files.zip
  fi
fi

# Textdatei behandeln
# Überprüfen, ob eine Datei existiert, if/else
if [ -s "$TEXTFILE" ]; then
  #Ausgabe auf Konsolenbildschirm
  echo "Textfile gefunden, führe Beispielanwendungen aus"
  
  #erste Zeilen einer Datei anzeigen
  head $TEXTFILE
  
  #letzte Zeilen einer Datei anzeigen
  tail $TEXTFILE
  
  #Ausgaben in neue Datei Streamen, nur 5 Zeilen
  head -n 5 $TEXTFILE > newfile.txt
  
  #Ausgabe an bestehende Datai anfügen, nur 5 Zeilen
  tail -n 5 $TEXTFILE >> newfile.txt
  
  #ganze Datei anzeigen
  cat newfile.txt
  
  #Datei nach bestimmten Mustern durchsuchen, für Sonderzeichen \ voranstellen
  grep "\!mcs=" $TEXTFILE
fi

#Datendatei behandeln
if [ -s "$DATAFILE" ]; then
  echo "Datenfile gefunden, führe Beispielanwendungen aus"
  
  head $DATAFILE
  
  #Mittelwerte bilden aus zweiter, dritter, vierter und fünfter Spalte
  awk 'NR > 2 {s+=$2; n+=1 } END {printf "%.3f\n", s/n}' $DATAFILE > average_1.dat
  awk 'NR > 2 {s+=$3; n+=1 } END {printf "%.3f\n", s/n}' $DATAFILE >> average_1.dat
  awk 'NR > 2 {s+=$4; n+=1 } END {printf "%.3f\n", s/n}' $DATAFILE >> average_1.dat
  awk 'NR > 2 {s+=$5; n+=1 } END {printf "%.3f\n", s/n}' $DATAFILE >> average_1.dat
  
  #Mittelwerte bilden und Wurzeln ziehen aus zweiter, dritter vierter und fünfter Spalte
  awk 'NR > 2 {s+=sqrt($2); n+=1 } END {printf "%.3f\n", s/n}' $DATAFILE > average_2.dat
  awk 'NR > 2 {s+=sqrt($3); n+=1 } END {printf "%.3f\n", s/n}' $DATAFILE >> average_2.dat
  awk 'NR > 2 {s+=sqrt($4); n+=1 } END {printf "%.3f\n", s/n}' $DATAFILE >> average_2.dat
  awk 'NR > 2 {s+=sqrt($5); n+=1 } END {printf "%.3f\n", s/n}' $DATAFILE >> average_2.dat
  
  #zwei Spaltendateien nebeneinander zusammenfügen
  paste average_1.dat average_2.dat > average.dat
  
  #mehrere Dateien mit Dateinahme beginnend mit "average_" löschen
  rm -v average_*
  
  cat average.dat
  
  #erstelle gnuplot skript
  if [ ! -s "$GNUPLOTFILE" ]; then
    echo "set terminal png" > $GNUPLOTFILE
    echo "set output 'test_graph.png'" >> $GNUPLOTFILE
    echo "plot \"average.dat\" using 0:2 with linespoints" >> $GNUPLOTFILE
  fi
  
  #gnuplot skript ausführen
  gnuplot $GNUPLOTFILE
  #png anzeigen mit: gwenview test_graph.png &
fi

#png ins übergeordnete Verzeichnis kopieren
cp test_graph.png ../

#ins übergeordnete Stammverzeichnis wechseln
cd ../

#Ordnerinhalt anzeigen
ls test_ordner/

#aufräumen
#rm -vr test_ordner/
#rm test_graph

#abschließende Ausgabe
echo "Beispiel Skript Übung0 konsole.sh beendet"
