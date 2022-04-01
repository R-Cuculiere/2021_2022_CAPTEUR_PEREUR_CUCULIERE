
#include <Adafruit_SSD1306.h>

#define nombreDePixelsEnLargeur 128         // Taille de l'écran OLED, en pixel, au niveau de sa largeur
#define nombreDePixelsEnHauteur 64          // Taille de l'écran OLED, en pixel, au niveau de sa hauteur
#define brocheResetOLED         -1          // Reset de l'OLED partagé avec l'Arduino (d'où la valeur à -1, et non un numéro de pin)
#define adresseI2CecranOLED     0x3C        // Adresse de "mon" écran OLED sur le bus i2c (généralement égal à 0x3C ou 0x3D)

Adafruit_SSD1306 ecranOLED(nombreDePixelsEnLargeur, nombreDePixelsEnHauteur, &Wire, brocheResetOLED);

float R = 0.0;
void setup() {
  
  // Initialisation de l'écran OLED
  if(!ecranOLED.begin(SSD1306_SWITCHCAPVCC, adresseI2CecranOLED))
    while(1);                               // Arrêt du programme (boucle infinie) si échec d'initialisation
  boolean bCouleurInverse = false;
  ecranOLED.clearDisplay();
  ecranOLED.setTextSize(2);
  ecranOLED.setCursor(0,0);
  ecranOLED.setTextColor(SSD1306_WHITE);
  // ouverture port serie
  Serial.begin(9600);
}

void loop() {
  // Mesure et calcul de la résistance
R = (5.0*analogRead(A1))/1024.0;
Serial.println(R);
ecranOLED.clearDisplay();
ecranOLED.setCursor(0,0);
ecranOLED.print("Resistance : ");
ecranOLED.println(R);
ecranOLED.display();   
delay(1000);


}
