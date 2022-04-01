
#define encoder0PinA 2
#define encoder0PinB 4
#define Switch 5

volatile unsigned int encoder0Pos = 0;

void setup() {
pinMode(encoder0PinA, INPUT);
digitalWrite(encoder0PinA, HIGH);

pinMode(encoder0PinB, INPUT);
digitalWrite(encoder0PinB, HIGH);

attachInterrupt (0, doEncoder, RISING);

Serial.begin(9600);
Serial.println("start");

}

void loop() {

Serial.print("Position:");
Serial.println (encoder0Pos, DEC);
};

void doEncoder(){
  if (digitalRead(encoder0PinB)==HIGH){
    encoder0Pos++;
  }
  else {
    encoder0Pos--;
  }
}
