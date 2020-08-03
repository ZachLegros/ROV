import numpy as np
from inputs import get_gamepad
from time import sleep
import os

DEAD_ZONE = 7500
# descend: ABS_RZ
# lock descend: BTN_TR
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
# [ABS_X, ABS_Y, ABS_Z, ABS_RZ, BTN_EAST, BTN_TL, BTN_TR, BTN_START]
inputs = np.zeros(8)

while 1:
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

   # inputs: [ABS_X, ABS_Y, ABS_Z, ABS_RZ, BTN_EAST, BTN_TL, BTN_TR, BTN_START]
   inputs[0] = np.round(gp_state['ABS_RX']/32767, 2)
   inputs[1] = np.round(-gp_state['ABS_Y']/32767, 2) 
   inputs[2] = np.round(gp_state['ABS_Z']/1023, 2)
   inputs[3] = np.round(gp_state['ABS_RZ']/1023, 2)
   inputs[4] = gp_state['BTN_EAST']
   inputs[5] = gp_state['BTN_TL']
   inputs[6] = gp_state['BTN_TR']
   inputs[7] = gp_state['BTN_START']
   
   os.system('clear')
   print(inputs, end='\r')
   
      
   
