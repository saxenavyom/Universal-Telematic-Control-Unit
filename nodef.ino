#include "UbidotsESPMQTT.h"
#include <SPI.h>
#include <SD.h>
/****************************************
 * Define Constants
 ****************************************/
 double v1, v2, v3, v4, v5, v6, v7, v8, v9; 
#define TOKEN "BBUS-Sg4IM6WeTemBZfUQo0SvKiaFYvcF0v" // Your Ubidots TOKEN
#define WIFINAME "realme 7" //Your SSID
#define WIFIPASS "honormob" // Your Wifi Pass
const int chipSelect = 15;
Ubidots client(TOKEN);

/****************************************
 * Auxiliar Functions
 ****************************************/

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i=0;i<length;i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}

/****************************************
 * Main Functions
 ****************************************/

void setup() {
  // put your setup code here, to run once:
  //client.ubidotsSetBroker("industrial.api.ubidots.com"); // Sets the broker properly for the industrial account
  client.setDebug(true); // Pass a true or false bool value to activate debug messages
  Serial.begin(9600);
  client.wifiConnection(WIFINAME, WIFIPASS);
  client.begin(callback);
  if (!SD.begin(chipSelect))
  {
    Serial.println("SD card initialization failed!");
    return;
  }
  Serial.println("SD card initialized.");

  // Check if data.csv file exists, if not, create it
  if (!SD.exists("data.csv"))
  {
    createDataFile();
  }
 
  delay(2000);
  

  }

void loop() {
  
  if (WiFi.status() == WL_CONNECTED)
  {
    // If WiFi is connected, do nothing
  }
  else
  {
    // If WiFi is not connected, save data to SD card
    saveDataToSDCard();
    delay(1000); // Wait for 1 second before checking WiFi connection again
  }
  if (Serial.available() > 0) {
    // Read the incoming data
    String data = Serial.readStringUntil('\n'); // Read until newline character

    Serial.println("Received data: " + data); // Debugging: print received data
    
    // Find the positions of "$" symbols
    int startIdx = data.indexOf('$');
    int endIdx = data.indexOf('$', startIdx + 1);

    // Check if both "$" symbols are found
    if (startIdx != -1 && endIdx != -1) {
      // Extract the data between "$" symbols
      String values = data.substring(startIdx + 1, endIdx);

      Serial.println("Extracted values: " + values); // Debugging: print extracted values
      
      // Extract individual values using "," as delimiter
      int commaIdx = 0;
      int nextCommaIdx = values.indexOf(',', commaIdx);
      v1 = values.substring(commaIdx, nextCommaIdx).toDouble();
      commaIdx = nextCommaIdx + 1;
      nextCommaIdx = values.indexOf(',', commaIdx);
      v2 = values.substring(commaIdx, nextCommaIdx).toDouble();
      commaIdx = nextCommaIdx + 1;
      nextCommaIdx = values.indexOf(',', commaIdx);
      v3 = values.substring(commaIdx, nextCommaIdx).toDouble();
      commaIdx = nextCommaIdx + 1;
      nextCommaIdx = values.indexOf(',', commaIdx);
      v4 = values.substring(commaIdx, nextCommaIdx).toDouble();
      commaIdx = nextCommaIdx + 1;
      nextCommaIdx = values.indexOf(',', commaIdx);
      v5 = values.substring(commaIdx, nextCommaIdx).toDouble();
      commaIdx = nextCommaIdx + 1;
      nextCommaIdx = values.indexOf(',', commaIdx);
      v6 = values.substring(commaIdx, nextCommaIdx).toDouble();
      commaIdx = nextCommaIdx + 1;
      nextCommaIdx = values.indexOf(',', commaIdx);
      v7 = values.substring(commaIdx, nextCommaIdx).toDouble();
      commaIdx = nextCommaIdx + 1;
      nextCommaIdx = values.indexOf(',', commaIdx);
      v8 = values.substring(commaIdx, nextCommaIdx).toDouble();
      commaIdx = nextCommaIdx + 1;
      v9 = values.substring(commaIdx).toDouble();

      // Print received values for debugging
      Serial.print("v1: ");
      Serial.println(v1, 2); // Print with 2 decimal places
      Serial.print("v2: ");
      Serial.println(v2, 2); // Print with 2 decimal places
      Serial.print("v3: ");
      Serial.println(v3, 2); // Print with 2 decimal places
      Serial.print("v4: ");
      Serial.println(v4, 2); // Print with 2 decimal places
      Serial.print("v5: ");
      Serial.println(v5, 6); // Print with 6 decimal places
      Serial.print("v6: ");
      Serial.println(v6, 6); // Print with 6 decimal places
      Serial.print("v7: ");
      Serial.println(v7, 2); // Print with 2 decimal places
      Serial.print("v8: ");
      Serial.println(v8, 2); // Print with 2 decimal places
      Serial.print("v9: ");
      Serial.println(v9, 2); // Print with 2 decimal places
    }
  }
  // put your main code here, to run repeatedly:
  if(!client.connected()){
      client.reconnect();
      }
  
 client.add("Battery SOC", v1);
  client.add("Battery Voltage", v2);
  
  client.ubidotsPublish("vyom");
  client.loop();
  delay(4000);
  client.add("Longitude", v6);
  client.add("Latitude", v5);
  client.ubidotsPublish("vyom");
  client.loop();
  delay(4000);
  client.add("Battery Temperature", v7);
  client.add("speed", v8);
  client.ubidotsPublish("vyom");
  client.loop();
  delay(4000);
  client.add("distance", v9);
  client.add("Cabin Temperature", v4);
  client.add("Humidity", v3);
  client.ubidotsPublish("vyom");
  client.loop();
  delay(4000);
}
void saveDataToSDCard()
{

  File dataFile = SD.open("data.csv", FILE_WRITE);

  if (dataFile)
  {

    dataFile.print(v1);
    dataFile.print(",");
    dataFile.print(v2);
    dataFile.print(",");
    dataFile.print(v3);
    dataFile.print(",");
    dataFile.print(v4);
    dataFile.print(",");
    dataFile.print(v5);
    dataFile.print(",");
    dataFile.print(v6);
    dataFile.print(",");
    dataFile.print(v7);
    dataFile.print(",");
    dataFile.print(v8);
    dataFile.print(",");
    dataFile.println(v9);
    dataFile.close();

    Serial.println("Data saved to SD card.");
  }
  else
  {
    Serial.println("Error opening data.csv file.");
  }

}

void createDataFile()
{
  File dataFile = SD.open("data.csv", FILE_WRITE);
  if (dataFile)
  {
    dataFile.println("v1,v2,v3,v4,v5,v6,v7,v8,v9");
    dataFile.close();
    Serial.println("Created data.csv file.");
  }
  else
  {
    Serial.println("Error creating data.csv file.");
  }
}