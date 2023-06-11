#include <Adafruit_CircuitPlayground.h>
#include <AsyncDelay.h>

const int leftButtonPin = 4;
const int rightButtonPin = 5;
volatile bool lbuttonFlag = 0;
volatile bool rbuttonFlag = 0;
const int switchPin = 7; 
volatile bool switchFlag = 0; 
bool switchVal = 0;
int level = 1; 
int score = 0; 
int sequence[30]; 
int levelLength = 1; 
int sequenceArray = 0; 
float midi[127];
int A_four = 440;
int gameOverSong[4][2] = {
  {80, 200},
  {79, 200},
  {78, 200},
  {77, 600},
};
int levelCompleteSong[4][2] = {
  {90, 120},
  {91, 120},
  {92, 120},
  {93, 200},
};

void setup() {
  CircuitPlayground.begin();
  randomSeed(analogRead(0));
  generateMIDI();

  pinMode(leftButtonPin, INPUT_PULLDOWN);
  attachInterrupt(digitalPinToInterrupt(leftButtonPin), lplay, RISING);
  pinMode(rightButtonPin, INPUT_PULLDOWN);
  attachInterrupt(digitalPinToInterrupt(rightButtonPin), rplay, RISING);
  pinMode(switchPin,INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(switchPin), onOff, CHANGE);

  levelLength = 1;
  sequenceArray = 0;
  generateSequence(); 
  playSequence(); 
}

void loop() {
  if (switchFlag) {
    delay(1);
    switchVal = digitalRead(switchPin);
    CircuitPlayground.clearPixels();
    levelLength = 1;
    sequenceArray = 0;
    switchFlag = 0; 
  }
  if (!switchVal) {
    if (sequenceArray < levelLength) {
        if (lbuttonFlag || rbuttonFlag) {
          int color = sequence[sequenceArray];

          if ((lbuttonFlag && color == 0) || (rbuttonFlag && color == 1)) {
            sequenceArray++;
            score++;
            CircuitPlayground.playTone(500, 100);

            if (sequenceArray == levelLength) {
              level++;
              levelLength++;
              sequenceArray = 0;
              Serial.print("Level ");
              Serial.println(level);
              Serial.print("Your Score is ");
              Serial.println(score);
              delay(300);
              for (int i = 0; i < sizeof(levelCompleteSong) / sizeof(levelCompleteSong[0]); i++) {
                CircuitPlayground.playTone(midi[levelCompleteSong[i][0]], levelCompleteSong[i][1]);
                delay(1);
                }
              CircuitPlayground.clearPixels();
              generateSequence(); 
              delay(1000); 
              CircuitPlayground.clearPixels(); 
              playSequence(); 
            } 
          } 
          else {
              CircuitPlayground.clearPixels();
              Serial.println("GAME OVER!");
              for (int i = 0; i < sizeof(gameOverSong) / sizeof(gameOverSong[0]); i++) {
                CircuitPlayground.playTone(midi[gameOverSong[i][0]], gameOverSong[i][1]);
                delay(1);
              }
              delay(1000);
              CircuitPlayground.clearPixels();
              levelLength = 1;
              sequenceArray = 0;
              score = 0;
              level = 1; 
              }
          lbuttonFlag = 0;
          rbuttonFlag = 0;
        }
      }
  }
}

void generateSequence() {
  for (int i = 0; i < levelLength; i++) {
    sequence[i] = random(2); 
  }
}

void playSequence() {
  for (int i = 0; i < levelLength; i++) {
    int color = sequence[i];
  if (color == 0) {
    CircuitPlayground.setPixelColor(random(10), 0, 255, 0); 
  } 
  else {
    CircuitPlayground.setPixelColor(random(10), 255, 0, 0); 
  } 
    delay(1000);
    CircuitPlayground.clearPixels();
    delay(500); 
  }
}

void generateMIDI() {
  for (int x = 0; x < 127; ++x) {
    midi[x] = (A_four / 32.0) * pow(2.0, ((x - 9.0) / 12.0));
    Serial.println(midi[x]);
  }
}

void lplay() {
  lbuttonFlag = 1;
}

void rplay() {
  rbuttonFlag = 1;
}

void onOff()
{
  switchFlag = 1; 
}