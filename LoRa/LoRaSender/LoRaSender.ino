#include <SPI.h>
#include <LoRa.h>
#include <EEPROM.h>

int counter = 0;
int number_messages = 0;
int message_size = 0;
String message = "";
int jitter = 0;

void setup() {
  Serial.begin(9600);
  while (!Serial);
//  Serial.println("LoRa Sender");
  if (!LoRa.begin(433E6)) {
    Serial.println("Arranque de LoRa fallido!");
    while (1);
  }

  // register the receive callback
  LoRa.onReceive(onReceive);

  // put the radio into receive mode
  LoRa.receive();
}

void loop() {
  String serialResponse;
  int data[10];
  int i = 0;

  if(Serial.available()){
    serialResponse = Serial.readStringUntil('\r\n');
//    /Serial.println(serialResponse);

    
    char buf[10];
    serialResponse.toCharArray(buf, sizeof(buf));
    char *p = buf;
    char *str;
    while ((str = strtok_r(p, ";", &p)) != NULL) // delimiter is the semicolon
    {
      data[i] = atoi(str);
      i++;
    }
  
    number_messages = data[0];
    message_size = data[1];
    message = "";
    
    for (int i = 0; i < message_size ; i++) {
      message += "x";
    }
    counter = 0;

//    Serial.print("number_messages ");
//    Serial.println(number_messages);
//    Serial.print("message_size ");
//    Serial.println(message_size);
//    Serial.print("message ");
//    Serial.println(message);
  }
 
  if (counter < number_messages && number_messages > 0) {
//      Serial.print("Sending packet ");
//      Serial.print(counter);
//      Serial.print(" at: ");
      int millis_sending = millis();
//      Serial.println(millis_sending);

      LoRa.beginPacket();
      LoRa.print(message);
      LoRa.endPacket();
      LoRa.receive();

      EEPROM.put(0, millis_sending);
  
      if(counter == number_messages){
        counter = 0;
        number_messages = 0;

        
      }

      counter++;
      delay(3500);
      
   }
   
}

void onReceive(int packetSize) { 
//  Serial.print("Received packet at: ");
  int millis_received = millis(); 
//  Serial.println(millis_received);
 
  int millis_sending;
  EEPROM.get(0,millis_sending);
  int travel_time = millis_received - millis_sending;
//  /Serial.print("Time travel: ");
  Serial.print(travel_time);

//  /EEPROM.put(counter, travel_time);
//  /Serial.print(EEPROM.read(counter));

}
 
 

 
