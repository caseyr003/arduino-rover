#include <Servo.h>  

#define ON  1   
#define OFF 0       

Servo myservo;      

//Definition of Pins connected to Arduino UNO
const int servoPin = 3;

const int leftMotorForward = 8;
const int leftMotorBack = 7;
const int leftMotorSpeed = 6;

const int rightMotorForward = 2;
const int rightMotorBack = 4;
const int rightMotorSpeed = 5;

const int echoPin = 12;
const int trigPin = 13;

const int LED_R = 11;
const int LED_G = 10;
const int LED_B = 9;

//Distance in inches
long duration, distance;

void setup()
{
  //Set the baud rate of serial data transmission to 9600
  Serial.begin(9600);

  //Initialize the motor drive IO as the output mode
  pinMode(leftMotorForward, OUTPUT);
  pinMode(leftMotorBack, OUTPUT);
  pinMode(rightMotorForward, OUTPUT);
  pinMode(rightMotorBack, OUTPUT);

  //Initialize the RGB IO as the output mode
  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(LED_B, OUTPUT);

  //Initialize ultrasonic pin
  pinMode(echoPin, INPUT);   
  pinMode(trigPin, OUTPUT);   

  myservo.attach(servoPin);

  //Initialize the servo position forward
  //May need to tweak to find forward position
  int ServoPos = 146;
  myservo.write(ServoPos);
}

//Move both motors forward
void forward(int speed)
{
  //Left motor forward
  digitalWrite(leftMotorForward, HIGH);   
  digitalWrite(leftMotorBack, LOW);  
  analogWrite(leftMotorSpeed, speed);

  //Right motor forward
  digitalWrite(rightMotorForward, HIGH);  
  digitalWrite(rightMotorBack, LOW); 
  analogWrite(rightMotorSpeed, speed);
}

//Move both motors back
void back(int speed)
{
  //Left motor back
  digitalWrite(leftMotorForward, LOW);     
  digitalWrite(leftMotorBack, HIGH);  
  analogWrite(leftMotorSpeed, speed);

  //Right motor back
  digitalWrite(rightMotorForward, LOW);    
  digitalWrite(rightMotorBack, HIGH); 
  analogWrite(rightMotorSpeed, speed);
}

//Move left motor back and right forward
void spin_left(int speed)
{
  //Left motor back
  digitalWrite(leftMotorForward, LOW);     
  digitalWrite(leftMotorBack, HIGH);  
  analogWrite(leftMotorSpeed, speed);

  //Right motor forward
  digitalWrite(rightMotorForward, HIGH);  
  digitalWrite(rightMotorBack, LOW); 
  analogWrite(rightMotorSpeed, speed);
}

//Move left motor forward and right back
void spin_right(int speed)
{
  //Left motor forward
  digitalWrite(leftMotorForward, HIGH);  
  digitalWrite(leftMotorBack, LOW);  
  analogWrite(leftMotorSpeed, speed);

  //Right motor back
  digitalWrite(rightMotorForward, LOW);    
  digitalWrite(rightMotorBack, HIGH); 
  analogWrite(rightMotorSpeed, speed);
}

//Stop both motors
void brake()
{
  digitalWrite(leftMotorForward, LOW);
  digitalWrite(leftMotorBack, LOW);
  digitalWrite(rightMotorForward, LOW);
  digitalWrite(rightMotorBack, LOW);
}

//Change LED light to have specified colors on
void change_led_color(int red, int green, int blue)
{
  //Initialize with all colors off
  digitalWrite(LED_R, LOW);
  digitalWrite(LED_G, LOW);
  digitalWrite(LED_B, LOW);
  if (red == ON)
  {
    digitalWrite(LED_R, HIGH);
  }
  if (green == ON)
  {
    digitalWrite(LED_G, HIGH);
  }
  if (blue == ON)
  {
    digitalWrite(LED_B, HIGH);
  }
}

//Flash LED colors for warning
void flash()
{
  change_led_color(ON, OFF, OFF);
  delay(500);
  change_led_color(OFF, ON, OFF);
  delay(500);
  change_led_color(OFF, OFF, ON);
  delay(500);
  change_led_color(OFF, OFF, OFF);
}

//Update distance from ultrasonic sensor
void update_distance()
{
  pinMode(trigPin, OUTPUT);
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  pinMode(echoPin, INPUT);

  //Get duration from sensor and convert to inches
  duration = pulseIn(echoPin, HIGH);
  distance = microseconds_to_inches(duration);

  //Print the current distance to the closest object
  Serial.print(distance);
  Serial.print(" inches");
  Serial.println();
  Serial.println("---------------");
  delay(100);
}

//Convert microseconds to inches for ultrasonic sensor
long microseconds_to_inches(long microseconds) {
  return microseconds / 74 / 2;
}

//Main loop to run
void loop()
{
  //Continuously update distance to closest object
  update_distance();

  //Move forward until distance is under 6 inches
  //Then stop movement and flash lights
  if (distance > 6) 
  {
    change_led_color(OFF, OFF, ON);
    forward(50);
  }
  else 
  {
    brake();
    flash();
  }
}
