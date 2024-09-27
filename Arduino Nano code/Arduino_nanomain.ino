

#include <Wire.h>
#include <LiFuelGauge.h>
const int ledPin = 6;

void lowPower();
LiFuelGauge gauge(MAX17043, 0, lowPower);

// A flag to indicate a generated alert interrupt
volatile boolean alert = false;


void setup()
{
    Serial.begin(9600); // Initializes serial port
    pinMode(ledPin, LOW);
    while ( !Serial ) ;
    
    gauge.reset();  // Resets MAX17043
    delay(200);  // Waits for the initial measurements to be made
    
    // Sets the Alert Threshold to 10% of full capacity
    gauge.setAlertThreshold(10);
    ;
}

void loop()
{
    float soc=gauge.getSOC();
    float v = gauge.getVoltage();
    Serial.print(soc);
    Serial.print(",");
    Serial.println(v);
      // Gets the battery's state of charge
    
    
    if (alert )
    {
        digitalWrite(ledPin, HIGH);  
        delay(500);
        digitalWrite(ledPin, LOW);
        delay(500);
        gauge.clearAlertInterrupt();  // Resets the ALRT pin
        alert = false;
       
          // Forces the MAX17043 into sleep mode
        while ( true ) ;
    }
    
    delay(2000);
}

void lowPower() { alert = true; }
