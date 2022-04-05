#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
#include <SoftwareSerial.h>
#include <SPI.h>

// Définintions OLED

#define nombreDePixelsEnLargeur 128         // Taille de l'écran OLED, en pixel, au niveau de sa largeur
#define nombreDePixelsEnHauteur 64          // Taille de l'écran OLED, en pixel, au niveau de sa hauteur
#define brocheResetOLED         -1          // Reset de l'OLED partagé avec l'Arduino (d'où la valeur à -1, et non un numéro de pin)
#define adresseI2CecranOLED     0x3C        // Adresse de "mon" écran OLED sur le bus i2c (généralement égal à 0x3C ou 0x3D)

Adafruit_SSD1306 OLED(nombreDePixelsEnLargeur, nombreDePixelsEnHauteur, &Wire, brocheResetOLED);

// Définitions encodeur rotatoire

#define pinClk  3
#define pinData  4
#define pinSw 2

volatile unsigned int encoderPos = 1;
volatile unsigned int encoderPos1 = 5; //position par défaut de la sélection des sous-menu

byte clk;
byte dir = 0;
bool runState = false;

bool swState  = HIGH;
bool swLast  = HIGH;
bool menuType = HIGH;


// Définitions Module bluetooth

#define rxPin 5 //Broche 5 en tant que RX, � raccorder sur TX du HC-05
#define txPin 6 //Broche 6 en tant que RX, � raccorder sur TX du HC-05
#define baudrate 9600
SoftwareSerial mySerial(rxPin ,txPin); //D�finition du software serial

// Déclarations variables debouncing

int lastDebounceTime = 0;
int debounceDelay = 150;


float Vadc = 0.0;
float R3 = 100000.0;
float R1 = 100000.0;
float R2 ;
float R5 = 10000.0;
float Vcc = 5.0;
float Rcapteur = 0.0;

// Déclarations Calibration

float flatR = 0.0 ;
float upR = 0.0 ;
float downR = 0.0 ;
bool menuAactive = LOW ;

// SPI def

const byte csPin           = 10;      // MCP42100 chip select pin
const int  maxPositions    = 256;     // wiper can move from 0 to 255 = 256 positions
const long rAB             = 46200;   // 100k pot resistance between terminals A and B, 
                                      // mais pour ajuster au multimètre, je mets 92500
const byte rWiper          = 125;     // 125 ohms pot wiper resistance
const byte pot0            = 0x11;    // pot0 addr // B 0001 0001
const byte pot0Shutdown    = 0x21;    // pot0 shutdown // B 0010 0001

void setPotWiper(int addr, int pos) {
  pos = constrain(pos, 0, 255);            // limit wiper setting to range of 0 to 255
  digitalWrite(csPin, LOW);                // select chip
  SPI.transfer(addr);                      // configure target pot with wiper position
  SPI.transfer(pos);
  digitalWrite(csPin, HIGH);               // de-select chip

  // print pot resistance between wiper and B terminal
  long resistanceWB = ((rAB * pos) / maxPositions ) + rWiper;
//  Serial.print("Wiper position: ");
//  Serial.print(pos);
//  Serial.print(" Resistance wiper to B terminal: ");
//  Serial.print(resistanceWB);
//  Serial.println(" ohms");
  R2 = resistanceWB;
}


/****************************SETUP********************************/

void setup() {
  
  // Initialisation de l'écran OLED
  if(!OLED.begin(SSD1306_SWITCHCAPVCC, adresseI2CecranOLED))
    while(1);                               // Arrêt du programme (boucle infinie) si échec d'initialisation
  boolean bCouleurInverse = false;
  OLED.clearDisplay();
  OLED.setTextColor(WHITE);

  // SETUP Encodeur

  pinMode(pinClk,INPUT);
  pinMode(pinData,INPUT);
  pinMode(pinSw,INPUT_PULLUP);

  attachInterrupt(digitalPinToInterrupt(pinClk), doEncoder, CHANGE);  // encoder pin on interrupt 1 - pin 3
  attachInterrupt(digitalPinToInterrupt(pinSw), doSwitch, CHANGE);

  // setup module bluetooth

  pinMode(rxPin,INPUT);
  pinMode(txPin,OUTPUT);
    
  mySerial.begin(baudrate);
  
  // ouverture port serie
  Serial.begin(38400);

  // Initialisation SPI

  digitalWrite(csPin, HIGH);        // chip select default to de-selected
  pinMode(csPin, OUTPUT);           // configure chip select as output
  SPI.begin();
  
}


/****************************LOOP********************************/

void loop() {
  
  // Mesure et calcul de la résistance
  
  setPotWiper(pot0, 3);
  Vadc = (Vcc*analogRead(A0))/1023.0;
  Rcapteur = (1+R3/R2)*R1*(Vcc/Vadc)-R1-R5;

  //OLED

  calibration();
  menuOLED();
  OLED.clearDisplay();

  //test module bluetooth
  mySerial.print(Rcapteur);

  //TESTS



  delay(50);
}


/*******calibration*******/

