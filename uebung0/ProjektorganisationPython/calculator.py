# do the imports in the module file
import numpy as np
import matplotlib.pyplot as plt
import os

# class object in calculator, named calculator ;o) don't get confused when using this module!
class calculator:
    def __init__ (self, A=1.0, B=1.0):
        self.a = A
        self.b = B
        
    def add(self):
        return self.a + self.b
    
    def substract(self):
        return self.a - self.b
    
    def multiply(self):
        return self.a * self.b
    
    def writeLinear(self):
        x = np.linspace(0,20,21)
        y = self.a* x + self.b
        dataContainer = np.column_stack((x,y))
        np.savetxt("mylinFun_"+str(self.a)+"_"+str(self.b)+".dat", dataContainer ,delimiter = '\t', header = "x\ty = "+str(self.a)+" * x + "+str(self.b))
           
    def plotLinear(self):
        x = np.linspace(0,20,21)
        y = self.a*x + self.b
        plt.figure()
        plt.plot(x,y,'bo')
        plt.show()
        

# function to plot some stuff
def plotTwoColumnData(filepath, outfilename="myPlot.png"):
    # check if file exists
    if os.path.isfile(filepath):
        dataFile = np.loadtxt(filepath,comments='#', dtype={'names' : ('x', 'y'),'formats' : (np.float, np.float)}, delimiter='\t')
        plt.figure()
        plt.plot(dataFile['x'], dataFile['y'], 'rx')
        print("plot figure to file "+outfilename)
        plt.savefig(outfilename, bbox_inches='tight')

#
