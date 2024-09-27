#include <Robojax_L298N_DC_motor.h>
#include <BluetoothSerial.h>
//--------- Flag structure --------------------------------------
typedef struct _vFlag
{
  uint8_t BTFlag = 0;
  uint8_t L298NFlag = 0;
  uint8_t CANFlag = 0;
  uint8_t I2C_Flag = 0;
  uint8_t BMP180Flag = 0;
  uint8_t DS18B20Flag = 0;
  uint8_t JSONFlag = 0;
  uint8_t LEDFlag = 1;
  uint8_t back_light_Flag = 0;
  uint8_t front_light_Flag = 0;
  uint8_t sensor1_Flag = 0;
  uint8_t initial_Flag = 0;
  uint8_t Tone_Flag = -1;
  uint8_t IR_RECV_Flag=0;
  uint8_t IR_SEND_Flag=0;
  uint8_t FunctionFlag = 3;
  uint8_t SendFlag = 0;
  uint8_t BMPCnt = 0;
} vFlag;
vFlag *flag_Ptr;
vFlag flag;

//----------uart--------------
#define LINE_BUFFER_LENGTH 64
//--------- uart structure --------------------------------------
typedef struct _vUart
{
  char c;
  int lineIndex = 0;
  int line1Index = 0;
  int BTlineIndex = 0;
  bool lineIsComment; 
  bool lineSemiColon;
  char line[128];
  char BTline[20];
  String inputString;
  String BTinputString;
  String S1inputString;
  int V[16];
  char ctemp[30];
  char I2C_Data[80];
  int DC_Spped = 50;
  float Voltage[16];
  int Buffer[128];
  int StartCnt = 0;
  int ReadCnt = 0;
  int sensorValue = 0;
} vUart;
vUart *Uart_Ptr;
vUart Uart;
//---------BT--------------------
BluetoothSerial SerialBT;
//------LED------------------
#define LED_BUILTIN 2
//-------------L298---------------------------------------------------
// motor 1 settings
#define CHA 0
#define ENA 4 // this pin must be PWM enabled pin if Arduino board is used
#define IN1 16
#define IN2 17
// motor 2 settings
#define IN3 18
#define IN4 19
#define ENB 5// this pin must be PWM enabled pin if Arduino board is used
#define CHB 1

const int CCW = 2; // do not change
const int CW  = 1; // do not change

#define motor1 1 // do not change
#define motor2 2 // do not change


Robojax_L298N_DC_motor motors(IN1, IN2, ENA, CHA,  IN3, IN4, ENB, CHB);
//------Bluetooth RC Controller Define ----
#define back_light 21
#define front_light 22
#define horn_Buzz 18    


void Forward() 
{  
  motors.rotate(motor1, 70, CCW);
  motors.rotate(motor2, 70, CCW);
}

void Reverse(){
  motors.rotate(motor1, 70, CW);
  motors.rotate(motor2, 70, CW);

}
void Left()
{
  //motors.rotate(motor1, 70, CW);
  motors.rotate(motor2, 70, CCW); 
}
void Right()
{
  motors.rotate(motor1, 70, CCW); 
  //motors.rotate(motor2, 70, CW);
}

