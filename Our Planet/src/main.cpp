#include <Arduino.h>
#include <ESP32Servo.h>
#include <WiFi.h>
#include <HTTPClient.h>
#define USE_SERIAL Serial

int check = 0;

TaskHandle_t wifiTaskHandle;
TaskHandle_t stepperTaskHandle;
TaskHandle_t servoTaskHandle;

Servo myservo; // create servo object to control a servo
int posVal = 0; // variable to store the servo position
int servoPin = 15; // Servo motor pin
int outPorts[] = {33, 27, 26, 25};

String address= "http://134.122.113.13/arj2145/running";

const char *ssid_Router     = "Columbia University"; //Enter the router name
const char *password_Router = ""; //Enter the router password

void moveOneStep(bool dir) {
  // Define a variable, use four low bit to indicate the state of port
  static byte out = 0x01;
  // Decide the shift direction according to the rotation direction
  if (dir) {  // ring shift left
    out != 0x08 ? out = out << 1 : out = 0x01;
  }
  else {      // ring shift right
    out != 0x01 ? out = out >> 1 : out = 0x08;
  }
  // Output singal to each port
  for (int i = 0; i < 4; i++) {
    digitalWrite(outPorts[i], (out & (0x01 << i)) ? HIGH : LOW);
  }
}

//Suggestion: the motor turns precisely when the ms range is between 3 and 20
void moveSteps(bool dir, int steps, byte ms) {
  for (unsigned long i = 0; i < steps; i++) {
    moveOneStep(dir); // Rotate a step
    delay(constrain(ms,3,20));        // Control the speed
  }
}

void moveAround(bool dir, int turns, byte ms){
  for(int i=0;i<turns;i++)
    moveSteps(dir,32*64,ms);
}
void moveAngle(bool dir, int angle, byte ms){
  moveSteps(dir,(angle*32*64/360),ms);
}

void wifiTask( void * parameter ) {
for(;;){
  if((WiFi.status() == WL_CONNECTED)) {
      HTTPClient http;
      http.begin(address);
   
      int httpCode = http.GET(); // start connection and send HTTP header
      if (httpCode == HTTP_CODE_OK) { 
          String response = http.getString();
          if (response.equals("false")) {
              // Do not run sculpture, perhaps sleep for a couple seconds
              Serial.println("FALSE");
              check = 0;
          }
          else if(response.equals("true")) {
              // Run sculpture

                Serial.println( analogRead(12));
               check = 1;
//                moveSteps(true, 32 * 64, 3);
  
          }
          
          USE_SERIAL.println("Response was: " + response);
      } else {
          USE_SERIAL.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
      }
      http.end();
//      delay(500); // sleep for half of a second
    }
}
}

void servoTask( void * parameter ) {
  for(;;){

    delay(500);
    if(check == 1){
              for(int i = 0; i <= 180; i++){
                myservo.write(i);
                delay(25);
              }
              
              delay(500);
                                      
              for(int i = 180; i >= 0; i--){
                myservo.write(i);
                delay(25);
              }      
          }
  }

}

void stepperTask( void * parameter ) {
  for(;;){
    Serial.println("stepper checking");
    if(check == 1){
      moveSteps(true, 32 * 64, 3);
     }
  }
}

void setup() {

  USE_SERIAL.begin(115200);

  WiFi.begin(ssid_Router, password_Router);
  USE_SERIAL.println(String("Connecting to ")+ssid_Router);
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    USE_SERIAL.print(".");
  }
  USE_SERIAL.println("\nConnected, IP address: ");
  USE_SERIAL.println(WiFi.localIP());
  USE_SERIAL.println("Setup End");

  for (int i = 0; i < 4; i++) {
    pinMode(outPorts[i], OUTPUT);
  }
  
  myservo.setPeriodHertz(50); // standard 50 hz servo
  myservo.attach(servoPin, 500, 2500); // attaches the servo on servoPin to the servo object

    xTaskCreatePinnedToCore(
    wifiTask,                       /* Function to implement the task */
    "wifiTask",                    /* Name of the task */
    4000,                           /* Stack size in words */
    NULL,                           /* Task input parameter */
    5,                              /* Priority of the task */
    &wifiTaskHandle,                /* Task handle. */
    0);                             /* Core where the task should run */

  Serial.println("created stepper task");
  
  xTaskCreatePinnedToCore(
    stepperTask,                       /* Function to implement the task */
    "stepperTask",                    /* Name of the task */
    4000,                           /* Stack size in words */
    NULL,                           /* Task input parameter */
    5,                              /* Priority of the task */
    &stepperTaskHandle,                /* Task handle. */
    0);                             /* Core where the task should run */

  Serial.println("created stepper task");

  xTaskCreatePinnedToCore(
    servoTask,                       /* Function to implement the task */
    "servoTask",                    /* Name of the task */
    4000,                           /* Stack size in words */
    NULL,                           /* Task input parameter */
    5,                              /* Priority of the task */
    &servoTaskHandle,                /* Task handle. */
    1);                             /* Core where the task should run */

  Serial.println("created servo task");
}

void loop() {

}
