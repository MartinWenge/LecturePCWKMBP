import copy
import numpy as np

class linearCongruential:
    def __init__(self, A=1664525, B=1013904223, M=2<<32, X_0=42):
        self.a = A
        self.b = B
        self.m = M
        self.x_0 = X_0
        
    def getRandomInt(self):
        self.x_0 = ( self.a * self.x_0 + self.b ) % self.m
        return self.x_0
    
    def getRandomFloat(self):
        return self.getRandomInt() / self.m
    
    def getBitRepresentation(self):
        return bin(self.getRandomInt())
    
    def getPeriod(self):
        # make sure we are in the circle
        self.getRandomInt()
        # make a real copy
        x_0_reference = copy.copy(self.x_0)
        # change the numer
        self.getRandomInt()
        counter = 1
        # do the loop
        while (x_0_reference != self.getRandomInt()):
            counter += 1
            if counter > 2<<22:
                print("Interrupt: period is larger than ",2<<22)
                return counter
        return counter
    
    def createListOfPairs(self, numPairs=2000):
        listOfPairs = []
        for i in range(numPairs):
            pair = [self.getRandomFloat(), self.getRandomFloat()]
            listOfPairs.append(pair)
        return listOfPairs
    
    def createListOfTriples(self, numTriples=2000):
        listOfTriples = []
        for i in range(numTriples):
            pair = [self.getRandomFloat(), self.getRandomFloat(), self.getRandomFloat()]
            listOfTriples.append(pair)
        return listOfTriples
    
    def writeSeriesToFile(self, filename ="myRandomSeries.dat", numOfNumbers=256):
        listOfNumbers = []
        for i in range(numOfNumbers):
            listOfNumbers.append(self.getRandomInt())
        np.savetxt(filename, listOfNumbers)

# end module linearCongruential
