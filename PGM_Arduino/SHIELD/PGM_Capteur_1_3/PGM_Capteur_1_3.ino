
#include <Adafruit_SSD1306.h>

#define nombreDePixelsEnLargeur 128         // Taille de l'écran OLED, en pixel, au niveau de sa largeur
#define nombreDePixelsEnHauteur 64          // Taille de l'écran OLED, en pixel, au niveau de sa hauteur
#define brocheResetOLED         -1          // Reset de l'OLED partagé avec l'Arduino (d'où la valeur à -1, et non un numéro de pin)
#define adresseI2CecranOLED     0x3C        // Adresse de "mon" écran OLED sur le bus i2c (généralement égal à 0x3C ou 0x3D)

Adafruit_SSD1306 ecranOLED(nombreDePixelsEnLargeur, nombreDePixelsEnHauteur, &Wire, brocheResetOLED);

// Définition Module bluetooth

#include <SoftwareSerial.h>
#define rxPin 5 //Broche 5 en tant que RX, � raccorder sur TX du HC-05
#define txPin 6 //Broche 6 en tant que RX, � raccorder sur TX du HC-05
#define baudrate 9600
SoftwareSerial mySerial(rxPin ,txPin); //D�finition du software serial


float Vadc = 0.0;
float R3 = 100000.0;
float R2 = 1000.0;
float R1 = 100000.0;
float R5 = 10000.0;
float Vcc = 5.0;
float Rcapteur = 0.0;

void setup() {
  
  // Initialisation de l'écran OLED
  if(!ecranOLED.begin(SSD1306_SWITCHCAPVCC, adresseI2CecranOLED))
    while(1);                               // Arrêt du programme (boucle infinie) si échec d'initialisation
  boolean bCouleurInverse = false;
  ecranOLED.clearDisplay();
  ecranOLED.setTextSize(2);
  ecranOLED.setCursor(0,0);
  ecranOLED.setTextColor(SSD1306_WHITE);

  // setup module bluetooth

   pinMode(rxPin,INPUT);
  pinMode(txPin,OUTPUT);
    
  mySerial.begin(baudrate);
  
  // ouverture port serie
  Serial.begin(19200);
}

void loop() {
  // Mesure et calcul de la résistance
Vadc = (Vcc*analogRead(A0))/1024.0;
Rcapteur = (1+R3/R2)*R1*(Vcc/Vadc)-R1-R5;
Serial.println(Rcapteur);

  //OLED
ecranOLED.clearDisplay();
ecranOLED.setCursor(0,0);
ecranOLED.print("Resistance : ");
ecranOLED.print(Rcapteur/1000000.0);
ecranOLED.println(" MOhm");
ecranOLED.display();   
delay(100);

  //test module bluetooth
     mySerial.print(Rcapteur); 

}
