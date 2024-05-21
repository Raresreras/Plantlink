import serial

ser = serial.Serial()
ser.port = 'COM10'
ser.baudrate = 9600
ser.timeout = None

ser.open()
while (True):
    value = ser.readline()
    if ('V' = chr(value)):
        value = ser.readline()
        print("Board code " + str(value) + " has transmitted the following data")
    elif ('T' = chr(value)):
        value = ser.readline()
        print("Temperature is " + str(value))
    elif ('H' = chr(value)):
        value = ser.readline()
        print("Humidity is " + str(value))
    elif ('S' = chr(value)):
        value = ser.readline()
        print("Soil humidity is " + str(value))
ser.close()