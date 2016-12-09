import Adafruit_BBIO.UART as UART
import serial

#UART.setup("UART1")




while(1):
    ser = serial.Serial(port = "/dev/ttyS0", baudrate=9600)
    ser.close()
    ser.open()
    if ser.isOpen():
            print "Serial 1 is open!"
            ser.write("Hello 1 World!")
    ser.close()

    ser2 = serial.Serial(port = "/dev/ttyS1", baudrate=9600)
    ser2.close()
    ser2.open()
    if ser2.isOpen():
            print "Serial 2 is open!"
            ser2.write("Hello  World!")
    ser2.close()

    ser3 = serial.Serial(port = "/dev/ttyS2", baudrate=9600)
    ser3.close()
    ser3.open()
    if ser3.isOpen():
            print "Serial 3 is open!"
            ser3.write("Hello 3 World!")
    ser3.close()

    ser4 = serial.Serial(port = "/dev/ttyS4", baudrate=9600)
    ser4.close()
    ser4.open()
    if ser4.isOpen():
        print "Serial 4 is open!"
        ser4.write("Hello 4 World!")
    ser4.close()
