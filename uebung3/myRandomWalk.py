import numpy as np
import matplotlib.pyplot as plt

# class for random walk in 3 dimensions, arbitrary dimensions should be simple?
class randomWalk:
    def __init__(self, Steps):
        steps=Steps

    def createPath(self):
        pass

    def getEndPosition(self):
        pass

    def calculateREndToEnd(self):
        pass

    def calculateRadiusOfGyration(self):
        pass

def runRandomWalkSimulationSimple(nSteps, nIteration):
    walker = randomWalk(nSteps)

    radialEndPosition = []
    radiusOfGyration = []
    endPositons = [] # list of vector of endPositons
    for i in range(nIteration):
        walker.createPath()
        radialEndPosition.append( walker.calculateREndToEnd() )
        radiusOfGyration.append( walker.calculateRadiusOfGyration() )
        endPositons.append( walker.getEndPosition() )

    