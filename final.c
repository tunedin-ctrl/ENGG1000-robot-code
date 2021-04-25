// Arduino Pins
#define RIGHT_MOTOR_ENABLE        3 
#define RIGHT_MOTOR_DIRECTION_1   4  
#define RIGHT_MOTOR_DIRECTION_2   5  
#define LEFT_MOTOR_DIRECTION_1    6 
#define LEFT_MOTOR_DIRECTION_2    12 
#define LEFT_MOTOR_ENABLE         8
#define SPEAKER_PIN               7    
#define GRIP_MOTOR_ENABLE         9    
#define GRIP_MOTOR_DIRECTION_1    10 
#define GRIP_MOTOR_DIRECTION_2    11
#define IR_DISTURBANCE            800


// Constants
#define GRIP_DELAY                400
#define STOP_DELAY                2000

 
// IR Receiver PIN
int IR_RECEIVE_PIN = 2;

// PiezoPin / Speaker Pin
int piezoPin = 7;

//Green & Red LED Pins
int green_led = 13;
int red_led = 2;  


// Global variables
int irCommand = 0;
bool sensorKeyReceived = LOW;
bool power = LOW;
bool automatic = LOW;
unsigned char autoMode = 0;
long time = 0L;
int count = 0;
int motorSpeedLeft = 128;
int motorSpeedRight = 128;
int motorSpeedGrip = 128;
unsigned long startTime = 0UL;

 
// Initialise Input/Output Pins
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
  pinMode(SPEAKER_PIN, OUTPUT);
  pinMode(A0, INPUT);
  pinMode(green_led, OUTPUT);
  pinMode(red_led, OUTPUT);
}


// Initiliase serial communications
void setup() {

  Serial.begin(9600);
  initializeIOPins();    
  Serial.println("HELLO");    
}

// Move robot forward
void robotForward(){
  digitalWrite(LEFT_MOTOR_ENABLE, HIGH);
  digitalWrite(RIGHT_MOTOR_ENABLE, HIGH);  
  digitalWrite(RIGHT_MOTOR_DIRECTION_1, LOW);
  digitalWrite(RIGHT_MOTOR_DIRECTION_2, HIGH);  
  digitalWrite(LEFT_MOTOR_DIRECTION_1, HIGH);
  digitalWrite(LEFT_MOTOR_DIRECTION_2, LOW);  
}

 

 

 
// Move robot backward
void robotBackward(){
  digitalWrite(LEFT_MOTOR_ENABLE, HIGH);
  digitalWrite(RIGHT_MOTOR_ENABLE, HIGH);  
  digitalWrite(RIGHT_MOTOR_DIRECTION_1, HIGH);  
  digitalWrite(RIGHT_MOTOR_DIRECTION_2, LOW);
  digitalWrite(LEFT_MOTOR_DIRECTION_1, LOW);  
  digitalWrite(LEFT_MOTOR_DIRECTION_2, HIGH);
}

 

 

 
// Stop robot
void robotStop(){
  digitalWrite(LEFT_MOTOR_ENABLE, LOW);
  digitalWrite(RIGHT_MOTOR_ENABLE, LOW);  
  digitalWrite(RIGHT_MOTOR_DIRECTION_1, LOW);  
  digitalWrite(RIGHT_MOTOR_DIRECTION_2, LOW);
  digitalWrite(LEFT_MOTOR_DIRECTION_1, LOW);  
  digitalWrite(LEFT_MOTOR_DIRECTION_2, LOW);
}
 

 

// Stop crane
void gripStop(){
  digitalWrite(GRIP_MOTOR_ENABLE, LOW); 
  digitalWrite(GRIP_MOTOR_DIRECTION_1, LOW);
  digitalWrite(GRIP_MOTOR_DIRECTION_2, LOW);  
}

 

 

 
// Lower crane
void gripUp(){
  digitalWrite(GRIP_MOTOR_ENABLE, HIGH);  
  digitalWrite(GRIP_MOTOR_DIRECTION_1, LOW);
  digitalWrite(GRIP_MOTOR_DIRECTION_2, HIGH);  
}

 

 

 
// Lift crane
void gripDown(){
  digitalWrite(GRIP_MOTOR_ENABLE, HIGH);  
  digitalWrite(GRIP_MOTOR_DIRECTION_1, HIGH);  
  digitalWrite(GRIP_MOTOR_DIRECTION_2, LOW);
} 

 

// Play tone on speaker
void Speaker(){
  tone(piezoPin, 1000, 800);
  delay(1100);
}


// Turn green led on
void forward_led_on(){
  digitalWrite(green_led,HIGH);
  
}

 

// Turn red led on
void backward_led_on(){
  digitalWrite(red_led,HIGH);
  
}


// Turn green led off
void forward_led_off(){
  digitalWrite(green_led,LOW);
  
}

 

// Turn red led off
void backward_led_off(){
  digitalWrite(red_led,LOW);
  
}
 

 


void loop() {
   
    //Robot always moving foward until sensor
    robotForward();
    forward_led_on();
   
    // Ir receiver detects first IR source
    if (analogRead(A0) >= IR_DISTURBANCE && count == 0) {
        robotStop();
        forward_led_off();
        backward_led_on();
        Serial.println("Stop");
        delay(1000);
        robotForward();
        forward_led_on();
        backward_led_off();
        delay(2000);
        count++;
    }

    // Ir reciever detects second IR source
    if (analogRead(A0) >= IR_DISTURBANCE && count == 1) {
        robotStop();
        forward_led_off();
        backward_led_on();
        delay(400);
        robotBackward();
        delay(210);
        robotStop();
        delay(400);
        gripDown();
        delay(500);
        gripStop();
        gripUp();
        delay(250);
        gripStop();
        delay(140);
        Speaker();
        Speaker();
        robotBackward();
        forward_led_on();
        backward_led_off();
        delay(250);
        // When robot is reversing, play speaker
        while (analogRead(A0) <= IR_DISTURBANCE) {
          Speaker();
          robotBackward();
        }
        
        // When robot detects first Ir source again, repeat from the start
        if (analogRead(A0) >= IR_DISTURBANCE){
            count = 0;
        }
    }
    
      


}
The_final_robot_code.ino
6 KB