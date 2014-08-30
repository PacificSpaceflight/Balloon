# high-altitude balloon data logger

# goals

* minimum weight
* SD card logging of pressure, altitude, and temperature
* 9V battery trigger release relay, to end the trip by inverting the balloon

# current part list

* DigiX by Digistump Arduino clone (onboard microSD)
* DS18B20 Dallas OneWire temperature sensors
* Adafruit BMP180 (temp, pressure, altitude)

# libraries

* OneWire (http://playground.arduino.cc/Learning/OneWire)
* Dallas Temperature (http://www.milesburton.com/Dallas_Temperature_Control_Library)
* Adafruit BMP (www.adafruit.com)

![sample](https://raw.github.com/PacificSpaceflight/Balloon/master/sensor.jpg)