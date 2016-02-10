filename = input("Specify file .bmml :: ")
f = open(filename + ".bmml" ,"r")
filecont = f.read()
f.close
print("Input:\n" + filecont + " ")

filesize = 0
fileout = ""
pos = 0
encnum = "" #number encountered
#todo: turn capital into non-capital, support for symbols, not require a termination pipe, comment support, ignore non relevant characters
while(ord(filecont[pos]) != 124):                               #pipe character tells that it is the end
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
print("Output:\n" +fileout)
metadata = "const uint8_t " + filename + "[" + str(filesize + 1) + "] = \"" #+1 for null character
f = open(filename + ".txt","w")
f.write(metadata + fileout + "\";")
f.close()

input("Press 'enter' or 'return' to exit...")
