#include "BluetoothSerial.h"
#include <ESP32Servo.h>

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;

Servo myservo1;
Servo myservo2;

//L298 Connection
//const int motor_IN1  = 4;
//const int motor_IN2  = 16;
//const int motor_IN3  = 15;
//const int motor_IN4  = 2;
const int motor_IN1  = 2;
const int motor_IN2  = 4;
const int motor_IN3  = 16;
const int motor_IN4  = 17;

const int motor_relay  = 26;

//Leds connected 
const int light_FL  = 13;
const int light_FR  = 12;
const int light_RL  = 14;
const int light_RR  = 27;
//Buzzer / Speaker 
const int buzzer = 26 ;

//setting PWM
const int freq = 5000;
const int ledChannel_1 = 1;
const int ledChannel_2 = 2;
const int ledChannel_3 = 3;
const int ledChannel_4 = 4;
const int resolution = 8;


//Useful Variables
int state;
int vSpeed = 255;   // Default speed, from 0 to 255
int offset_motor_left = 10;
int offset_motor_right = 0;

void setup() {
//  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(3);
  myservo1.attach(25);
  myservo2.attach(32);
  myservo1.write(0);
  myservo2.write(0);
  // Set pins as outputs:
  pinMode(ledChannel_1, OUTPUT);
  pinMode(ledChannel_2, OUTPUT);
  pinMode(ledChannel_3, OUTPUT);
  pinMode(ledChannel_4, OUTPUT);
  pinMode(light_FL, OUTPUT);
  pinMode(light_FR, OUTPUT);
  pinMode(light_RL, OUTPUT);
  pinMode(light_RR, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(motor_relay, OUTPUT);
  
  digitalWrite(motor_relay, LOW);
  digitalWrite(buzzer, HIGH);
  //    pinMode(BTState, INPUT);
  // Initialize serial communication at 9600 bits per second:
  Serial.begin(115200);
  SerialBT.begin("R.O._Robot_Car"); //Bluetooth device name

  // configure LED PWM functionalitites
  ledcSetup(ledChannel_1, freq, resolution);
  ledcSetup(ledChannel_2, freq, resolution);
  ledcSetup(ledChannel_3, freq, resolution);
  ledcSetup(ledChannel_4, freq, resolution);
  // attach the channel to the GPIO to be controlled
  ledcAttachPin(motor_IN1, ledChannel_1);
  ledcAttachPin(motor_IN2, ledChannel_2);
  ledcAttachPin(motor_IN3, ledChannel_3);
  ledcAttachPin(motor_IN4, ledChannel_4);
}

void loop() {

  //Save income data to variable 'state'
  if (SerialBT.available() > 0) {
    state = SerialBT.read();
    Serial.println(state);
  }  

  //Change speed if state is equal from 0 to 4. Values must be from 0 to 255 (PWM)
  if (state == '0') {
    vSpeed = 0;
  }
  else if (state == '1') {
    vSpeed = 100;
  }
  else if (state == '2') {
    vSpeed = 127;
  }
  else if (state == '3') {
    vSpeed = 150;
  }
  else if (state == '4') {
    vSpeed = 180;
  }
  else if (state == '5') {
    vSpeed = 200;
  }
  else if (state == '6') {
    vSpeed = 210;
  }
  else if (state == '7') {
    vSpeed = 220;
  }
  else if (state == '8') {
    vSpeed = 230;
  }
  else if (state == '9') {
    vSpeed = 240;
  }
  else if (state == 'q') {
    vSpeed = 255;
  }

  /***********************Forward****************************/
  //If state is equal with letter 'F', car will go forward!
  if (state == 'F') {
    ledcWrite(ledChannel_1, vSpeed-offset_motor_left); ledcWrite(ledChannel_2, 0);
    ledcWrite(ledChannel_3, vSpeed-offset_motor_right);ledcWrite(ledChannel_4, 0);
  }
  /**********************Forward Left************************/
  //If state is equal with letter 'G', car will go forward left
  else if (state == 'G') {
    ledcWrite(ledChannel_1, 0);                        ledcWrite(ledChannel_2, 0);
    ledcWrite(ledChannel_3, vSpeed-offset_motor_right);ledcWrite(ledChannel_4, 0);
  }
  /**********************Forward Right************************/
  //If state is equal with letter 'I', car will go forward right
  else if (state == 'I') {
    ledcWrite(ledChannel_1, vSpeed-offset_motor_left); ledcWrite(ledChannel_2, 0);
    ledcWrite(ledChannel_3, 0);                        ledcWrite(ledChannel_4, 0);
  }
  /***********************Backward****************************/
  //If state is equal with letter 'B', car will go backward
  else if (state == 'B') {
    ledcWrite(ledChannel_1, 0);                        ledcWrite(ledChannel_2, vSpeed-offset_motor_left);
    ledcWrite(ledChannel_3, 0);                        ledcWrite(ledChannel_4, vSpeed-offset_motor_right);
  }
  /**********************Backward Left************************/
  //If state is equal with letter 'H', car will go backward left
  else if (state == 'H') {
    ledcWrite(ledChannel_1, 0);                        ledcWrite(ledChannel_2, 0);
    ledcWrite(ledChannel_3, 0);                        ledcWrite(ledChannel_4, vSpeed-offset_motor_right);
  }
  /**********************Backward Right************************/
  //If state is equal with letter 'J', car will go backward right
  else if (state == 'J') {
    ledcWrite(ledChannel_1, 0);                        ledcWrite(ledChannel_2, vSpeed-offset_motor_left);
    ledcWrite(ledChannel_3, 0);                        ledcWrite(ledChannel_4, 0);
  }
  /***************************Left*****************************/
  //If state is equal with letter 'L', wheels will turn left
  else if (state == 'L') {
    ledcWrite(ledChannel_1, 0);                        ledcWrite(ledChannel_2, vSpeed-offset_motor_left);
    ledcWrite(ledChannel_3, vSpeed-offset_motor_right);ledcWrite(ledChannel_4, 0);
  }
  /***************************Right*****************************/
  //If state is equal with letter 'R', wheels will turn right
  else if (state == 'R') {
    ledcWrite(ledChannel_1, vSpeed-offset_motor_left); ledcWrite(ledChannel_2, 0);
    ledcWrite(ledChannel_3, 0);                        ledcWrite(ledChannel_4, vSpeed-offset_motor_right);
  }
  /************************Lights*****************************/
  //If state is equal with letter 'W', turn leds on or of off
  else if (state == 'W') {
//    digitalWrite(light_FL, HIGH);
//    digitalWrite(light_FR, HIGH);
    myservo1.write(90);
   delay(1000);
    digitalWrite(motor_relay, HIGH);
  }
  else if (state == 'w') {
//    digitalWrite(light_FL, LOW);
//    digitalWrite(light_FR, LOW);
    myservo1.write(0);
    digitalWrite(motor_relay, LOW);
  }
  else if (state == 'U') {
//    digitalWrite(light_RL, HIGH);
//    digitalWrite(light_RR, HIGH);
    myservo2.write(90);
  }
  else if (state == 'u') {
//    digitalWrite(light_RL, LOW);
//    digitalWrite(light_RR, LOW);
    myservo2.write(0);
  }
  /**********************Horn sound***************************/
  //If state is equal with letter 'V', play (or stop) horn sound
  else if (state == 'V') {
    digitalWrite(buzzer, LOW);
  }
  else if (state == 'v') {
    digitalWrite(buzzer, HIGH);
  }
  /************************Stop*****************************/
  //If state is equal with letter 'S', stop the car
  else if (state == 'S') {
    ledcWrite(ledChannel_1, 0);  ledcWrite(ledChannel_2, 0);
    ledcWrite(ledChannel_3, 0);  ledcWrite(ledChannel_4, 0);
  }

}
