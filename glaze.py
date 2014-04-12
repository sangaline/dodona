#Relevent functions and their definitions for the genetic algorithm

from donut import *

from random import random

#keyBoard_A and keyBoard_B are just the enumerated lists of letters corresponding to the actual keyboards
def SwapGenes(keyBoard_A, keyBoard_B, co_index):
    newGenesA = keyBoard_B[0:co_index] #the new genes to be swapped into chromosome A
    newGenesB = keyBoard_A[0:co_index] #   "           "            "  chromosome B

    oldGenesA = keyBoard_A[co_index:len(keyBoard_A)] #the unnaffected genes from chromosome A
    oldGenesB = keyBoard_B[co_index:len(keyBoard_B)] # "      "          "       chromosome B

    newChromeA = newGenesA+oldGenesA
    newChromeB = newGenesB+oldGenesB
    
    swpCommon = list(set(newGenesA)&set(newGenesB))  #find any genes that were swapped from both chromosomes
    for i in range(len(swpCommon)):
        newGenesA.remove(swpCommon[i])
        newGenesB.remove(swpCommon[i])

    swpPairs = []
    for i in range(len(newGenesA)):
        swpPairs.append((newGenesA[i],newGenesB[i]))


    tmpChromeA = newChromeA[co_index:len(newChromeA)]
    tmpChromeB = newChromeB[co_index:len(newChromeB)]
    for i in range(len(swpPairs)):
        tmpChromeA = [swpPairs[i][1] if x==swpPairs[i][0] else x for x in tmpChromeA]
        tmpChromeB = [swpPairs[i][0] if x==swpPairs[i][1] else x for x in tmpChromeB]

    finalChromeA = newChromeA[0:co_index]+tmpChromeA
    finalChromeB = newChromeB[0:co_index]+tmpChromeB

    return finalChromeA, finalChromeB










   
