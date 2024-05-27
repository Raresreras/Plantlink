import serial

ser = serial.Serial()
ser.port = 'COM8'
ser.baudrate = 9600
ser.timeout = None

ser.open()
while (True):
    bValue = ser.readline()
    try:
        value = int(str(bValue)[:-5][9:])
    except:
        continue

    
    try:
        if ('V' == chr(value)):
            bValue = ser.readline()
            value = int(str(bValue)[:-5][9:])
            print("Board code " + str(value) + " has transmitted the following data")
    except:
        continue

    try:
        if ('T' == chr(value)):
            bValue = ser.readline()
            value = int(str(bValue)[:-5][9:])
            print("Temperature is " + str(value))
    except:
        continue

    try:
        if ('H' == chr(value)):
            bValue = ser.readline()
            value = int(str(bValue)[:-5][9:])
            print("Humidity is " + str(value))
    except:
        continue

    try:
        if ('S' == chr(value)):
            bValue = ser.readline()
            value = int(str(bValue)[:-5][9:])
            print("Soild humidity is " + str(value))
    except:
        continue
ser.close()