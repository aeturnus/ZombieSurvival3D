filename = input("Specify file .bmml :: ")
f = open(filename + ".bmml" ,"r")
filecont = f.read()
filecont = filecont.replace('\n','')
f.close
print("Input:\n" + filecont + " ")

trackCount = 0
filesize = 0
fileout = ""
buffer = ""
pos = 0
encnum = "" #number encountered

length = len(filecont)
#todo: turn capital into non-capital, support for symbols, not require a termination pipe, comment support, ignore non relevant characters
#while(ord(filecont[pos]) != 124):                               #pipe character tells that it is the end
while(pos<length):
    filesize = 0
    trackCount += 1
    fileout = ""
    while(ord(filecont[pos]) != 93):
        if(ord(filecont[pos]) > 47 and ord(filecont[pos]) < 58):  #if a char of num is encountered
            encnum += filecont[pos]
            if(ord(filecont[pos+1]) > 47 and ord(filecont[pos+1]) < 58):
                encnum += filecont[pos+1]
                pos += 1
                if(ord(filecont[pos+1]) > 47 and ord(filecont[pos+1]) < 58):
                    encnum += filecont[pos+1]
                    pos += 1
            fileout += "\\x"
            if(int(encnum) < 16):
                fileout += "0"
            if(int(encnum) < 255):
                fileout += "{0:x}".format(int(encnum))
            else:
                fileout += "FF"
            fileout += "\"\""
            encnum = ""
        else:
            fileout += filecont[pos]
        pos += 1
        filesize += 1
    print("Track " + str(trackCount) + "Output:\n" + fileout)
    metadata = "const uint8_t " + filename + str(trackCount) + "[] = \""     #+1 for null character
    buffer += metadata + fileout + "\";  //"+ str(filesize + 1) +"\n"
    pos += 1

buffer += "\n"
buffer += "const uint8_t* " + filename + "[] = {"
i = 1
while (i<=trackCount):
    buffer += (filename + str(i))
    i+=1
    if(i<trackCount+1):
        buffer+=","
buffer += "};\n"
buffer += "//{"+str(trackCount)+",(uint8_t**)"+filename+"}"
print("Buffer:\n" + buffer)
f = open(filename + ".txt","w")
f.write(buffer)
f.close()

input("Press 'enter' or 'return' to exit...")
