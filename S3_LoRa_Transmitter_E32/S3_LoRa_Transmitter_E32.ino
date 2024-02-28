/*
** First select which module you are working with, either e32 or e22
** Remove the "//" comment mark next to the module you are using and comment out the other module 
** https://store.fixaj.com/
*/

#include "LoRa_E32.h"
#define library LoRa_E32

#include <HardwareSerial.h>
#define M0 4                    // Pin 7 on our 3in1 PCB
#define M1 6                    // Pin 6 on our 3in1 PCB
#define RX 18                   // Lora RX connected to esp32 s3
#define TX 17                   // Lora TX connected to esp32 s3

HardwareSerial fixSerial(1);
library FixajSerial(TX, RX, &fixSerial, UART_BPS_RATE_9600);


#define Address 1              // You can enter a value between 0 and 65000. Should be DIFFERENT from other modules.
#define Channel 20             // Frequency value should be the SAME as other modules.
#define DestinationAddress 2  // Address of the LoRa to which the message will be sent

struct Signal {
  char password[15] = "Fixaj.com";
  char location[15];
  bool button1;
  byte Coor[10];
  byte Number[4];
} data;


void setup() {
  pinMode(M0, OUTPUT);
  pinMode(M1, OUTPUT);

  Serial.begin(115200);
  while (!Serial) { ; }
  delay(200);
  FixajSerial.begin();

  LoraE32Settings();
  Serial.print("E32 433t20d ");

  digitalWrite(M0, LOW);
  digitalWrite(M1, LOW);

  delay(500);
  Serial.println("Starting with... Fixaj.com");
}

void loop() {

  data.button1 = false;
  *(float*)(data.Coor) = 37.199386;
  *(int*)(data.Number) = 1234;
  strcpy(data.location, "Teknofest");

  ResponseStatus rs = FixajSerial.sendFixedMessage(highByte(DestinationAddress), lowByte(DestinationAddress), Channel, &data, sizeof(Signal));
  Serial.print(rs.getResponseDescription());
  Serial.println(rs.code);
  delay(2000);
}


void LoraE32Settings() {
  digitalWrite(M0, HIGH);
  digitalWrite(M1, HIGH);

  ResponseStructContainer c;
  c = FixajSerial.getConfiguration();
  Configuration configuration = *(Configuration*)c.data;

  //CONFIGURABLE SETTINGS
  // Adjust in the #define section above
  configuration.ADDL = lowByte(Address);
  configuration.ADDH = highByte(Address);
  configuration.CHAN = Channel;

  //OPTIONAL SETTINGS
  configuration.SPED.airDataRate = AIR_DATA_RATE_010_24;  //Data Rate 2.4 Default
  //configuration.SPED.airDataRate = AIR_DATA_RATE_000_03;  //Data Rate 0.3 Furthest Distance
  //configuration.SPED.airDataRate = AIR_DATA_RATE_101_192; //Data Rate 19.2 Fastest

  //configuration.OPTION.transmissionPower = POWER_20;  //Transmission Power max Default
  configuration.OPTION.transmissionPower = POWER_10;  //Transmission Power min
  //configuration.OPTION.transmissionPower = POWER_30; // Enable this for E32 30d modules

  //ADVANCED SETTINGS
  configuration.SPED.uartBaudRate = UART_BPS_9600;
  configuration.SPED.uartParity = MODE_00_8N1;
  configuration.OPTION.fec = FEC_0_OFF;
  //configuration.OPTION.fec = FEC_1_ON;
  configuration.OPTION.fixedTransmission = FT_FIXED_TRANSMISSION;
  //configuration.OPTION.fixedTransmission = FT_TRANSPARENT_TRANSMISSION;
  configuration.OPTION.wirelessWakeupTime = WAKE_UP_250;
  configuration.OPTION.ioDriveMode = IO_D_MODE_PUSH_PULLS_PULL_UPS;

  // SAVE and STORE Settings
  ResponseStatus rs = FixajSerial.setConfiguration(configuration, WRITE_CFG_PWR_DWN_SAVE);
  Serial.println(rs.getResponseDescription());
  Serial.println(rs.code);
  c.close();
}