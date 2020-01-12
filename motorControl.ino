void limitSwitches() {
  int topLimit = digitalRead(TOP_LIMIT);
  upperLimitFilter.addValue(topLimit, millis());
  int bottomLimit = digitalRead(BOTTOM_LIMIT);
  lowerLimitFilter.addValue(bottomLimit, millis());

  //  if (motorLocation == BOTTOM && upperLimitFilter.getAverage() == 1) {
  if (motorLocation == BOTTOM && topLimit == 1) {
    motorLocation = TOP;
  } else if (motorLocation == TOP && bottomLimit == 1) {
    motorLocation = BOTTOM;
  }
}

void motorControl(int inputLastingTime) {

  float speedLastTime = (16 / 4) * 1000;
  float brakeLastTime = (inputLastingTime / 4) * 1000 - speedLastTime;

  if (initializeMotorPos) {
    if (Debug) Serial.println("driving motor upward until top limit switch is pressed");
    candleMotor.drive(motorSpeed);
    if (motorLocation == TOP) {
      if (Debug) Serial.println("melting begin");
      initializeMotorPos = false;
      Melting = true;
      meltingBegin = true;
    }
  } else {
    if (Melting == true) {
      if (Debug) Serial.println("melting");
      if (meltingBegin) {
        lightStatus = BRIGHT_HIGH;
        readyForBlow = true;
        BrakeTimeUpdate();
        Brake = true;
        meltingBegin = false;
      }


      if (Counter <= 4) { // Drive the motor downward in four phases.
        lightControl(lightStatus);
        readMicInput();
        if (Brake) {
          if (millis() - currentBrakeTime <= brakeLastTime) {
            if (Debug) Serial.println("Motor Pause");
            candleMotor.brake();
          } else {
            Brake = false;
            Backward = true;
            BackwardTimeUpdate();
          }
        }

        if (Backward) {
          if (millis() - currentSpeedTime <= speedLastTime) {
            if (Debug) Serial.println("Motor Drive Backward");
            candleMotor.drive(-motorSpeed);
          } else {
            Backward = false;
            Counter++;
            BrakeTimeUpdate();
            Brake = true;
          }
        }
      } // end of if(Counter <= 4)
      else {
        if (Debug) Serial.println("Counter > 4");
        Melting = false;
      }
    } // end of if(Melting)
    else {
      if (Debug) Serial.println("Melting Done:: get ready for reset");
      lightControl(OFF);
      readyForBlow = false;
      if (motorLocation == BOTTOM) {
        if (Debug) Serial.println("Motor is now back to initial position. Reset");
        candleMotor.brake();
        candleReset();
      } else {
        if (Debug) Serial.println("Motor lowering");
        candleMotor.drive(-motorSpeed);
      }
    }
  }
}

void BackwardTimeUpdate() {
  currentSpeedTime = millis();
}
void BrakeTimeUpdate() {
  currentBrakeTime = millis();
}

