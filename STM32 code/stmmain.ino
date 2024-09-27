  #include <TinyGPS++.h>         //Library for using GPS functions
  #include<DHT.h>                     //Library for using DHT sensor 
  #define DHTPIN PA0
  #define SPEED_SENSOR_PIN PB4 
  #define DHTTYPE DHT11

  TinyGPSPlus gps;               //Object gps for class TinyGPSPlus 
  DHT dht(DHTPIN, DHTTYPE);     //initilize object dht for class DHT with DHT pin with STM32 and DHT type as DHT11
  int DSPIN = PA8;
  float wheelRadius = .15; // add your own wheel radius here
  volatile int pulseCount = 0;
  unsigned long lastTime = 0;
  float speed = 0;
  const int trigPin = PB6;
  const int echoPin = PB7;
  float duration, distance ,value1,value2;

  
  volatile boolean alert = false;

  void setup()
  {

    Serial3.begin(9600);
    Serial2.begin(9600);
    Serial1.begin(9600);
    dht.begin();
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
    pinMode(SPEED_SENSOR_PIN, INPUT);
    attachInterrupt(digitalPinToInterrupt(SPEED_SENSOR_PIN), countPulses, RISING);  

    delay(200);  // Waits for the initial measurements to be made
  
                              
    delay(600);

  }
  
  void loop()
  {

    static String receivedData; // Variable to store received data across loop iterations

    // Check if data is available on Serial3
    while (Serial3.available() > 0) {
      char incomingChar = Serial3.read(); // Read the incoming byte

      // Check for end of line character
      if (incomingChar == '\n') {
        // Process received data if end of line is detected
        processReceivedData(receivedData);

        // Clear the string to receive new data
        receivedData = "";
      } else {
        // Append the incoming character to the received data string
        receivedData += incomingChar;
      }
    }


  //new code
    float h = dht.readHumidity();       //Gets Humidity value
    float t = dht.readTemperature();    //Gets Temperature value
    Serial.print("temperature: ");
    Serial.println(t);
    Serial.print("Humidity: ");
    Serial.println(h);

    GPSDelay(1000);

    unsigned long start;

    double lat_val, lng_val;

    bool loc_valid;

    lat_val = gps.location.lat();        //Gets the latitude

    loc_valid = gps.location.isValid(); 

    lng_val = gps.location.lng();
    if (!loc_valid)                     //Excecutes when invalid data is received from GPS

    { 

      Serial.print("Latitude : ");

      Serial.println("*****");

      Serial.print("Longitude : ");

      Serial.println("*****");

      delay(4000);

    }

    else                              //Excutes when valid data is received from GPS

    {

      Serial.print("Latitude : ");
      Serial.println(lat_val, 6);   //Prints latitude at Serial Monitor

      Serial.print("Longitude : ");
      Serial.println(lng_val, 6);   //Prints longitude at Serial monitor  

    }
    double temp = TempRead();
    temp  = temp * 0.0625; // conversion accuracy is 0.0625 / LSB
    Serial.print("Battery temperature: ");
    Serial.print(temp);
    Serial.println(" °C");
    delay(500);
    if (millis() - lastTime >= 1000) {
      detachInterrupt(digitalPinToInterrupt(SPEED_SENSOR_PIN));
      speed = (pulseCount / 20.0) * wheelRadius * 3.6;
      pulseCount = 0;
      attachInterrupt(digitalPinToInterrupt(SPEED_SENSOR_PIN), countPulses, RISING);
      lastTime = millis();
    }

    Serial.println("Speed (km/h):");
    Serial.println(speed, 1);
    Serial.println("Speed (m/s):");
    Serial.println(speed / 3.6, 1);
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    duration = pulseIn(echoPin, HIGH);
    distance = (duration*.0343)/2;
    Serial.print("Distance: ");
    Serial.println(distance);

    
    delay(100);
    Serial2.print("$");
    Serial2.print(value1);
    Serial2.print(",");
    Serial2.print(value2);
    Serial2.print(",");
    Serial2.print(h);
    Serial2.print(",");
    Serial2.print(t);
    Serial2.print(",");
    Serial2.print(lat_val, 6);
    Serial2.print(",");
    Serial2.print(lng_val, 6);
    Serial2.print(",");
    Serial2.print(temp);
    Serial2.print(",");
    Serial2.print(speed, 1);
    Serial2.print(",");
    Serial2.print(distance);
    Serial2.println("$");
    



    
  }
  void processReceivedData(String data) {
    // Parse received data
    int commaIndex = data.indexOf(',');
    if (commaIndex != -1) { // Check if comma is found
      String value1_str = data.substring(0, commaIndex);
      String value2_str = data.substring(commaIndex + 1);
      value1 = value1_str.toFloat();
      value2 = value2_str.toFloat();

      // Print received values for debugging
      Serial.print("Received values: ");
      Serial.print(value1);
      Serial.print(", ");
      Serial.println(value2);
      
    }
  }
  void countPulses() {
    pulseCount++;

  }

  static void GPSDelay(unsigned long ms)          //Delay for receiving data from GPS

  {

    unsigned long start = millis();

    do

    {

      while (Serial1.available()) 

      gps.encode(Serial1.read());

    } while (millis() - start < ms);
          //Gets the longitude
    
  }
  boolean DS18B20_Init()
  {
    pinMode(DSPIN, OUTPUT);
    digitalWrite(DSPIN, HIGH);
    delayMicroseconds(5);
    digitalWrite(DSPIN, LOW);
    delayMicroseconds(750);//480-960
    digitalWrite(DSPIN, HIGH);
    pinMode(DSPIN, INPUT);
    int t = 0;
    while (digitalRead(DSPIN))
    {
      t++;
      if (t > 60) return false;
      delayMicroseconds(1);
    }
    t = 480 - t;
    pinMode(DSPIN, OUTPUT);
    delayMicroseconds(t);
    digitalWrite(DSPIN, HIGH);
    return true;
  }
  
  void DS18B20_Write(byte data)
  {
    pinMode(DSPIN, OUTPUT);
    for (int i = 0; i < 8; i++)
    {
      digitalWrite(DSPIN, LOW);
      delayMicroseconds(10);
      if (data & 1) digitalWrite(DSPIN, HIGH);
      else digitalWrite(DSPIN, LOW);
      data >>= 1;
      delayMicroseconds(50);
      digitalWrite(DSPIN, HIGH);
    }
  }
  
  byte DS18B20_Read()
  {
    pinMode(DSPIN, OUTPUT);
    digitalWrite(DSPIN, HIGH);
    delayMicroseconds(2);
    byte data = 0;
    for (int i = 0; i < 8; i++)
    {
      digitalWrite(DSPIN, LOW);
      delayMicroseconds(1);
      digitalWrite(DSPIN, HIGH);
      pinMode(DSPIN, INPUT);
      delayMicroseconds(5);
      data >>= 1;
      if (digitalRead(DSPIN)) data |= 0x80;
      delayMicroseconds(55);
      pinMode(DSPIN, OUTPUT);
      digitalWrite(DSPIN, HIGH);
    }
    return data;
  }
  
  int TempRead()
  {
    if (!DS18B20_Init()) return 0;
    DS18B20_Write (0xCC); // Send skip ROM command
    DS18B20_Write (0x44); // Send reading start conversion command
    if (!DS18B20_Init()) return 0;
    DS18B20_Write (0xCC); // Send skip ROM command
    DS18B20_Write (0xBE); // Read the register, a total of nine bytes, the first two bytes are the conversion value
    int temp = DS18B20_Read (); // Low byte
    temp |= DS18B20_Read () << 8; // High byte
    return temp;
  }