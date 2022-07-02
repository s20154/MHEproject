import math
import random
from collections import namedtuple
import operator
from itertools import chain
from time import time
import matplotlib.pyplot as plt
import pandas as pd
import numpy as np

# Helper functions
def calculateDistance(bits):
    return math.fabs(sum(bitsToSet(bits)))

def bitsToSet(bits):
    set = []
    for i, bit in enumerate(list(bits)):
        if bit == True:
            set.append(DATA[i])
    return set

def getAllNeighbours(bits):
    allNeighbours = []

    for i in range(len(bits)):
        neighbour = []
        for bit in bits:
            neighbour.append(bit)
        neighbour[i] = (not neighbour[i])
        # Check if list isn't only [False, False, ...]
        if sum(neighbour) != 0:
            allNeighbours.append(neighbour)

    return allNeighbours

def getBestNeighbour(allNeighbours):
    if allNeighbours == []:
        return []
    allNeighboursDistance = {}
    for neighbour in allNeighbours:
        allNeighboursDistance[calculateDistance(neighbour)] = neighbour

    return allNeighboursDistance[min(allNeighboursDistance.keys())]

def getRandomNeighbour(bits):
    neighbour = []
    for bit in bits:
        neighbour.append(bit)

    randVal = random.choice(range(len(neighbour)))
    neighbour[randVal] = (not neighbour[randVal])

    return neighbour


with open('data.txt') as f:
    w = next(f)
    DATA = []
    for line in f: # read rest of lines
        DATA.append(int(line))

with open("bits.txt") as file:
    lines = file.readlines()
    lines = [line.rstrip() for line in lines]

BITS = []
for x in lines[0]:
    if x == '1':
        BITS.append(True)
    else:
        BITS.append(False)

DATA.insert(0, -99949535)








class TabuAlgo:
    def __init__(self, iterationsLimit):
        self.bits = BITS
        self.iterationsLimit = iterationsLimit
        self.i = 1
        self.tabuList = []
        self.stepsHeap = []
        self.bestBits = self.bits

    def execute(self):
        self.stepsHeap.append(self.bits)
        while self.i <= self.iterationsLimit and calculateDistance(self.bits) != 0 and self.stepsHeap != []:
            self.i += 1
            allNeighbours = getAllNeighbours(self.bits)
            allPossibleNeighbours = [n for n in allNeighbours if n not in self.tabuList]
            bestNeighbour = getBestNeighbour(allPossibleNeighbours)
            
            self.tabuList.append(self.bits)
            if bestNeighbour == []:
                self.bits = self.stepsHeap.pop()
            else:
                if calculateDistance(self.bits) > calculateDistance(bestNeighbour):
                    self.stepsHeap.append(self.bits)
                    self.bestBits = bestNeighbour
                self.bits = bestNeighbour

        return self.bestBits

tabuAlgo = TabuAlgo(100).execute()
print("Przed :", int(calculateDistance(BITS)))
print("Po    :", int(calculateDistance(tabuAlgo)))
