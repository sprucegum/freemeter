
# FreeMeter

![A 2 gauge CPU/RAM Freemeter](2meters.jpg)

This repo contains the Arduino C code, Python based OS driver, and OpenSCAD designs for this [3D printable Analog CPU/RAM gauge project](https://www.thingiverse.com/thing:3484111).

# Usage

### Programming the Gauge Controller

Program your Arduino with the code found in **meter-arduino/meter-arduino.ino**

The Meter accepts a very basic serial protocol running at 9600 baud. 

### Host Requirements (Linux, Windows, Mac)

Install pyserial and psutil on your Linux, Windows, or Mac desktop. It would probably be better to install the dependencies in a python virtualenv, but for simplicity, I just install them globally.

    pip install -r requirements.txt

### Host/Meter Config

Open up **meter_config.yaml** in your favorite editor and modify it to suit your OS and meter setup.

##### Linux, 1 meter connected on pin 10

    port: '/dev/ttyUSB0'
    pins:
      10: CPU

##### Windows, 2 meters connected on pins 10 and 11

    port: 'COM3'
    pins:
      10: CPU
      11: RAM

##### Mac

I haven't tried it on a Mac but I think you can just use **/dev/tty.usbserial0** as the port and use brew to install python and pip.

### Running the host driver

Now that you've installed the dependencies and modified the config file, just run the 'meter' script:

    python meter
