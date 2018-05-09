import numpy as np
import bfmSimulator as bfm

# some examples on how to use the bfm module for a pore
BoxX, BoxY, BoxZ = 128 , 16, 16
PeriodicX, PeriodicY, PeriodicZ = True, False, False
myBfm = bfm.BFMSimulator(BoxX, BoxY, BoxZ, PeriodicX, PeriodicY, PeriodicZ)

# create the chain
myBfm.addLinearChain( 64, np.array((0,BoxY/2,BoxZ/2)) )

# run the short simulation (True = with, False = without) excluded volume 
for i in range(2):
    myBfm.performMcs(True)

# print the config and do all the evaluations (Rg, Ree, g(r))
myBfm.plotConfig()
