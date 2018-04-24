import numpy as np
import matplotlib.pyplot as plt

# basic ising class 
class IsingSimulator:
    def __init__(self, l=20, InvTemp=1.0, j_inter=1.0, b_ext=0.0):
        self.latticeSize = l
        self.lattice = np.zeros((l,l),dtype=np.int)
        self.B_ext = b_ext
        self.J_interact = j_inter
        self.invTemp = InvTemp
        
    def randomConformation(self):
        # create random array with -1 or 1
        self.lattice = ( ( np.random.randint(0,2,(self.latticeSize,self.latticeSize)) * 2 ) - 1)
        
    def calcMagnetisation(self):
        return float(self.lattice.sum()) / float(self.lattice.size )
        
    def singleSpinenergy(self, x, y):
        # here we go for periodic boundary conditions:
        left  = x - 1 if x > 0 else ( self.latticeSize - 1 )
        right = x + 1 if x < ( self.latticeSize - 1 ) else 0
        down  = y + 1 if y < ( self.latticeSize - 1 ) else 0
        up    = y - 1 if y > 0 else ( self.latticeSize - 1 )
        
        # also possible, but slower due to %-operator
        #nb = 0.5 * self.J_interact * ( 
        #    self.lattice[(x+1)%self.latticeSize,y] + 
        #    self.lattice[x,(y+1)%self.latticeSize] + 
        #    self.lattice[(x-1)%self.latticeSize,y] + 
        #    self.lattice[x,(y-1)%self.latticeSize] )
        
        # E = - B_ext * S_i - 1/2 * J * S_i * sum_Sj_next_neighbors
        return -1.0 * self.lattice[x,y] * ( self.B_ext + 0.5 * self.J_interact * (
            self.lattice[x,up] + self.lattice[x,down] + self.lattice[left,y] + self.lattice[right,y] ) )
    
    def calcEnergy(self):
        E = 0.0
        for x in range(self.latticeSize):
            for y in range(self.latticeSize):
                E += self.singleSpinenergy(x,y)
        return -E/self.lattice.size
    
    def performMCS(self, time):
        # check if lattice was initialised
        if self.lattice[0,0] == 0:
            self.randomConformation()
            
        #counter for accepted moves
        counter = 0
        
        # do the loop
        for t in range(time):
            # get random x,y coordinates @ students ???? what could be done to optimize????
            randX = np.random.randint(0,self.latticeSize,(self.latticeSize))
            randY = np.random.randint(0,self.latticeSize,(self.latticeSize))
            for x, y in zip(randX, randY):
                e_old = self.singleSpinenergy(x, y)
                # do the "test" flip
                e_new = -e_old
                # apply metropolis
                prob =np.exp( - (e_new - e_old) * self.invTemp)
                #print(e_old, e_new, prob)

                if np.random.random_sample() < prob:
                    self.lattice[x,y] *= -1
                    counter += 1
                    
        return counter
                
                
    def plotConfig(self):
        plt.figure(dpi=90)
        plt.pcolormesh(self.lattice)
        plt.xlabel('X')
        plt.ylabel('Y')
        plt.title('spatial magnetisation')
        plt.show()
        
    def plotEquilibrationRun(self):
        self.randomConformation()
        nMCS = 20
        time = []
        magnetisation = []
        for i in range(200):
            self.performMCS(nMCS)
            time.append(nMCS*i)
            magnetisation.append(self.calcMagnetisation())
        
        plt.figure(dpi=120)
        plt.plot(time, magnetisation, 'g+')


# function to perform simple sampling simulation run        
def runIsingSimulationSimple(l=20, InvTemp=1.0, j_inter=1.0, b_ext=0.0, nSteps=1000):
    ising = IsingSimulator(l, InvTemp, j_inter, b_ext)
    energy = []
    energy2 = []
    magnet = []
    bolzma = []
    # run simulations
    for t in range(nSteps):
        ising.randomConformation()
        e = ising.calcEnergy()
        bolz = np.exp(-e*InvTemp)
        bolzma.append(bolz)
        energy.append(e * bolz)
        energy2.append(e * e * bolz)
        magnet.append(ising.calcMagnetisation() * bolz)
        
    # calculate averages
    aveEner = sum(energy) / (sum(bolzma) * nSteps)
    aveEner2 = sum(energy2) / (sum(bolzma) * nSteps)
    aveMagn = sum(magnet) / (sum(bolzma) * nSteps)
    heatcap = InvTemp * InvTemp * ( aveEner2 - aveEner*aveEner )
    
    return aveMagn, aveEner, heatcap


#function to perform importance sampling simulations
def runImportanceSampling(l=20, InvTemp=1.0, j_inter=1.0, b_ext=0.0, nSteps=100):
    ising = IsingSimulator(l, InvTemp, j_inter, b_ext)
    energy  = []
    energy2 = []
    magnet  = []
    
    # do the equilibration
    ising.performMCS(2000)
    # run simulations
    for t in range(nSteps):
        ising.performMCS(200)
        e = ising.calcEnergy()
        energy.append(e)
        energy2.append(e*e)
        magnet.append(ising.calcMagnetisation())
        
    # calculate averages
    aveEner = sum(energy) / nSteps
    aveMagn = sum(magnet) / nSteps
    heatcap = InvTemp * InvTemp * ( sum(energy2)/nSteps - aveEner*aveEner )
    
    return aveMagn, aveEner, heatcap