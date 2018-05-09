import numpy as np
import bfmSimulator as bfm

# some examples on how to use the bfm module
BoxX, BoxY, BoxZ = 32 , 32, 32
PeriodicX, PeriodicY, PeriodicZ = False, True, True
myBfm = bfm.BFMSimulator(BoxX, BoxY, BoxZ, PeriodicX, PeriodicY, PeriodicZ)

# create a brush
for y in range(0,4):
    for z in range(0,4):
        # adding a sequence of (16) monomers with the bond (2,0,0) starting at the defined position
        myBfm.addLinearChain(16, np.array( (0, y*8, z*8) ))
        # fix the first monomer to make a brush
        myBfm.molecules[myBfm.numMonos-16].isFixed = True

# check the grafting points
for mono in myBfm.molecules:
    if mono.pos[0] == 0:
        if not mono.isFixed:
            print(mono.pos)

# run a short simulation (True = with, False = without) excluded volume 
for i in range(500):
    myBfm.performMcs(True)

# print the config in a scatterplot to get an impression of the system
# find the output in "configScatterplot.png"
myBfm.plotConfig()
