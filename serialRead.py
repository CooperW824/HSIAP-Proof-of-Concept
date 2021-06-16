import serial
ser = serial.Serial('/dev/ttyACM0', baudrate=9600, timeout=10)  # open serial port
line = ser.read(size=100)
line = str(line)
line = line[2:len(line)-1]
lineList = list(line)
lineBin = ''.join(format(i, '08b') for i in bytearray(line, encoding ='utf-8'))
print(line)
print(lineList)
print(lineBin)
ser.close()