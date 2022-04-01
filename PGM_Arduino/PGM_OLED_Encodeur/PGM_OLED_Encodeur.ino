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

volatile unsigned int encoderPos = 1;
volatile unsigned int encoderPos1 = 1;

byte clk;
byte menuCount = 0;
byte dir = 0;
bool runState = false;

bool swState  = HIGH;
bool swLast  = HIGH;
bool menuType = HIGH;

// Déclaration variables debouncing
int lastDebounceTime = 0;
int debounceDelay = 50;

int i = 0 ;

void setup() {
  Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  display.display();
  display.clearDisplay();
  for(i;i<50;i++){
  display.drawLine(i*2, 1, 1000, 1000, WHITE);
  display.display();
  Serial.println(i);
  }
  display.clearDisplay();

  // Initialisation encodeur
  pinMode(pinClk,INPUT);
  pinMode(pinData,INPUT);
  pinMode(pinSw,INPUT_PULLUP);
 
  attachInterrupt(digitalPinToInterrupt(pinClk), doEncoder, CHANGE);  // encoder pin on interrupt 0 - pin 3
}

void loop() {
  
  
  
  Sw();
  menuOled();


  display.clearDisplay();
  
  delay(50);
}


// Test Switch

void Sw(){

  swState = digitalRead(pinSw);
  if (swState == LOW && swLast == HIGH) {
    Serial.print("Y");
    if(menuType == HIGH){
      menuType = LOW;
      Serial.println(LOW);
      }
    else{
      menuType = HIGH;
      Serial.println(HIGH);
      }
  }
  swLast = swState;
  delay(100);
}


// menuOled : Choix du menu

void menuOled(){

  if(menuType==HIGH){staticMenu();}
  if(menuType==LOW && encoderPos==1){menuA();}
  if(menuType==LOW && encoderPos==3){menuC();}
  
}

// staticMenu : menu de démarrage

void staticMenu() {
  display.setTextSize(1);
  display.setTextColor(WHITE);

  //---------------------------------
  display.setTextSize(1);
  display.setCursor(10, 0);
  display.println("CALIBRE");
  display.setCursor(60, 0);

  display.setCursor(10, 10);
  display.println("RESISTANCE");
  display.setCursor(60, 10);

  display.setCursor(10, 20);
  display.println("CREDIT");
  display.setCursor(60, 20);

  Serial.println(encoderPos);
  
  display.setCursor(2, (encoderPos * 10)- 10);
  display.println(">");

  display.display();

  display.clearDisplay();
}

// menuA : premier menu - Calibration

void menuA(){
  
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(10, 0);
  display.println("CALIBRE");
  display.setCursor(60, 0);

  display.setCursor(10, 20);
  display.println("Click to go back");

  display.display();
  
  display.clearDisplay();
}

// menuC : Crédit du projet


void menuC(){
  
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(10, 0);
  display.println("CREDIT");
  display.setCursor(60, 0);

  display.setCursor(10, 10);
  display.println("CucLuc ou Luccuc");

  display.setCursor(10, 20);
  display.println("Click to go back");

  display.display();
  
  display.clearDisplay();
}


// Interruption incrémentation de l'encodeur

void doEncoder(){

  if ((millis() - lastDebounceTime) > debounceDelay)
  {
  
  if (digitalRead(pinClk)==digitalRead(pinData)){
    if(menuType==HIGH){
      if(encoderPos > 1){
       encoderPos --; 
      }
      else{
        encoderPos == 3;
      }
    }
    if(menuType==LOW){
      if(encoderPos1 > 1){
       encoderPos1 --; 
      }
      else{
      encoderPos1 == 3;
      }
    }
  }
  if (digitalRead(pinClk)!=digitalRead(pinData)){
    if(menuType==HIGH){
      if(encoderPos < 3){
       encoderPos ++; 
      }
      else{
        encoderPos == 1;
      }
    }
    if(menuType==LOW){
      if(encoderPos1 < 3){
       encoderPos1 ++; 
      }
      else{
      encoderPos1 == 1;
      }
    }
  }
  lastDebounceTime = millis();
  }
}
