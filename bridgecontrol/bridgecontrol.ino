// Protocol 3: Protect the Hydraulic Milk.
// Include libraries for the ir remote, stepper driver, and ultrasonic sensor.
#include <IRremote.h>
#include <NewPing.h>
#include <CheapStepper.h>

// Define the pin for the IR receiver.
#define IRpin 7
int sc = 0; //sonar count variable
int ic = 3; //succesful sonar ping count variable to trigger incoming vehicle
int ba = 360; //number of degrees for motor to turn
int bd = 1000; //delay between bridge opening and closing in ms
int mi = 0; // manual open/close position

// Define trigger pin, echo pin, and max measured distance in cm.
NewPing sonar(2, 3, 400);
CheapStepper bridge (8,9,10,11);
// friendship ended with robert sanchez, now tyler henry is friend

int INCOMING(){ //Standby for titanfall
  Serial.println("Titanfall incoming");
  sc = 0;
  bridge.moveDegrees (true, ba);
  delay(bd);
  bridge.moveDegrees (false, ba);
}

int manual(){
  if (mi == 0){
    bridge.moveDegrees (true, ba);
    mi++;
  }
  else {
    bridge.moveDegrees (false, ba);
    mi = 0;
  }
}

void setup() {
  Serial.begin(9600); // begins serial interface
  IrReceiver.begin(IRpin); // starts the IR receiver
  bridge.setRpm(20);
  
}

void loop() {
  
  if (IrReceiver.decode()) {
    IrReceiver.resume(); // if the arduino receives and decodes a command, it'll resume watching the receiver
  Serial.println(IrReceiver.decodedIRData.command); // prints the number of the command it just received and decodes 
  if (IrReceiver.decodedIRData.command == 69) // runs the manual open or close if the code is 69, nice
    manual();
    
  }

  delay(500);
  int distance = sonar.ping_median(10); // send 10 pings and get the median of them, pong
  int cm = NewPing::convert_cm(distance); // convert to centimeters
  //Serial.println(cm);

  // logik to make sure there's actually a car there
  if (cm > 1)
    sc++;
  if ((sc > 0) && (cm < 1))
    sc = 0;
  if (sc == ic)
    INCOMING(); // do the thing
    
  //Serial.println(sc);


  

}