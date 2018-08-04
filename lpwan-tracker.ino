#include <SigFox.h>
#include <SoftwareSerial.h>

String version;
String ID;
String PAC;

void setup() {
  Serial.begin(9600);
  delay(500);

  while (!Serial) {};

  if (!SigFox.begin()) {
    Serial.println("Shield error or not present!");
    return;
  }

  // Send the module to the deepest sleep
  SigFox.end();

  // Activate debug mode
  SigFox.debug();
}

void loop() {
  // Start Sigfox module
  SigFox.begin();

  delay(100);

  int8_t temperature = (int) SigFox.internalTemperature();

  Serial.print("Temperature: ");
  Serial.println(temperature);

  // Clean all pending interruptions
  SigFox.status();
  delay(1);

  SigFox.beginPacket();
  SigFox.write(temperature);
  SigFox.print("W");
  int result = SigFox.endPacket();
  
  // Check result
  if (result == 0) {
    Serial.println("Message Sent !!!");
  } else {
    Serial.println("Error sending message: " + result);
  }

  // Send the module to the deepest sleep
  SigFox.end();

  // Wait for 15 minutes until next message
  delay(900000);
}

