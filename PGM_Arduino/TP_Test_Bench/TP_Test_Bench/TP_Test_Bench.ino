#include <Servo.h>

#define PIN_SERVO 11
#define PIN_FLEX_SENSOR A0

Servo myservo;

int pos = 0;
int pos_max = 150;
int pos_min = 20;
const float VCC = 5;      // voltage at Ardunio 5V line
const float R_DIV = 47000.0;  // resistor used to create a voltage divider
const float flatResistance = 25000.0; // resistance when flat
const float bendResistance = 100000.0;  // resistance at 90 deg
 
void setup() {
  Serial.begin(9600);  
  myservo.attach(PIN_SERVO);
}

void measure(){
  Serial.println(analogRead(PIN_FLEX_SENSOR));
  delay(50); 
}

void loop() {

  int ADCflex = analogRead(PIN_FLEX_SENSOR);
  float Vflex = ADCflex * VCC / 1023.0;
  float Rflex = R_DIV * (VCC / Vflex - 1.0);
  int angle = int(map(Rflex, flatResistance, bendResistance, 0, 90.0));
  
    myservo.write(5*angle);          
    measure();
 
  
}
