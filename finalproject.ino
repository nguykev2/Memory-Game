#include <Adafruit_CircuitPlayground.h>
#include <AsyncDelay.h>

const int leftButtonPin = 4; 
const int rightButtonPin = 5; 
volatile bool lbuttonFlag = 0; 
volatile bool rbuttonFlag = 0; 
const int switchPin = 7; 
volatile bool switchFlag = 0; 
bool switchVal = 0;
long randColor;
int level = 1; 

void setup() {
  Serial.begin(9600);
  CircuitPlayground.begin();
  randomSeed(analogRead(0)); 
  
  pinMode(leftButtonPin, INPUT_PULLDOWN);
  attachInterrupt(digitalPinToInterrupt(leftButtonPin), lplay, RISING);
  pinMode(rightButtonPin, INPUT_PULLDOWN);
  attachInterrupt(digitalPinToInterrupt(rightButtonPin), rplay, RISING);

  pinMode(switchPin,INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(switchPin), onOff, CHANGE);
}

void loop() {
  for(int i = 0; i < 10; i++) {
    CircuitPlayground.setPixelColor(i, 155, 155, 155);
  }
  randColor = random(2);
  if (randColor == 0){
    CircuitPlayground.setPixelColor(random(9), 0, 255, 0);
    delay(1000);
    CircuitPlayground.clearPixels();
  }
  else {
  CircuitPlayground.setPixelColor(random(9), 255, 0, 0);
  delay(1000);
  CircuitPlayground.clearPixels();
  }
}

void lplay() 
{
  lbuttonFlag = 1; 
}
void rplay() 
{
  rbuttonFlag = 1; 
}
void onOff()
{
  switchFlag = 1; 
}
