import numpy as np
import matplotlib.pyplot as plt

# die Klasse randomWalk enthält die Anzahl Schritte des Walks, "self.steps"
#  und den absoluten Pfad als list object "self.walkedPath"
# die createPath Funktion erstellt einen neuen, zufälligen walk
#
# zu bearbeiten: 
#    calculateREndToEnd
#    calculateRadiusOfGyration
#
#  Die Modulfuntion runRandomWalkSimulationSimple(steps, iterations) 
#  berechnet Mittelwerte und Verteilungsfunktionen und speichert diese in Dateien und ein Übersichtsbild
# 
#   !!!!!!!!!!  Bevor ihr irgendetwas ändert: einfach mal die Datei ausführen, 
#               schauen was so passiert, dann SCHRITTWEISE Änderungen durchführen  !!!!!!!!!!!!!

# zu kompliziert? Baut euch euren eigenen Randomwalk, so wie ihr es implementieren würdet :o)


# class for random walk in 2 dimensions, arbitrary dimensions should be simple?
class randomWalk:
    def __init__(self, Steps):
        self.steps = Steps
        self.walkedPath = [(0,0)]

    def createPath(self):
        if len(self.walkedPath) > 1:
            self.walkedPath = [(0,0)]
        for _ in range(self.steps):
            newX = self.walkedPath[-1][0] + ( ( np.random.randint(0,3) ) - 1)
            newY = self.walkedPath[-1][1] + ( ( np.random.randint(0,3) ) - 1)
            self.walkedPath.append((newX,newY))

    def getEndPosition(self):
        return self.walkedPath[-1]

    def calculateREndToEnd(self):
        # ree = (x_end-x_0)^2 + (y_end-y0)^2
        return 0.0

    def calculateRadiusOfGyration(self):
        # rg =  sum_i (r_com-ri)^2, r_com = sum_i (r_i)
        # https://en.wikipedia.org/wiki/Radius_of_gyration#Molecular_applications
        return 0.0
    
    def plotPath(self):
        x = []
        y = []
        for pos in self.walkedPath:
            x.append(pos[0])
            y.append(pos[1])
        plt.figure()
        plt.plot(x,y, 'b-o', ms=0.1,linewidth=0.5)

def runRandomWalkSimulationSimple(nSteps, nIteration):
    walker = randomWalk(nSteps)

    radialEndPosition = []
    radiusOfGyration = []
    endPositons = [] # list of vector of endPositons
    for _ in range(nIteration):
        walker.createPath()
        radialEndPosition.append( walker.calculateREndToEnd() )
        radiusOfGyration.append( walker.calculateRadiusOfGyration() )
        endPositons.append( walker.getEndPosition()[0] )

    rg = sum(radiusOfGyration)/len(radiusOfGyration)
    print(rg)
    endPosDistHisto, endPosDistBins = np.histogram(radialEndPosition, bins="auto")
    endPosDistinXHisto,endPosDistinXBins = np.histogram(endPositons, bins="auto")
    
    # make some plots:
    fig, (ax1, ax2, ax3) = plt.subplots(3) #, sharex=True

    # save to file:
    np.savetxt('rg'+str(nSteps)+'.dat',radiusOfGyration, comments="mean radius of gyration of random walk with "+str(nSteps)+ "steps =\n"+str(rg))
    ax1.plot(range(len(radiusOfGyration)),radiusOfGyration, 'g+')
    ax1.set( ylabel='Rg',xlabel='iteration' )

    x = endPosDistBins[0:(len(endPosDistHisto))] + ( endPosDistBins[1]-endPosDistBins[0] ) / 2.0 # x = center of bin
    # save to file:
    np.savetxt('reeDistr'+str(nSteps)+'.dat',np.column_stack( (x,endPosDistHisto) ))
    ax2.bar(x,endPosDistHisto, color='r')
    ax2.set( ylabel='frequency',xlabel='Ree')

    x = endPosDistinXBins[0:(len(endPosDistinXHisto))] + ( endPosDistinXBins[1]-endPosDistinXBins[0] ) / 2.0
    # save to file:
    np.savetxt('rxDistr'+str(nSteps)+'.dat',np.column_stack( (x,endPosDistinXHisto) ))
    ax3.bar(x,endPosDistinXHisto, color ='b')
    ax3.set( ylabel='frequency', xlabel='ReeX', )

    # save figure
    fig.savefig("RandomwalkPy.png")

    

# um das Programm ganz einfach zu nutzen diese Zeile auskommentieren, 
# die Parameter eventuell anpassen und die ganze Datei mit 
# "python3 myRandomWalk.py" ausführen

#runRandomWalkSimulationSimple(512,100)
