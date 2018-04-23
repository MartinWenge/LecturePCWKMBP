import matplotlib.pyplot as plt
import numpy as np
import myIsing

# prepare data containers
temp = []
magnetisierung = []
energie = []
waermekapazitaet = []

# run simulation for different temperatures
for T in np.arange(0.1, 3, 0.02):
    m, e, c = myIsing.runIsingSimulationSimple(20, 1/T, 1.0, 0.0, 1000)
    temp.append(T)
    magnetisierung.append(np.fabs(m))
    energie.append(e)
    waermekapazitaet.append(c)
    
fig, (ax1, ax2, ax3) = plt.subplots(3, sharex=True)
beta_0=0.86
T_C=1/beta_0
ax1.plot(temp,magnetisierung,'g+')
ax1.set( ylabel='M' )
ax2.plot(temp,energie,'ro')
ax2.set( ylabel='E')
ax3.plot(temp,waermekapazitaet,'b.')
ax3.set( ylabel='C', xlabel='T', )
ax3.set_xlim(0,4)

fig.savefig("SimpleSampling.png")
