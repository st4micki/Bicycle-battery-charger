Software part of Bicycle-battery-charger project based on Arduino UNO R3 board. The program is written to monitor INR18650-25R lithium-ion battery.
Features:
 - Reading voltage on battery terminals
 - Reading load current
 - Program takes battery voltage drops under load into consideration
 - approximating battery charge percentage based on discharge curve
 - Battery percentage is being displayed on 16x2 LCD display
 - I2C communication with LCD driver

Pinout:
battery voltage measurement - A0
current sensor output - A1
I2C SDA line - SDA
I2C SCL line - SCL
