import serial
import time

arduino = serial.Serial(port='COM3', baudrate=9600, timeout=.1)

def write_read(x):
    data = arduino.write(bytes(x, 'utf-8'))
    #time.sleep(0.05)
    #data = arduino.readline()
    return data     #returns the number of bytes written

def read_data():
    return arduino.read(2)

while True:
    readval = read_data().decode('ascii')
    #print("\nArduino reading     -->   ")
    print(readval)
