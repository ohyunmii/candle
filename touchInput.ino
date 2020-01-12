int touchInsTime = 0;
void runTouchInput() {
  if (Debug) Serial.println("Receiving Touch Input");

  if (millis() - touchStartTime <= TOUCH_INPUT_DURATION) {
    float touch = (float) analogRead(TOUCH_PIN);
    //        Serial.println(touch);
    if (touch > 90 && touch < 930) {
      touchFilter.addValue(touch, (float)(millis()));
    }
    touchInsTime = millis();
  } else {
    Serial.print("TOUCHED:: ");
    Serial.print(touchFilter.getMinMaxDifference());
    Serial.println();
    if (touchFilter.getMinMaxDifference() > 90 && touchFilter.getMinMaxDifference() <= 180) {
      readyForTouch = false;
      runOnce = false;
      initializeMotorPos = true;
      startLight = true;
    } else {
      if (millis() - touchInsTime <= 2000) {
        lightControl(FLICKER);
      } else{
        lightControl(OFF);
      }
    }

  }
}



