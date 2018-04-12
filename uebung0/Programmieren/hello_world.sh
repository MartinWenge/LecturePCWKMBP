#!/bin/bash

#shell Skript Ausgabe
echo "Hello World!"

#python
if [ -s "hello_world.py" ]; then
  python hello_world.py
fi

#c++ Code
if [ -s "hello_world.cpp" ]; then
  if [ ! -s "HelloWorld" ]; then
    g++ hello_world.cpp -o HelloWorld
    ./HelloWorld
  else
    ./HelloWorld
  fi
fi
