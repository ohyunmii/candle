#include <RunningAverage.h>
#include <SparkFun_TB6612.h>
#include <Wire.h>

#define TOUCH_PIN              A0    //  Touch Sensor
#define MIC_PIN                A1    //  Mic Sensor
#define AIN1                    2    //  Motor Driver (A01-Motor V, A02-Motor Ground)
#define AIN2                    3
#define PWMA                    5
#define STBY                    4
#define TOUCH_SWITCH            6     //  To trigger touch input 
#define FLAME1                  7     //  LED
#define FLAME2                  8     //  LED
#define BOTTOM_LIMIT            9     //  For indicating min distance (linear actuator)
#define TOP_LIMIT              10     //  For indicating max distance (linear actuator) 
#define TOUCH_INPUT_DURATION 1200

RunningAverage touchFilter(1000);
RunningAverage micFilter(10);
RunningAverage upperLimitFilter(10);
RunningAverage lowerLimitFilter(10);
Motor candleMotor = Motor(AIN1, AIN2, PWMA, 1, STBY);

enum Light { BRIGHT_HIGH, BRIGHT_MED, BRIGHT_LOW, FLICKER, OFF };
Light lightStatus;
enum MotorPos { TOP, BOTTOM};
MotorPos motorLocation;

long currentSpeedTime = 0;
long currentBrakeTime = 0;
long touchStartTime = 0;
boolean readyForTouch = false;    //  Ready to process touch input
boolean readyForBlow = false;     //  Ready to process mic input
boolean Backward = false;
boolean Brake = false;
boolean meltingBegin = false;
boolean Melting = false;
boolean initializeMotorPos = false;
boolean startLight = false;
boolean Debug = true;
boolean runOnce = false;
const int motorSpeed = 255;
int Counter = 0;

void setup() {
  Serial.begin(9600);

  pinMode(TOUCH_SWITCH, INPUT);
  pinMode(TOUCH_PIN, INPUT);
  pinMode(TOP_LIMIT, INPUT);
  pinMode(BOTTOM_LIMIT, INPUT);
  pinMode(MIC_PIN, INPUT);
  pinMode(FLAME1, OUTPUT);
  pinMode(FLAME2, OUTPUT);

  //  Filter Initialization
  touchFilter.clear();
  micFilter.clear();
  upperLimitFilter.clear();
  lowerLimitFilter.clear();

  runOnce = true;
  lightStatus = OFF;
  motorLocation = BOTTOM;
}

void loop() {

  if (runOnce) {
    if (digitalRead(TOUCH_SWITCH)) {  //if reading from touch switch is equal to 1, this will be true
      readyForTouch = true;
      touchFilter.clear();
      touchStartTime = millis();
    }
  }

  if (readyForTouch) {
    runTouchInput();
  }
  if (readyForBlow) {
    readMicInput();
  }
  if (startLight) {
    limitSwitches();
    motorControl(30);  // input parameter: user time input in sec
  }

}

//ready for blow 수정


void candleReset() {
  if (Debug) Serial.println("Reset");
  touchFilter.clear();
  micFilter.clear();
  upperLimitFilter.clear();
  lowerLimitFilter.clear();

  lightStatus = OFF;
  Counter = 0;
  meltingBegin = true;
  Melting = false;
  readyForTouch = false;    //  Ready to process touch input
  readyForBlow = false;     //  Ready to process mic input
  motorLocation = BOTTOM;
  initializeMotorPos = false;
  startLight = false;
  runOnce = true;

}






