#!/bin/bash

echo "Install Lemonade one click"

#show actual working directory
pwd

#create Lemonade directory
if [ ! -d "LeMonADE" ]; then
  mkdir LeMonADE
fi

if [ -d "LeMonADE" ]; then
  cd LeMonADE
  
  #create install folder
  if [ ! -d "lemonade_install" ]; then
    mkdir lemonade_install
  fi
  
  if [ ! -d "LeMonADE" ]; then
    git clone https://github.com/LeMonADE-project/LeMonADE.git
  fi
  
  if [ -d "LeMonADE" ]; then
    cd LeMonADE
    #run configure script to set installpath to folder we created
    ./configure -DINSTALLDIR_LEMONADE=../../lemonade_install
    make docs
    make install
    
    if [ -d "build/docs/html" ]; then
      cd build/docs/html
      if [ -s "index.html" ]; then
	firefox index.html &
      fi
      cd ../../..
    fi
    
    cd ../
  fi
  
  #go back to original directory
  cd ../
fi

echo "LeMonADE was installed successfully"
