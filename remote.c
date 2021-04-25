// -------------------------------------------------------
// Global Variables
// -------------------------------------------------------
#include <stdio.h>
// #include <IRremote.h>

//IO Pin definition

#define RIGHT_MOTOR_ENABLE        3 
#define RIGHT_MOTOR_DIRECTION_1   4  
#define RIGHT_MOTOR_DIRECTION_2   5  
#define LEFT_MOTOR_DIRECTION_1    6 
#define LEFT_MOTOR_DIRECTION_2    12 
#define LEFT_MOTOR_ENABLE         8    
#define GRIP_MOTOR_ENABLE         9    
#define GRIP_MOTOR_DIRECTION_1    10 
#define GRIP_MOTOR_DIRECTION_2    11

#define GRIP_DELAY                400
#define STOP_DELAY                500

// IR Receive PIN
int IR_RECEIVE_PIN = 2;

// Global variables
int irCommand = 0;
bool sensorKeyReceived = LOW;
bool power = LOW;
bool automatic = LOW;
unsigned char autoMode = 0;
long time = 0L;

IRrecv irrecv(IR_RECEIVE_PIN);

int motorSpeedLeft = 128;
int motorSpeedRight = 128;
int motorSpeedGrip = 128;
unsigned long startTime = 0UL;


void initializeIOPins(){
  
  pinMode(RIGHT_MOTOR_ENABLE, OUTPUT);
  pinMode(RIGHT_MOTOR_DIRECTION_1, OUTPUT);
  pinMode(RIGHT_MOTOR_DIRECTION_2, OUTPUT);
  pinMode(LEFT_MOTOR_DIRECTION_1, OUTPUT);
  pinMode(LEFT_MOTOR_DIRECTION_2, OUTPUT);
  pinMode(LEFT_MOTOR_ENABLE, OUTPUT);  
  pinMode(GRIP_MOTOR_ENABLE, OUTPUT);
  pinMode(GRIP_MOTOR_DIRECTION_1, OUTPUT);
  pinMode(GRIP_MOTOR_DIRECTION_2, OUTPUT);
}
  
void setup() {

  Serial.begin(9600);
  IrReceiver.begin(IR_RECEIVE_PIN, DISABLE_LED_FEEDBACK);
  initializeIOPins();    
  Serial.println("HELLO");    
}

bool checkDelay(unsigned long period)
{
   if ((unsigned long) (millis() - startTime) > period)
   {
      startTime = (unsigned long)millis();
      return HIGH; 
   }
   return LOW; 
}

void robotForward(){
  digitalWrite(LEFT_MOTOR_ENABLE, HIGH);
  digitalWrite(RIGHT_MOTOR_ENABLE, HIGH);  
  digitalWrite(RIGHT_MOTOR_DIRECTION_1, LOW);
  digitalWrite(RIGHT_MOTOR_DIRECTION_2, HIGH);  
  digitalWrite(LEFT_MOTOR_DIRECTION_1, HIGH);
  digitalWrite(LEFT_MOTOR_DIRECTION_2, LOW);  
}

void robotBackward(){
  digitalWrite(LEFT_MOTOR_ENABLE, HIGH);
  digitalWrite(RIGHT_MOTOR_ENABLE, HIGH);  
  digitalWrite(RIGHT_MOTOR_DIRECTION_1, HIGH);  
  digitalWrite(RIGHT_MOTOR_DIRECTION_2, LOW);
  digitalWrite(LEFT_MOTOR_DIRECTION_1, LOW);  
  digitalWrite(LEFT_MOTOR_DIRECTION_2, HIGH);
}

void robotStop(){
  digitalWrite(LEFT_MOTOR_ENABLE, LOW);
  digitalWrite(RIGHT_MOTOR_ENABLE, LOW);  
  digitalWrite(RIGHT_MOTOR_DIRECTION_1, LOW);  
  digitalWrite(RIGHT_MOTOR_DIRECTION_2, LOW);
  digitalWrite(LEFT_MOTOR_DIRECTION_1, LOW);  
  digitalWrite(LEFT_MOTOR_DIRECTION_2, LOW);
}


