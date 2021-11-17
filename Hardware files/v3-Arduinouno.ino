

#define USE_ARDUINO_INTERRUPTS true    // Set-up low-level interrupts for most accurate BPM math.
#include <PulseSensorPlayground.h>     // Includes the PulseSensorPlayground Library.   
#include <Adafruit_MLX90614.h>


//  Variables
const int PulseWire = 0;       // PulseSensor PURPLE WIRE connected to ANALOG PIN 0
const int LED13 = 13;          // The on-board Arduino LED, close to PIN 13.
int Threshold = 550;           // Determine which Signal to "count as a beat" and which to ignore.
                               // Use the "Gettting Started Project" to fine-tune Threshold Value beyond default setting.
                               // Otherwise leave the default "550" value. 
                               
PulseSensorPlayground pulseSensor;  // Creates an instance of the PulseSensorPlayground object called "pulseSensor"
Adafruit_MLX90614 mlx = Adafruit_MLX90614();

void setup() {   

  Serial.begin(9600);          // For Serial Monitor

  // Configure the PulseSensor object, by assigning our variables to it. 
  pulseSensor.analogInput(PulseWire);   
  pulseSensor.blinkOnPulse(LED13);       //auto-magically blink Arduino's LED with heartbeat.
  pulseSensor.setThreshold(Threshold);   

  // Double-check the "pulseSensor" object was created and "began" seeing a signal. 
   if (pulseSensor.begin()) {
    //Serial.println("We created a pulseSensor Object !");  //This prints one time at Arduino power-up,  or on Arduino reset.  
  }

  // Starting and checking the MLX sensor
  while(!mlx.begin())
  {
    //Serial.println("Error connecting to MLX sensor. Retrying");
    if (mlx.begin()) break;
  }

  // Initial run for the MLX Sensor
  Serial.print("Emissivity = "); 
  Serial.println(mlx.readEmissivity());
  Serial.println("================================================");

}

void loop() {

 int myBPM = pulseSensor.getBeatsPerMinute();  // Calls function on our pulseSensor object that returns BPM as an "int".
 // "myBPM" hold this BPM value now. 

  if (pulseSensor.sawStartOfBeat()) {             // Constantly test to see if "a beat happened". 
  //Serial.println("  A HeartBeat Happened ! ");    // If test is "true", print a message "a heartbeat happened".
  Serial.print("BPM: ");                          // Print phrase "BPM: " 
  Serial.println(myBPM);                          // Print the value inside of myBPM. 
  }

  // Temperature sensor read
  Serial.print("Ambient = "); Serial.print(mlx.readAmbientTempC());
  Serial.print("C\tObject = "); Serial.print(mlx.readObjectTempC()); Serial.println("C");
  Serial.print("Ambient = "); Serial.print(mlx.readAmbientTempF());
  Serial.print("F\tObject = "); Serial.print(mlx.readObjectTempF()); Serial.println("F");

  Serial.println();
  delay(500);                                     // Delay time


}
