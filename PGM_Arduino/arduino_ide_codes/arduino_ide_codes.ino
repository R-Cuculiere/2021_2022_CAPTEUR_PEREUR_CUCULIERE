#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#define OLED_ADDR   0x3C
Adafruit_SSD1306 display(-1);

//#if (SSD1306_LCDHEIGHT != 64)
//#error("Height incorrect, please fix Adafruit_SSD1306.h!");
//#endif
//
//#if (SSD1306_LCDHEIGHT != 64)
//#error("Height incorrect, please fix Adafruit_SSD1306.h!");
//#endif

#define pinClk  3
#define pinData  4
#define pinSw 2

volatile unsigned int encoder0Pos = 0;

int valA;
int valB;
int valC;

byte clk;
byte menuCount = 0;
byte dir = 0;
bool runState = false;

bool swState  = HIGH;
bool swLast  = HIGH;

bool quit = LOW ; 

void setup() {
  Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  display.display();
  display.clearDisplay();
  attachInterrupt(digitalPinToInterrupt(pinClk), doEncoder, CHANGE);  // encoder pin on interrupt 0 - pin 3
}

void loop() {
  clk = digitalRead(pinClk);

  menuCheck();
  //gestion bouton
  swState = digitalRead(pinSw);
  if (swState == LOW && swLast == HIGH) {
    // mettre un while
    Serial.println("Rotary pressed");
    quit = LOW ;

    if (menuCount == 0 && swState == LOW){
      staticMenu();
    }

    if (menuCount == 1 && swState == LOW){
      menuA();
    }
    
    //delay(100);//debounce
    }
  swLast = swState;

  
  menuCheck();
  Serial.println(menuCount);
  staticMenu();
  display.clearDisplay();
  //delay(50);
}


void staticMenu() {
  display.setTextSize(1);
  display.setTextColor(WHITE);

  //---------------------------------
  display.setTextSize(1);
  display.setCursor(10, 0);
  display.println("Value A:");
  display.setCursor(60, 0);
  display.println(valA);

  display.setCursor(10, 10);
  display.println("Value B:");
  display.setCursor(60, 10);
  display.println(valB);

  display.setCursor(10, 20);
  display.println("Value C:");
  display.setCursor(60, 20);
  display.println(valC);

  display.setCursor(10, 30);
  display.println("Start:");
  display.setCursor(45, 30);
  if (encoder0Pos > 5 && menuCount == 4) {
    display.println("Run!");
    runState = true;
  } else {
    display.println("Idle");
    runState = false;
  }
  display.setCursor(2, (menuCount * 10)- 10);
  display.println(">");

  display.display();
}



void menuCheck() {
  if (clk == LOW && menuCount < 5) {
    menuCount++;
    encoder0Pos = 0;
  }
  if (clk == LOW && menuCount >= 5) {
    menuCount = 1;
  }
}


/*           /
/   Menu A   /
/           */

void menuA(){

  
  Serial.println(menuCount);
  
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(10, 0);
  display.println("CALIBRE");
  display.setCursor(60, 0);
  display.println(valA);

  display.setCursor(10, 30);
  display.println("Back");


  if (encoder0Pos > 5 && menuCount == 4) {
    display.println("Run!");
    runState = true;
  } else {
    display.println("Idle");
    runState = false;
  }
  display.setCursor(2, (menuCount * 10)- 10);
  display.println(">");

  display.display();

  swState = digitalRead(pinSw);
  if (swState == LOW && swLast == HIGH && menuCount == 3) {
    Serial.println("Pressed");
    quit = HIGH;
  }
  swLast = swState;
  
  display.clearDisplay();
  
}


// Tests

int buttonState; // the current reading from the input pin
int lastButtonState = LOW; // the previous reading from the input pin
// the following variables are long's because the time, measured in miliseconds, will quickly become a bigger number than can be stored in an int.
long lastDebounceTime = 0; // the last time the output pin was toggled
long debounceDelay = 50;

void doEncoder() {

  if (digitalRead(pinClk) == HIGH) {
    lastDebounceTime = millis();

//    if ((millis() - lastDebounceTime) > debounceDelay)
//  {
    if (digitalRead(pinData) == LOW && encoder0Pos > 0) {
      encoder0Pos = encoder0Pos - 1;
      dir = 0;
    }
    else {
      encoder0Pos = encoder0Pos + 1;
      dir = 1;
    }
  }
//  }
  else
  {
//    if ((millis() - lastDebounceTime) > debounceDelay)
//    {
    if (digitalRead(pinData) == LOW ) {
      encoder0Pos = encoder0Pos + 1;
      dir = 1;
    }
    else {
      if (encoder0Pos > 0) {
        encoder0Pos = encoder0Pos - 1;
        dir = 0;
      }
    }
    }
  //}
}