void calibration(){

  if(menuType == HIGH && encoderPos1 == 3){
    OLED.setTextSize(1);
    //OLED.setTextColor(WHITE);
    OLED.setCursor(10, 0);
    OLED.println("Mettez le capteur vers le bas");

    OLED.setCursor(10, 20);
    OLED.println(Rcapteur/1000000.0);
    
    OLED.setCursor(10, 40);
    OLED.println("Cliquez pour continuer");

    downR = Rcapteur/1000000.0;

    OLED.display();
    OLED.clearDisplay();
  }
  if(menuType == HIGH && encoderPos1 == 2){
    OLED.setTextSize(1);
    //OLED.setTextColor(WHITE);
    OLED.setCursor(10, 0);
    OLED.println("Mettez le capteur vers le haut");

    OLED.setCursor(10, 20);
    OLED.println(Rcapteur/1000000.0);
    
    OLED.setCursor(10, 40);
    OLED.println("Cliquez pour continuer");

    upR = Rcapteur/1000000.0;

    OLED.display();
    OLED.clearDisplay();
  }
  
}


/***************MenuDef***************/

/*******menuOLED*******/

void menuOLED(){

  if(menuType==HIGH && encoderPos1==5){
    staticMenu();
    menuAactive = LOW;
    }
  if(menuType==LOW && encoderPos==1){
    menuA();
    menuAactive = HIGH;
    }
  if(menuType==LOW && encoderPos==2){menuB();}
  if(menuType==LOW && encoderPos==3){menuC();}
  
}

/*******staticMenu*******/
// Menu de démarrage

void staticMenu() {
  OLED.setTextSize(1);
  //OLED.setTextColor(WHITE);

  //---------------------------------
  OLED.setTextSize(1);
  OLED.setCursor(10, 0);
  OLED.println("CALIBRE");

  OLED.setCursor(10, 10);
  OLED.println("RESISTANCE");

  OLED.setCursor(10, 20);
  OLED.println("CREDIT");

  // Curseur de selection
  OLED.setCursor(2, (encoderPos * 10)- 10);
  OLED.println(">");

  OLED.display();

  OLED.clearDisplay();
}

/*******menuA*******/
// Menu de calibration (à programmer)

void menuA(){
  
  
  OLED.setTextSize(1);
  OLED.setCursor(10, 0);
  OLED.println("CALIBRE");

  OLED.setCursor(10, 10);
  OLED.println("MAX");
  OLED.setCursor(60, 10);
  OLED.println(upR);
  

  OLED.setCursor(10, 20);
  OLED.println("MIN");
  OLED.setCursor(60, 20);
  OLED.println(downR);


  OLED.setCursor(10, 40);
  OLED.println("Click to go back");

  // Curseur de selection
  OLED.setCursor(2, (encoderPos1 * 10)- 10);
  OLED.println(">");

  OLED.display();

  delay(50);
  
  //OLED.clearDisplay();
}

/*******menuB*******/

void menuB(){
  OLED.setCursor(0,0);
  OLED.print("Resistance : ");
  OLED.print(Rcapteur/1000000.0);
  OLED.println(" MOhm");
  float angle = (((Rcapteur/1000000.0)-upR)/(downR-upR))*180;
  OLED.print("angle : ");
  OLED.println(angle);
  
  OLED.setCursor(10, 40);
  OLED.println("Click to go back");

  // Curseur de selection
  OLED.setCursor(2, (encoderPos1 * 10)- 10);
  OLED.println(">");
  
  OLED.display();
  OLED.clearDisplay();
}  

/*******menuC*******/
// Crédit du projet

void menuC(){
  
  OLED.setTextSize(1);
  //OLED.setTextColor(WHITE);
  OLED.setCursor(10, 0);
  OLED.println("CREDIT");

  OLED.setCursor(10, 10);
  OLED.println("CucLuc ou Luccuc");

  OLED.setCursor(10, 40);
  OLED.println("Click to go back");

  // Curseur de selection
  OLED.setCursor(2, (encoderPos1 * 10)- 10);
  OLED.println(">");

  OLED.display();
  
  OLED.clearDisplay();
}



/*******doSwitch*******/

void doSwitch(){

  swState = digitalRead(pinSw);
  if (swState == LOW && swLast == HIGH) {
    
    if(menuType == HIGH){
      menuType = LOW;
      }
    else if(menuType == LOW && encoderPos1 == 5 || menuType == LOW && menuAactive ){
      menuType = HIGH;
      }
  }
  swLast = swState;
  
  delay(100);
}


/*******doEncoder*******/

void doEncoder(){

  if ((millis() - lastDebounceTime) > debounceDelay)
  {
  
  if (digitalRead(pinClk)==digitalRead(pinData)){
    if(menuType==HIGH){
      if(encoderPos > 1){
       encoderPos --; 
      }
      else{
        encoderPos == 5;
      }
    }
    if(menuType==LOW){
      if(encoderPos1 > 1){
       encoderPos1 --; 
      }
      else{
      encoderPos1 == 5;
      }
    }
  }
  if (digitalRead(pinClk)!=digitalRead(pinData)){
    if(menuType==HIGH){
      if(encoderPos < 5){
       encoderPos ++; 
      }
      else{
        encoderPos == 1;
      }
    }
    if(menuType==LOW){
      if(encoderPos1 < 5){
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
