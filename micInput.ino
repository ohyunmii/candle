int dimCntTime = 0;
int dimmingCnt = 0;

void readMicInput() {
  if (Debug) Serial.println("reading mic input");

  float micReading = analogRead(MIC_PIN);
  micFilter.addValue(micReading, millis());

  //  Serial.print("Light Status: ");
  //  Serial.print(lightStatus);
  //  Serial.print(", ");
  //  Serial.print("Count: ");
  //  Serial.print(dimmingCnt);
  //  Serial.println();

  if (micReading > 40 && micFilter.getAverage() > 35) {
    if (Debug) Serial.println("dimming");
    if (millis() - dimCntTime > 2000) {
      dimmingCnt++;
      dimCntTime = millis();
    }

    if (dimmingCnt == 1) {
      lightStatus = BRIGHT_MED;
    } else if (dimmingCnt == 2) {
      lightStatus = BRIGHT_LOW;
    } else if (dimmingCnt >= 3) {
      lightStatus = OFF;
      //  dimmingCnt = 0; // reset
    }
  } else if (micReading > 800 && micFilter.getAverage() > 70) {
    if (Debug) Serial.println("light off");
    lightStatus = OFF;
  }

}

void lightControl(Light lightMode) {
  if (lightMode == OFF) {
    analogWrite(FLAME1, 0);
    analogWrite(FLAME2, 0);
    Melting = false;
  }
  else if (lightMode == BRIGHT_LOW) {
    int randVal = random(50);
    analogWrite(FLAME1, 0);
    analogWrite(FLAME2, randVal + 100);
    delay(random(100));
    Serial.println("LOW");
  }
  else if (lightMode == BRIGHT_MED) {
    int randVal = random(50);
    analogWrite(FLAME1, randVal + 100);
    analogWrite(FLAME2, randVal + 100);
    delay(random(100));
    Serial.println("MED");
  }
  else if (lightMode == BRIGHT_HIGH) {
    int randVal = random(100);
    analogWrite(FLAME1, randVal + 125);
    analogWrite(FLAME2, randVal + 125);
    delay(random(100));
    Serial.println("HIGH");
  }
  else if (lightMode == FLICKER) {
    int randVal = random(100);
    analogWrite(FLAME1, randVal + 50);
    analogWrite(FLAME2, randVal + 50);
    delay(random(500));
  }
}
