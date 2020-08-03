class Motor:
   MAX_VOLT = 12

   def __init__(self):
      self.voltage = 0
      self.locked = 0


   def toggle_lock(self):
      if self.voltage != 0:
         if self.locked == 1:
            self.locked = 0
         else:
            self.locked = 1


   def map(self, val, val_max):
      """
      maps values between -1 and 1 to the motor's voltage range
      """

      return Motor.MAX_VOLT * val


   def set_voltage(self, val):
      if not self.locked:
         if val > Motor.MAX_VOLT:
            self.voltage = Motor.MAX_VOLT
         elif val < -Motor.MAX_VOLT:
            self.voltage = -Motor.MAX_VOLT
         else:
            self.voltage = val
   

   def stop(self):
      if self.locked == 1:
         self.locked = 0

      self.voltage = 0



class ROV:

   def __init__(self):
      # forward and backward
      self.motor1 = Motor()
      self.motor2 = Motor()
      # up and down
      self.motor3 = Motor()
      self.lights = 0

      
   def move_x(self, val):
      voltage = Motor.map(val)
      self.motor1.set_voltage(voltage)
      self.motor2.set_voltage(voltage)

   
   def move_y(self, val):
      voltage = Motor.map(val)
      if self.motor3.locked == 1:
         self.motor3.toggle_lock()

      self.motor3.set_voltage(voltage)


   def set_lights(self, val):
      """
      1: on, 0: off
      """
      self.lights = val


   def stop(self):
      self.motor1.stop()
      self.motor2.stop()
      self.motor3.stop()