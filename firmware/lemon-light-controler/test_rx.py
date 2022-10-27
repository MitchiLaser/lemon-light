import serial
import numpy as np

# configure the serial connections
ser = serial.Serial(port='/dev/ttyUSB0', baudrate=115200)

boxes_cols = 1
boxes_rows = 2

m = np.zeros((boxes_rows*4, boxes_cols*6), dtype=bool)

i = 0
while True:
    for c in range(boxes_cols):
        for r in range(boxes_rows):
            dat = ser.read(3)
            print("read:", dat)
            for bit in range(3*8):
                m[4*r + bit%4, 6*c + bit//4] = (dat[bit//8] >> (bit%8)) & 1 
    i += 1
    print("---- res", i)
    for r in range(m.shape[0]):
        for c in range(m.shape[1]):
            print("#"  if m[r, c] else "~", end=" ")
        print()
    print("--"*m.shape[1])