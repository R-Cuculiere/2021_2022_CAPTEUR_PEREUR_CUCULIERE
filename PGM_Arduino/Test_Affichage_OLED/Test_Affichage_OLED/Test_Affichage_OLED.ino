/*
  Fichier :       afficherTexteSsd1306.ino
  Description :   Affiche du texte de test, sur un écran OLED i2c de 128x64 pixels,
                  fonctionnant avec un contrôleur SSD1306
  Auteur :        Jérôme TOMSKI (https://passionelectronique.fr/)
  Créé le :       26.07.2021
*/

#include <Adafruit_SSD1306.h>


#define nombreDePixelsEnLargeur 128         // Taille de l'écran OLED, en pixel, au niveau de sa largeur
#define nombreDePixelsEnHauteur 64          // Taille de l'écran OLED, en pixel, au niveau de sa hauteur
#define brocheResetOLED         -1          // Reset de l'OLED partagé avec l'Arduino (d'où la valeur à -1, et non un numéro de pin)
#define adresseI2CecranOLED     0x3C        // Adresse de "mon" écran OLED sur le bus i2c (généralement égal à 0x3C ou 0x3D)

Adafruit_SSD1306 ecranOLED(nombreDePixelsEnLargeur, nombreDePixelsEnHauteur, &Wire, brocheResetOLED);

//Parameters
const int clkPin  = 3;
const int dtPin  = 4;
const int swPin  = 2;
//Variables
int rotVal  = 0;
bool clkState  = LOW;
bool clkLast  = HIGH;
bool swState  = HIGH;
bool swLast  = HIGH;
//

void setup() {

  // Initialisation de l'écran OLED
  if(!ecranOLED.begin(SSD1306_SWITCHCAPVCC, adresseI2CecranOLED))
    while(1);                               // Arrêt du programme (boucle infinie) si échec d'initialisation

    ecranOLED.clearDisplay();                                   // Effaçage de l'intégralité du buffer
    ecranOLED.setTextSize(2);                   // Taille des caractères (1:1, puis 2:1, puis 3:1)
    ecranOLED.setCursor(0, 0);                                  // Déplacement du curseur en position (0,0), c'est à dire dans l'angle supérieur gauche
    ecranOLED.setTextColor(SSD1306_WHITE);                  // Affichage du texte en "blanc" (avec la couleur principale, en fait, car l'écran monochrome peut être coloré)
    ecranOLED.display();
    
     //Init Serial USB
 Serial.begin(9600);
 Serial.println(F("Initialize System"));
 //Init Rotary
 pinMode(clkPin,INPUT);
 pinMode(dtPin,INPUT);
 pinMode(swPin,INPUT_PULLUP);

 attachInterrupt(digitalPinToInterrupt(clkPin),doEncoder,CHANGE);
 attachInterrupt(digitalPinToInterrupt(swPin),doSwitch,RISING);
    //delay(500);

 MenuGlobal();

}

void loop() {
   readRotary();
}

void readRotary( ) { /* function readRotary */
 ////Test routine for Rotary
 // gestion position
 clkState = digitalRead(clkPin);
 if ((clkLast == LOW) && (clkState == HIGH)) {//rotary moving
   Serial.print("Rotary position ");
   if (digitalRead(dtPin) == HIGH) {
     rotVal = rotVal - 1;
     if ( rotVal < 0 ) {
       rotVal = 0;
     }
   }
   else {
     rotVal++;
     if ( rotVal > 10 ) {
       rotVal = 10;
     }
   }
   Serial.println(rotVal);                                  // Déplacement du curseur en position (0,0), c'est à dire dans l'angle supérieur gauche
   //ecranOLED.setTextColor(SSD1306_WHITE); 
                  // Affichage du texte en "blanc" (avec la couleur principale, en fait, car l'écran monochrome peut être coloré)
    
   delay(200);
 }
 clkLast = clkState;
 //gestion bouton
 swState = digitalRead(swPin);
 if (swState == LOW && swLast == HIGH) {
   Serial.println("Rotary pressed");
   delay(100);//debounce
 }
 swLast = swState;
}

// Design menu OLED
void MenuGlobal(){
  ecranOLED.clearDisplay();
  ecranOLED.setCursor(0,0);
  ecranOLED.fillRect(0,0, ecranOLED.width()-0*2, ecranOLED.height()-49, SSD1306_INVERSE);
  ecranOLED.setTextColor(SSD1306_BLACK);
  ecranOLED.println("Resistance");
  ecranOLED.setTextColor(SSD1306_WHITE); 
  ecranOLED.println("Calibre");
  ecranOLED.println("Jeu");
  ecranOLED.println(rotVal);
  ecranOLED.display();
  delay(200);
}

void doEncoder(){

  Serial.println("doEncoder");
  ecranOLED.clearDisplay();
  ecranOLED.setCursor(0,0);
  ecranOLED.setTextColor(SSD1306_WHITE);
  ecranOLED.println("Resistance");
  ecranOLED.fillRect(0,1, ecranOLED.width()-0*2, ecranOLED.height()-49, SSD1306_INVERSE);
  ecranOLED.setTextColor(SSD1306_BLACK); 
  ecranOLED.println("Calibre");
  ecranOLED.setTextColor(SSD1306_WHITE);
  ecranOLED.println("Jeu");
  ecranOLED.println(rotVal);
  ecranOLED.display();
  delay(200);

  
}

void doSwitch(){

  Serial.println("doSwitch");
  ecranOLED.clearDisplay();
  ecranOLED.setCursor(0,0);
  ecranOLED.setTextColor(SSD1306_WHITE);
  ecranOLED.println("Resistance");
  ecranOLED.fillRect(0,1, ecranOLED.width()-0*2, ecranOLED.height()-49, SSD1306_INVERSE);
  ecranOLED.setTextColor(SSD1306_BLACK); 
  ecranOLED.println("Calibre");
  ecranOLED.setTextColor(SSD1306_WHITE);
  ecranOLED.println("Jeu");
  ecranOLED.println(rotVal);
  ecranOLED.display();
  delay(200);
}
