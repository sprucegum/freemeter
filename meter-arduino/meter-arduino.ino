/*
  Simple 3-Gauge Meter
 */
#define METERS 3
int meterPins[3] =  {3,10,11};
int targetValues[METERS];
int currentValues[METERS];
unsigned long nextTick[METERS];
int meterVelocity[METERS];
int charsReceived = 0;


unsigned long messageDelay = 1000000; // 1 second to start
unsigned long currentTime = micros();
unsigned long oldTime = currentTime - messageDelay;
unsigned long targetTime = currentTime + messageDelay; // when we think we'll get the next serial update

bool dataReady = false;

/**
 * Setup the serial port, activate the pins
 */
void setup() {
  // initialize serial:
  Serial.begin(9600);
  for (int meter=0; meter < METERS; meter++) {
    pinMode(meterPins[meter], OUTPUT);
  }
}

/**
 * Check to see if any of the meters are scheduled to run right now.
 * @todo: switch from polling to an interrupt
 */
void loop() {
  if (dataReady) {
    currentTime = micros();
    for (int meter = 0;meter < METERS; meter++) {
      if (currentTime >= nextTick[meter]) {
        updatePin(meter);
      }
    }
  }
}

/**
 * Run the scheduler then move the pin by one.
 */
void updatePin(int meter) {
  updateScheduleForMeter(meter);
  currentValues[meter] = constrain(currentValues[meter] + meterVelocity[meter], 0, 255); 
  analogWrite(meterPins[meter], currentValues[meter]);
}

/**
 * For a given meter, calculate the next time we should move the meter.
 */
void updateScheduleForMeter(int meter) {
  //int currentTime = micros();
  int targetValue = targetValues[meter];
  int currentValue = currentValues[meter];
  int deltaValue = targetValue - currentValue;
  unsigned long remainingTime = (targetTime - currentTime) + messageDelay/20; // currentTime global is updated in the loop.
  if (remainingTime > 0) {
    if (deltaValue) {
      nextTick[meter] = remainingTime/abs(deltaValue) + currentTime;
      meterVelocity[meter] = (deltaValue > 0) ? 1 : -1; 
    }
  } else {
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
    targetValues[charsReceived] = inChar;
    charsReceived++;
    if (charsReceived == METERS) {
      currentTime = micros();
      messageDelay = currentTime - oldTime;
      oldTime = currentTime;
      targetTime = currentTime + messageDelay;
      charsReceived = 0;
      if (!dataReady) {
        dataReady = true;
      }
      for (int meter=0; meter < METERS; meter++) {
        updateScheduleForMeter(meter);
      }
    }
  }
}

