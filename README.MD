
# FreeMeter

![A 2 gauge CPU/RAM Freemeter](/2meters.jpg)

This repo contains the Arduino C code, Python based OS driver, and OpenSCAD designs for this [3D printable Analog CPU/RAM gauge project](https://www.thingiverse.com/thing:3484111).

# Usage

Program your arduino with the meter-arduino.ino code. It's currently configured for a single gauge wired to pin 11 on the Arduino.

Install pyserial and psutil on your Linux, Windows, or Mac, I recommend using a virtualenv.

    pip install -r requirements.txt

Run the 'meter' script:

    python meter
