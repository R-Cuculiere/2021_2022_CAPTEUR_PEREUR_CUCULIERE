/* PotSend
 *  Send analog values from pots or similar over the serial port
 *  Also see:  http://www.arduino.cc/en/Tutorial/Graph
 */

int potPin = 0;

void setup() {
  Serial.begin(19200);
}

void loop() {
  int val = analogRead(potPin);
  val = val/4;
//  Serial.write(val, BYTE);
  Serial.write(val);
  delay(100);
}

