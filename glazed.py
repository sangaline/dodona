#Relevent functions and their definitions for the genetic algorithm

from donut import cruller, bearclaw

from random import random
import numpy as np


#Takes a generation (current list of keyboards) and evolves a new generation based on a genetic-ish algorithm
#kList is the current generation (list) of keyboards, and inputModel is the inputModel to be used in the fitness function
#returns newkList which is the new generation (list) of keyboards

def Evolve(kList, inputModel):
    nk = len(kList)
    kExtList = []
    chList = []
    for i in range(nk):

        ################## Need to substitute in real fitness function here #######################

        #        kExtList.append((i,cruller.FittnessFunction(kList[i], inputModel), kList[i]))
        #        chList.append((i,cruller.FittnessFunction(kList[i], inputMOdel)))
        randomFitness = np.random.rand()

        ###########################################################################################

        kExtList.append((i,randomFitness, kList[i]))
        chList.append((i,randomFitness))

        print(kList[i].OrderedKeyList())

    #Pick pairs to repopulate the new generation with
    #Since each pair reproduces two new chromosomes this only needs to be done nChromosomes/2 times
    newkList = []
    for i in range(int(nk/2)):
        partnerA_index = NaturalSelection(chList)
        partnerB_index = NaturalSelection(chList)

        partnerA_letters = kList[partnerA_index].OrderedKeyList()
        partnerB_letters = kList[partnerB_index].OrderedKeyList()
#        partnerA_letters = list(dA.keys())
#        partnerB_letters = list(dB.keys())
        
        #pick random crossover position
        co_index = int(np.random.rand()*26)
        
        #Get freaky and make babies
        newChromeA_letters, newChromeB_letters = SwapGenes(partnerA_letters, partnerB_letters, co_index)

        #create new keyboard corresponding to the new chromosomes created above
        newKeyboardA_string = ''.join(newChromeA_letters)
        newKeyboardB_string = ''.join(newChromeB_letters)
        newKeyboardA = bearclaw.MakeStandardKeyboard(newKeyboardA_string)
        newKeyboardB = bearclaw.MakeStandardKeyboard(newKeyboardB_string)

        newkList.append(newKeyboardA)
        newkList.append(newKeyboardB)

    return newkList




#Fitness proportionate selection
#chList is a list of tuples containing the enumerated integer identity of the keyboard and it's fitness

def NaturalSelection(chList):
    weights = []

    for i, chEntry in enumerate(chList):
        weights.append(chEntry[1])

    choice = np.random.rand()*sum(weights)
    for i,w in enumerate(weights):
        choice -= w
        if choice < 0:
            return i




#Swap the "genes" (letters) up to a the cross over index (co_index) from two keyboards
#keyBoard_A and keyBoard_B are just the enumerated lists of letters corresponding to the actual keyboards

def SwapGenes(keyBoard_A, keyBoard_B, co_index):
    keyBoard_A = [a for a in keyBoard_A]
    keyBoard_B = [b for b in keyBoard_B]
        
    newGenesA = keyBoard_B[0:co_index] #the new genes to be swapped into chromosome A
    newGenesB = keyBoard_A[0:co_index] #   "           "            "  chromosome B

    oldGenesA = keyBoard_A[co_index:len(keyBoard_A)] #the unnaffected genes from chromosome A
    oldGenesB = keyBoard_B[co_index:len(keyBoard_B)] # "      "          "       chromosome B

    newChromeA = newGenesA+oldGenesA
    newChromeB = newGenesB+oldGenesB

    swpCommon = list(set(newGenesA)&set(newGenesB))  #find any genes that were swapped from both chromosomes
    for i in range(len(swpCommon)):              #remove the swapped letters that each chromosome had in common from the list of letters that
        newGenesA.remove(swpCommon[i])           #still need to be swapped.  This is because with every letter we swap we end up with a duplicate
        newGenesB.remove(swpCommon[i])           #in the new chromosome so we need to remove the duplicates from both chromosomes simply by swapping them but
                                                 #if the same letter is present in both swapped genes then no duplicates will be created.

    swpPairs = []
    for i in range(len(newGenesA)):                    #This pairs the remaining swapped letters so we can take are of the duplicates easily
        swpPairs.append((newGenesA[i],newGenesB[i]))


    tmpChromeA = newChromeA[co_index:len(newChromeA)]
    tmpChromeB = newChromeB[co_index:len(newChromeB)]
    for i in range(len(swpPairs)):                                                     #For every pair of duplicates
        tmpChromeA = [swpPairs[i][1] if x==swpPairs[i][0] else x for x in tmpChromeA]  #Find the duplicate (after the co_index) and swap it with it's pair
        tmpChromeB = [swpPairs[i][0] if x==swpPairs[i][1] else x for x in tmpChromeB]

    finalChromeA = newChromeA[0:co_index]+tmpChromeA
    finalChromeB = newChromeB[0:co_index]+tmpChromeB

    return finalChromeA, finalChromeB


