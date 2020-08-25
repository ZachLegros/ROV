import socket
import serial
import time   
import sys
import glob
import numpy as np
from inputs import get_gamepad
from time import sleep
import os

DEAD_ZONE = 7500
# descend: ABS_RZ
# lock descend: BTN_TRc
# ascend: ABS_Z
# lock ascend: BTN_TL
# lights: BTN_START
# turn: ABS_RX
# cut all motors: BTN_EAST
# forward/backwards: ABS_Y
gp_state = {#'ABS_HAT0X' : 0, #-1 to 1
             #'ABS_HAT0Y' : 0, #-1 to 1
             'ABS_RX' : 0, #-32768 to 32767
             #'ABS_RY' : 0, #-32768 to 32767
             #'ABS_X' : 0, #-32768 to 32767
             'ABS_Y' : 0, #-32768 to 32767
             'ABS_Z' : 0, #0 to 255
             'ABS_RZ' : 0, #0 to 255
             'BTN_EAST' : 0,
             'BTN_TR' : 0,
             'BTN_TL' : 0,
             'BTN_START' : 0,
             #'BTN_NORTH' : 0,
             #'BTN_SELECT' : 0,
             #'BTN_SOUTH' : 0,
             #'BTN_THUMBL' : 0,
             #'BTN_THUMBR' : 0,
             #'BTN_WEST' : 0,
             #'SYN_REPORT' : 0,
             }


def map_range(value, leftMin, leftMax, rightMin, rightMax):
   leftSpan = leftMax - leftMin
   rightSpan = rightMax - rightMin

   valueScaled = float(value - leftMin) / float(leftSpan)

   # Convert the 0-1 range into a value in the right range.
   return rightMin + (valueScaled * rightSpan)


def read_keypad():
   # [ABS_X, ABS_Y, ABS_Z, ABS_RZ, BTN_EAST, BTN_TL, BTN_TR, BTN_START]
   events = get_gamepad()
   for event in events:
      code = event.code
      state = event.state
      if (code == 'ABS_Y' or code == 'ABS_RX'):
         if state < 0 and state > -DEAD_ZONE:
            gp_state[code] = 0
         elif state > 0 and state < DEAD_ZONE:
            gp_state[code] = 0
         else:
            gp_state[code] = state
      else:
         gp_state[code] = state

   # inputs: [ABS_RX, ABS_Y, ABS_Z, ABS_RZ, BTN_EAST, BTN_TL, BTN_TR, BTN_START]
   ABS_RX = int(map_range(gp_state['ABS_RX'], 0, 32767, 0, 255))
   ABS_Y = int(-map_range(gp_state['ABS_Y'], 0, 32767, 0, 255))
   ABS_Z = int(map_range(gp_state['ABS_Z'], 0, 1023, 0, 255))
   ABS_RZ = int(map_range(gp_state['ABS_RZ'], 0, 1023, 0, 255))
   BTN_EAST = gp_state['BTN_EAST']
   BTN_TL = gp_state['BTN_TL']
   BTN_TR = gp_state['BTN_TR']
   BTN_START = gp_state['BTN_START']

   return ABS_RX, ABS_Y, ABS_Z, ABS_RZ, BTN_EAST, BTN_TL, BTN_TR, BTN_START


def serial_ports():
    """ Lists serial port names

        :raises EnvironmentError:
            On unsupported or unknown platforms
        :returns:
            A list of the serial ports available on the system
    """
    if sys.platform.startswith('win'):
        ports = ['COM%s' % (i + 1) for i in range(256)]
    elif sys.platform.startswith('linux') or sys.platform.startswith('cygwin'):
        # this excludes your current terminal "/dev/tty"
        ports = glob.glob('/dev/tty[A-Za-z]*')
    elif sys.platform.startswith('darwin'):
        ports = glob.glob('/dev/tty.*')
    else:
        raise EnvironmentError('Unsupported platform')

    result = []
    for port in ports:
        try:
            s = serial.Serial(port)
            s.close()
            result.append(port)
        except (OSError, serial.SerialException):
            pass
    return result 


def openSerial():
    global ser
    valid = False
    while(not valid):
        time.sleep(1)
        try:
            ser = serial.Serial(serial_ports()[0], 4800, timeout=1)
            ser.flush()
            valid = True
        except:
            valid = False


def getPWM(value):
   value = abs(value)
   if value <= 0:
      return 0
   elif value > 0 and value < 64:
      return 63
   elif value >= 64 and value < 128:
      return 127
   elif value >= 128 and value < 192:
      return 191
   elif value >= 192 and value < 256:
      return 255
   return 0


def getHexString(chars):
   return [hex(ord(c)).split('x')[-1] for c in ''.join(chars)]

openSerial()

while True:   
   try:
      ABS_RX, ABS_Y, ABS_Z, ABS_RZ, BTN_EAST, BTN_TL, BTN_TR, BTN_START = read_keypad()
   except:
      continue

   try:
      bytesToSend = bytes([getPWM(ABS_RX), getPWM(ABS_Y), getPWM(ABS_Z), getPWM(ABS_RZ), 1])
      print(bytesToSend)
      ser.write(bytesToSend)
   except:
      openSerial()

   # try:
   #    client.send(b'5')
   # except:
   #    break

   sleep(0.005)
   
