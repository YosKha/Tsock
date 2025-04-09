import random
import struct
import numpy as np

mersenneRandomNumber = []
NB_RANDOM_NUMBERS = 100000
NB_BITSTREAMS = 15
path = "data/mersenneData"


def copyInFile(fileName, data):
    with open(f'{fileName}', 'w') as file:
        # Écrire dans le fichier
        for i in range(len(data)-1):
            file.write(f"{data[i]}")
        file.write(f"{data[len(data) - 1]}")


def testFileLen(fileName):
    nbLine = 0
    with open(f'{fileName}', 'r') as file:
        for _ in file:
            nbLine += 1
    print(nbLine)
    return nbLine


def floatToBinary(input: float) -> str:
    return ''.join(format(c, '08b') for c in struct.pack('!f', input))


def pythonGeneration():
    for i in range(NB_RANDOM_NUMBERS):
        if i%10000 == 0 :
            print(f"{i}/{NB_RANDOM_NUMBERS}")
        mersenneRandomNumber.append(random.randint(0, 1))


def numpyGeneration():
    floats = np.random.random(NB_RANDOM_NUMBERS)
    for fl in floats:
        mersenneRandomNumber.append(floatToBinary(fl))


numpyGeneration()
copyInFile(path, mersenneRandomNumber)
print("---COPIED---")
testFileLen(path)
testFileLen("data/data2.pi")
print("---TESTED---")

