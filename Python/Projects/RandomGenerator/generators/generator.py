import psutil
import time
import random
import matplotlib.pyplot as plt
from collections import Counter


class Generator:

    def __init__(self, Process):
        self.seed = None
        self.process = Process()

        self.generateSeed()

    def generateSeed(self):
        cpu_stats = psutil.cpu_stats()
        self.seed = (cpu_stats[0] + cpu_stats[1], time.time())

    def setProcessInitValue(self):
        self.process.setInitValue(self.seed)

    def generate(self):
        self.generateSeed()
        self.setProcessInitValue()
        self.process.execute()
        return self.process.accumulationPoints


class Logistic:

    @staticmethod
    def logisticFunc(r, x):
        return r*x*(1-x)

    iterations = 1000
    last = 100

    def __init__(self, r=None, u_0=None):
        self.r = r
        self.u_0 = u_0
        self.accumulationPoints = []

    def setInitValue(self, value):
        #  3,57 ≤ r => comportement chaotique
        deci = 0
        while deci == 0 or deci == 1:
            deci = value[1] - int(value[1])
        r = 3.6 + deci * 0.1

        # x_0 dans ]0, 1[ => comportement chaotique
        U_0 = "0." + str(value[0])
        #print(deci, U_0)
        self.r, self.u_0 = float(r), float(U_0)

    def execute(self):
        u_n = self.u_0
        self.accumulationPoints = []
        for n in range(Logistic.iterations):
            u_n = Logistic.logisticFunc(self.r, u_n)
            if n >= Logistic.iterations - Logistic.last:
                self.accumulationPoints.append(u_n)


gen = Generator(Logistic)
randomNumbers = []

for i in range(10000):
    data = gen.generate()
    for i in range(len(data)):
        randomNumbers.append(round(data[i], 4))

dataOcc = Counter(randomNumbers)
data = dict(dataOcc)
# Tracer le graphe de points
plt.scatter(data.keys(), data.values(), color='blue')

# Ajouter des titres et des étiquettes
plt.title('Graphe de Points Simple')
plt.xlabel('Axe X')
plt.ylabel('Axe Y')

# Afficher la légende
plt.legend()

# Afficher le graphe
plt.show()

