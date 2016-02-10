# Author: Brandon Nguyen
#
# This script will generate a c array sine table given various inputs
import math

#This function returns the value

print("Sine/cose table generator")
name = input("Table name: ")
numBits = int(input("How many bits is your DAC?: "))
numElements = int(input("How many elements do you want to keep?: "))
numLines = int(input("How many lines do you want the array to be formatted to?: "))
numPerLine = numElements / numLines;

f = open(name + ".txt","w")
fileOutput = ("const uint8_t " + name + "[" + str(numElements) + "] =\n\t{\t")

mathResult = math.floor(math.sin(2 * math.pi * 0/numElements) * (int(math.pow(2,numBits-1)-1) ) + int(math.pow(2,numBits-1)) - 0)
#fileOutput +=(str(int(math.floor(math.sin(2 * math.pi * 0 /numElements ) * int(math.pow(2,numBits-1))) + int(math.pow(2,numBits-1))-1)))
fileOutput += str(mathResult)
lineEntries = 1
for x in range (1,numElements):
        if(lineEntries == numPerLine):
            fileOutput += ("\n\t\t")
            lineEntries = 0
        mathResult = math.floor(math.sin(2 * math.pi * x/numElements) * (int(math.pow(2,numBits-1)-1) ) + int(math.pow(2,numBits-1)) - 0)
        fileOutput += (", " + str(mathResult))
        lineEntries += 1
fileOutput +=("\n\t};")

print(fileOutput)
f.write(fileOutput)
f.close