void robotAllStop(){
  gripStop();
  robotStop();
  autoMode = 0;  
  automatic = LOW;
  sensorKeyReceived = LOW;
}

void gripStop(){
  digitalWrite(GRIP_MOTOR_ENABLE, LOW); 
  digitalWrite(GRIP_MOTOR_DIRECTION_1, LOW);
  digitalWrite(GRIP_MOTOR_DIRECTION_2, LOW);  
}

void gripUp(){
  digitalWrite(GRIP_MOTOR_ENABLE, HIGH);  
  digitalWrite(GRIP_MOTOR_DIRECTION_1, LOW);
  digitalWrite(GRIP_MOTOR_DIRECTION_2, HIGH);  
}

void gripDown(){
  digitalWrite(GRIP_MOTOR_ENABLE, HIGH);  
  digitalWrite(GRIP_MOTOR_DIRECTION_1, HIGH);  
  digitalWrite(GRIP_MOTOR_DIRECTION_2, LOW);
}


void loop(){

  if (IrReceiver.decode()) {  // Grab an IR code
    irCommand = IrReceiver.decodedIRData.command;
    switch (irCommand){

      case 0x00: //power
        if (power == LOW) {   
          power = HIGH;
          Serial.println("Power ON");         
        }
        break;

      case 0xc: //Camera - auto Mode
        Serial.println("Auto");       
        autoMode = 0;  
        automatic = HIGH;
        break;

      case 0x6: //enter
        Serial.println("Enter");         
        robotAllStop(); 
        break;
    
      case 0x44: //up
        Serial.println("Lift up");         
        gripUp();    
        break;
    
      case 0x48: //down
        Serial.println("Put down");         
        gripDown();    
        break;

      case 0x4c:  //left
        Serial.println("run forward");         
        robotForward();
        break;

      case 0x40:  //right
        Serial.println("run backward");               
        robotBackward();
        break;

      default:
        Serial.println(irCommand, HEX);
        if (automatic){
          sensorKeyReceived = HIGH;
        }
        break;
    }
    irrecv.resume();
  }
  if (!power) {   
    robotAllStop(); 
 }

 if (automatic)
 {
    if (sensorKeyReceived)
    {
      autoMode++;
      sensorKeyReceived = LOW;
      delay(400);
      irrecv.resume();
    }
    switch (autoMode)
    {
      case 0:
        robotForward();
        break;

      case 1:
        robotStop();
        autoMode++;
        Serial.println("forward stop");                 
        startTime = (unsigned long)millis();
        break;

      case 2:
        if (checkDelay(STOP_DELAY)){
          gripUp();  
          autoMode++;
          Serial.println("up"); 
          startTime = (unsigned long)millis();
        }
        break;
        
      case 3:        
        if (checkDelay(GRIP_DELAY)){
          gripStop();
          Serial.println("up stop");           
          autoMode++;
          startTime = (unsigned long)millis();
        }
        break;
        
      case 4:
        if (checkDelay(STOP_DELAY)){
          robotBackward();
          Serial.println("back");  
        }
        break;

      case 5:
        robotStop(); 
        autoMode++;
        Serial.println("back stop"); 
        startTime = (unsigned long)millis();
        break;

      case 6:
        if (checkDelay(STOP_DELAY)){
          gripDown();  
          autoMode++;
          Serial.println("down"); 
          startTime = (unsigned long)millis(); 
        }
        break;
        
      case 7:        
        if (checkDelay(GRIP_DELAY)){
          gripStop();     
          autoMode++;
          Serial.println("down stop"); 
          startTime = (unsigned long)millis(); 
        }
        break;
        
      case 8:
        if (checkDelay(STOP_DELAY)){
          Serial.println("one circle"); 
          robotBakcward();
          autoMode++;
        }
        
        break;

      default:
        autoMode = 0;
        break;
    }
  }
}