void Stop() 
{
  motors.brake(1);
  motors.brake(2);  
}
//-------------------------------------------------
void setup() 
{
  Serial.begin(9600);
  Serial.println(F("init"));
  SerialBT.begin("BT_L298N");
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(back_light, OUTPUT);
  pinMode(front_light, OUTPUT);
}
//-----------------------------------------
void loop() 
{
  Serial.print(F("Main at core:"));
  Serial.println(xPortGetCoreID());
  while(1)
  {
    if(flag.LEDFlag == 1)
    {
      digitalWrite(LED_BUILTIN, HIGH); 
      vTaskDelay(300);
      
      digitalWrite(LED_BUILTIN, LOW); 
      vTaskDelay(300);
    }

    if(flag.back_light_Flag==2)
    {
      digitalWrite(back_light, HIGH);
      vTaskDelay(300);
      digitalWrite(back_light, LOW);
      vTaskDelay(300);
    }
    
    while (Serial.available() > 0)
    {
      Uart.c = Serial.read();

      if ((Uart.c == '\n') || (Uart.c == '\r'))
      { // End of line reached
        if (Uart.lineIndex > 0)
        { // Line is complete. Then execute!
          Uart.line[Uart.lineIndex] = '\0'; // Terminate string
          processCommand(Uart.line); // do something with the command
          Uart.lineIndex = 0;
          Uart.inputString = "";
        }
        else
        {
          // Empty or comment line. Skip block.
        }
        Uart.lineIsComment = false;
        Uart.lineSemiColon = false;
        Serial.println(F("ok>"));
      }
      else
      {
        if ((Uart.lineIsComment) || (Uart.lineSemiColon))
        {
          if (Uart.c == ')')
            Uart.lineIsComment = false; // End of comment. Resume line.
        }
        else
        {
          if (Uart.c == '/')
          { // Block delete not supported. Ignore character.
          }
          else if (Uart.c == '~')
          { // Enable comments flag and ignore all characters until ')' or EOL.
            Uart.lineIsComment = true;
          }
          else if (Uart.c == ';')
          {
            Uart.lineSemiColon = true;
          }
          else if (Uart.lineIndex >= LINE_BUFFER_LENGTH - 1)
          {
            Serial.println("ERROR - lineBuffer overflow");
            Uart.lineIsComment = false;
            Uart.lineSemiColon = false;
          }
          else if (Uart.c >= 'a' && Uart.c <= 'z')
          { // Upcase lowercase
            Uart.line[Uart.lineIndex] = Uart.c - 'a' + 'A';
            Uart.lineIndex = Uart.lineIndex + 1;
            Uart.inputString += (char)(Uart.c - 'a' + 'A');
          }
          else
          {
            Uart.line[Uart.lineIndex] = Uart.c;
            Uart.lineIndex = Uart.lineIndex + 1;
            Uart.inputString += Uart.c;
          }
        }
      }
    } //while (Serial.available() > 0)
    
    while (SerialBT.available())
    {
      flag.L298NFlag=1;
      String BTdata = SerialBT.readString();
      Stop();
      Serial.println(BTdata);
      BTprocessCommand(BTdata); // do something with the command
    }//while (BT.available())
 

  }
}
//-------------------BT-----------------
void BTprocessCommand(String data)
{
  if (data =="FS")
  {
    Serial.println(F("Forward"));
    Forward();
  }
  if (data == "LS")
  {
    Serial.println(F("Left"));
    Left();
  }
  if (data == "RS")
  {
    Serial.println(F("Right"));
    Right();
  }
  if (data == "BS")
  {
    Serial.println(F("Reverse"));
    Reverse();
  }
  if (data == "S")
  {
    Serial.println(F("Stop"));
    Stop();
  }
  if (data == "X")
  {
    Serial.println(F("Stop"));
    Stop();
    flag.back_light_Flag=2;
  }
  if (data == "x")
  {
    Serial.println(F("Stop"));
    Stop();
    flag.back_light_Flag=0;
  }
  if (data == "FGFS")
  {
    //左前
    motors.rotate(motor1, 60, CCW);
    motors.rotate(motor2, 100, CCW);
  }
  if (data == "FIFS")
  {
    //右前
    motors.rotate(motor1, 100, CCW);
    motors.rotate(motor2, 60, CCW);
  }
  if (data == "BHBS")
  {
    //左後
    motors.rotate(motor1, 60, CW);
    motors.rotate(motor2, 100, CW);
  }
  if (data == "BJBS")
  {
    //右後
    motors.rotate(motor1, 100, CW);
    motors.rotate(motor2, 60, CW);
  }
  if (data == "U")
  {
    //backlight
    digitalWrite(back_light, HIGH);  
    flag.back_light_Flag=1;
    //Serial.println(F("light"));
  }
  if (data == "u")
  {

    digitalWrite(back_light, LOW);
    flag.back_light_Flag=0;
    //Serial.println(F("lightoff"));
  }
  if (data == "W")
  {
    digitalWrite(front_light, HIGH);  
    flag.front_light_Flag=1;
    //Serial.println(F("light"));
  }
  if (data == "w")
  {
    digitalWrite(front_light, LOW);
    flag.front_light_Flag=0;
    //Serial.println(F("lightoff"));
  }
  
}
//----------------------------------------
void processCommand(char *data)
{
  int len, xlen, ylen, zlen, alen;
  int tempDIO;
  String stemp;

  len = Uart.inputString.length();
  //---------------------------------------
  if (strstr(data, "VER") != NULL)
  {
    Serial.println(F("ESP32_20230801"));
  }
  if (strstr(data, "F") != NULL)
  {
    Serial.println(F("Forward"));
    Forward();
  }
  if (strstr(data, "L") != NULL)
  {
    Serial.println(F("Left"));
    Left();
  }
  if (strstr(data, "R") != NULL)
  {
    Serial.println(F("Right"));
    Right();
  }
  if (strstr(data, "B") != NULL)
  {
    Serial.println(F("Reverse"));
    Reverse();
  }
  if (strstr(data, "S") != NULL)
  {
    Serial.println(F("Stop"));
    Stop();
  }
}