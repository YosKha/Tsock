import struct
import random
import sys

x0 = 0.125
µ = 3.8
I = 345

initSeed = [x0, µ, I]
seed = initSeed
data = []


def logisticFunc(x, r):
    return r*x*(1-x)


def new_r(r):
    uPrim = r+3.5699
    while uPrim > 4:
        uPrim = uPrim % 4
        uPrim = uPrim + 3.5699
    return uPrim


def new_I(I):
    while I < 1000:
        I = I+1000
    return I


def newSeed(lastSeed):
    r = lastSeed[1]
    I = lastSeed[2]
    return (lastSeed[0], new_r(r), new_I(I))


def logistic_map(x0, r, n):
    """Générateur de la suite logistique."""
    x = x0
    for i in range(n):
        x = logisticFunc(x, r)
    return x


def generateData(n, reset=True):
    global seed
    x0, r, I = seed

    for i in range(n):
        if i%10000 == 0:
            print(f'{i}/{n}\n')
        # récolte des valeurs
        newNumber = logistic_map(x0, r, I)
        data.append(newNumber * sys.maxint)
        # nouvelle graine
        seed = newSeed(seed)
        x0, r, I = seed


def copyInFile(fileName, data):
    with open(f'{fileName}.txt', 'w') as file:
        # Écrire dans le fichier
        for i in range(len(data)-1):
            file.write(f"{data[i]}\n")
        file.write(f"{data[len(data) - 1]}")


def floatToBinary(input: float) -> str:
    return ''.join(format(c, '08b') for c in struct.pack('!f', input))


"""def floatToBinaryOpt(input: float) -> str:
    getcontext().prec = 22
    d = Decimal(input)
    i = int(d * (1 << 21))
    return bin(i)"""


def generateWithPython (n):
    data = []
    for i in range(n) :
        if i % 10000 == 0:
            print(f'{i}/{n}\n')
        data.append(floatToBinary(random.uniform(0, 1)))
    return data


n = 1000
generateData(n)
#pythonData = generateWithPython(n)

copyInFile("data/data2.txt", data)
#copyInFile("pythonData", pythonData)



