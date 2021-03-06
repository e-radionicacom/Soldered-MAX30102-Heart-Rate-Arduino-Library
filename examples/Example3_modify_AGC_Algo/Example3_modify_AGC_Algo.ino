 /**
 **************************************************
 *
 * @file        Example3_modify_AGC_Algo.ino
 * @brief       This example code is very similar to the first, however we're adjusting the
 *              Automatic Gain Control (AGC) Algorithm which automatically determines the
 *              pulse width and current consumption of the MAX30101's LEDs as it gathers data
 *              on light absorption. This particular setting is enabled when calling
 *              "configBpm" and so in this example we demonstrate how to modify the algorithm. 
 *              A summary of the hardware connections are as follows: 
 *              SDA -> SDA
 *              SCL -> SCL
 *              RESET -> PIN 4
 *              MFIO -> PIN 5
 *              
 *              If you run into an error code check the following table to help diagnose your
 *              problem: 
 *              1 = Unavailable Command
 *              2 = Unavailable Function
 *              3 = Data Format Error
 *              4 = Input Value Error
 *              5 = Try Again
 *              255 = Error Unknown
 *
 *              Product: https://www.solde.red/333137
 *
 * @authors     Author: Elias Santistevan
 *              Date: 8/2019
 *              SparkFun Electronics
 *              
 *              Modified by: Soldered
 ***************************************************/

#include <MAX30102-Heart-Rate-Library-SOLDERED.h>
#include <Wire.h>

// Reset pin, MFIO pin
int resPin = 4;
int mfioPin = 5;

int algoRange = 80; // ADC Range (0-100%)
int algoStepSize = 20; // Step Size (0-100%)
int algoSens = 20; // Sensitivity (0-100%)
int algoSamp = 10; // Number of samples to average (0-255)

// Takes address, reset pin, and MFIO pin.
MAX30102 max30102(resPin, mfioPin); 

bioData body; 
// What's this!? This is a type (like int, byte, long) unique to the SparkFun
// Pulse Oximeter and Heart Rate Monitor. Unlike those other types it holds
// specific information on your heartrate and blood oxygen levels. BioData is
// actually a specific kind of type, known as a "struct". 
// You can choose another variable name other than "body", like "blood", or
// "readings", but I chose "body". Using this "body" varible in the 
// following way gives us access to the following data: 
// body.heartrate  - Heartrate
// body.confidence - Confidence in the heartrate value
// body.oxygen     - Blood oxygen level
// body.status     - Has a finger been sensed?

void setup(){

  Serial.begin(115200);

  Wire.begin();
  int result = max30102.begin();
  if (result == 0) // Zero errors!
    Serial.println("Sensor started!");
  
  // Adjusting the Automatic Gain Control (AGC) Algorithm
  int error = max30102.setAlgoRange(algoRange);
  if (error > 0){
    Serial.println("Could not set algorithm's Range.");  
  }

  error = max30102.setAlgoStepSize(algoStepSize);
  if (error > 0){
    Serial.println("Could not set the step size."); 
  }

  error = max30102.setAlgoSensitivity(algoSens);
  if (error > 0){ 
    Serial.println("Could not set the sensitivity.");
  }

  error = max30102.setAlgoSamples(algoSamp);
  if (error > 0){ 
    Serial.println("Could not set the sample size.");  
  }

  // Let's read back what we set....
  int algoVal = max30102.readAlgoRange();
  Serial.print("Algorithm set to: ");
  Serial.println(algoVal);

  int stepVal = max30102.readAlgoStepSize();
  Serial.print("Algorithm set to: ");
  Serial.println(stepVal);

  int senVal = max30102.readAlgoSensitivity();
  Serial.print("Algorithm set to: ");
  Serial.println(senVal);

  int sampVal = max30102.readAlgoSamples();
  Serial.print("Algorithm set to: ");
  Serial.println(sampVal);

  Serial.println("Configuring Sensor.");
  error = max30102.configBpm(MODE_ONE);
  if (error > 0){ 
    Serial.println("Could not configure the sensor.");
  }

  // Data lags a bit behind the sensor, if you're finger is on the sensor when
  // it's being configured this delay will give some time for the data to catch
  // up. 
  Serial.println("Loading up the buffer with data....");
  delay(4000);

}

void loop(){
  
    // Information from the readBpm function will be saved to our "body"
    // variable.  
    body = max30102.readBpm();
    Serial.print("Heartrate: ");
    Serial.println(body.heartRate); 
    Serial.print("Confidence: ");
    Serial.println(body.confidence); 
    Serial.print("Oxygen: ");
    Serial.println(body.oxygen); 
    Serial.print("Status: ");
    Serial.println(body.status); 
    // Slow it down or your heart rate will go up trying to keep up
    // with the flow of numbers
    delay(250); 
  
}
