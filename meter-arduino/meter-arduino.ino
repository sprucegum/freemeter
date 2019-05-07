/*     
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/*
  Multi-Gauge Analog Meter
  Author: Jade Lacosse
 */
#define PINS 12
#define ONE_SECOND  1000000
#define SLEEP_DELAY 2000000
#define ATTACK      125000
#define RELEASE     2000000
int VALID_OUTPUTS[] = {0, 0, 0, 1, 0, 1, 1, 0, 0, 1, 1, 1}; // Pins 3, 5, 6, 9, 10 , and 11 allow PWM output.
int pinState[PINS]; // when first writing to a pin, we set it to OUTPUT
int targetValues[PINS];
int currentValues[PINS];
int meterVelocity[PINS];
int charsReceived = 0;
int lastChar = 0;
int currentPin = 0;

unsigned long nextTick[PINS];
unsigned long messageDelay = ONE_SECOND; // 1 second to start
unsigned long currentTime = micros();
unsigned long lastMessageTime = currentTime;
unsigned long targetTime = currentTime + messageDelay; // when we think we'll get the next serial update

bool dataReady = false;

/**
 * Setup the serial port, activate the pins
 */
void setup() {
  // initialize serial:
  Serial.begin(9600);
  for (int pin=0; pin < PINS; pin++) {
    if (VALID_OUTPUTS[pin] == 1) {
      pinMode(pin, OUTPUT);
    }
  }
}

/**
 * Check to see if any of the meters are scheduled to run right now.
 * @todo: switch from polling to an interrupt
 */
void loop() {
  currentTime = micros();
  if (dataReady) {
    for (int pin = 0;pin < PINS; pin++) {
      if (pinState[pin] == 1) { // If the pin is turned on, check to see if it's updated.
        if (currentTime >= nextTick[pin]) {
          updatePin(pin);
        }
      }
    }
  }
  if ((currentTime - lastMessageTime) > SLEEP_DELAY) {
    sleep();
  }
}

/**
 * Bring the gauges down to zero, prepare the meter for its next message.
 */
void sleep() {
  for (int meter=0; meter < PINS; meter++) {
    targetValues[meter] = 0;
  }
  charsReceived = 0;
  lastMessageTime = currentTime;
  targetTime = currentTime + SLEEP_DELAY;
}

/**
 * Run the scheduler then move the pin by one.
 */
void updatePin(int meter) {
  updateScheduleForMeter(meter);
  currentValues[meter] = constrain(currentValues[meter] + meterVelocity[meter], 0, 255); 
  analogWrite(meter, currentValues[meter]);
}

/**
 * For a given meter, calculate the next time we should move the meter.
 */
void updateScheduleForMeter(int meter) {
  int targetValue = targetValues[meter];
  int currentValue = currentValues[meter];
  int deltaValue = abs(targetValue - currentValue);
  unsigned long remainingTime = max(targetTime - currentTime, (targetValue > currentValue) ? ATTACK : RELEASE); // currentTime global is updated in the loop.
  if (deltaValue) {
    nextTick[meter] = (remainingTime/abs(deltaValue)) + currentTime;
    meterVelocity[meter] = (targetValue > currentValue) ? 1 : -1;
  } else {
    nextTick[meter]= currentTime + remainingTime;
    meterVelocity[meter] = 0;
  }
}

/*
  SerialEvent occurs whenever a new data comes in the
 hardware serial RX.  This routine is run between each
 time loop() runs, so using delay inside loop can delay
 response.  Multiple bytes of data may be available.
 */
void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    int inChar = (int) Serial.read();
    if (inChar == 255 && lastChar == 255) {
      Serial.write(255);
      charsReceived = 0;
      currentTime = micros();
      messageDelay = min(ONE_SECOND, max(currentTime - lastMessageTime, 10000)); // Limit to 1-1000hz.
      lastMessageTime = currentTime;
      targetTime = currentTime + messageDelay;
      if (!dataReady) {
        dataReady = true;
      }
      for (int meter=0; meter < PINS; meter++) {
        if (VALID_OUTPUTS[meter] == 1) {
          updateScheduleForMeter(meter);
        }
      }
    } else {
      if (charsReceived == 0) {
        currentPin = inChar;
        charsReceived++;

      } else {
        targetValues[currentPin] = inChar;
        pinState[currentPin] = 1;
        charsReceived = 0;
        //Serial.write(currentPin);
        //Serial.write(inChar);
      }
    }
    lastChar = inChar;
  }
}
