import serial
ser = serial.Serial(input("Please enter the address to an active serial port: "),
                    baudrate=int(input("Please enter a proper baud rate: ")),
                    timeout=int(input("Please enter a valid timeout duration: ")))  # open serial port
line = ser.read(size=100) # read upto 100 bytes
line = str(line) #convert to string
line = line[2:len(line)-1] #remove excess junk
lineList = list(line)
lineBin = ''.join(format(i, '08b') for i in bytearray(line, encoding ='utf-8')) # convert to bits
# outputs
print(line)
print(lineList)
print(lineBin)

# end the serial reader
ser.close()