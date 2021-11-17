#include <Wire.h>
#include "MAX30100_PulseOximeter.h"
#include <ESP8266WiFi.h>
#include <EEPROM.h>
#include <Adafruit_MLX90614.h>
#include "Wire.h"0
#include "Adafruit_GFX.h"
 
#define REPORTING_PERIOD_MS 1000
 
int addr1 = 0;
int addr2 = 512;
// Connections : SCL PIN - D1 , SDA PIN - D2 , INT PIN - D0
PulseOximeter pox;
Adafruit_MLX90614 mlx = Adafruit_MLX90614();
 
float BPM, SpO2;
uint32_t tsLastReport = 0;
 
void onBeatDetected()
{
    Serial.println("Beat Detected!");
    
}
 
void setup()
{
    Serial.begin(115200);
    EEPROM.begin(1024);
      while(!mlx.begin())
  {
    //Serial.println("Error connecting to MLX sensor. Retrying");
    if (mlx.begin()) break;
  }
   
     
    pinMode(16, OUTPUT);
    
 
    Serial.print("Initializing Pulse Oximeter..");
 
    if (!pox.begin())
    {
         Serial.println("FAILED");
         
         for(;;);
    }
    else
    {
         
         Serial.println("SUCCESS");
         pox.setOnBeatDetectedCallback(onBeatDetected);
    }
 
    
     //pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);
 
}
 
void loop()
{
    pox.update();
 
    BPM = pox.getHeartRate();
    SpO2 = pox.getSpO2();
     if (millis() - tsLastReport > REPORTING_PERIOD_MS)
    {
        Serial.print("Heart rate:");
        Serial.print(BPM);
        Serial.print(" bpm / SpO2:");
        Serial.print(SpO2);
        Serial.println(" %");

        Serial.print("Ambient = "); Serial.print(mlx.readAmbientTempC());
        Serial.print("C\tObject = "); Serial.print(mlx.readObjectTempC()); Serial.println("C");

        EEPROM.write(addr1,BPM);
        EEPROM.write(addr2,SpO2);
        addr1 = addr1+1;
        addr2 = addr2+1;
        if (addr1 == 511) {
          addr1 = 0;
        }

        if (addr2 == 1024) {
          addr2 = 512;
        }
             
       
        tsLastReport = millis();
    }
}
