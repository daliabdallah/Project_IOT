#include <Keypad.h>
#include <ESP32Servo.h>

int pos = 0;

Servo servo;

const byte ROWS = 4; //Four rows
const byte COLS = 4; //Four columns
String password ="3628";
int i =-1;
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {10, 9, 8, 7}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {6, 5, 4, 3}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

String v_passcode="";
int inches = 0;

int cm = 0;
int test;
int oldcm=0;

long readUltrasonicDistance(int triggerPin, int echoPin)
{
  pinMode(triggerPin, OUTPUT);  // Clear the trigger
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  // Sets the trigger pin to HIGH state for 10 microseconds
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  pinMode(echoPin, INPUT);
  // Reads the echo pin, and returns the sound wave travel time in microseconds
  return pulseIn(echoPin, HIGH);
}

void setup(){
  Serial.begin(9600);
  servo.attach(11);
  test=0;
  pinMode(2, OUTPUT);

}

void loop(){
  
    // measure the ping time in cm
  cm = 0.01723 * readUltrasonicDistance(13, 14);
  // convert to inches by dividing by 2.54
  
  oldcm=cm;
   if (cm <= 50){
      digitalWrite(2, HIGH);
      char key = keypad.getKey();
     if ((key !=NO_KEY)&& (test==0)&&(key=='#')){
       test=1;
     }
  
  if ((key != NO_KEY)&&(test==1)){
    Serial.print(key);
    i++;
    
    v_passcode = v_passcode + key;
    
    if(key=='A')
    {
      Serial.println("\nEnter Password");
        v_passcode="";
      
    }
    if (v_passcode[i]!=password[i]){
    Serial.println("\nAccess Denied");
          v_passcode="";
          i=-1;}
    
    if((key=='8')&&(i==3))
    {
      Serial.println("\nValidate the Password");
        Serial.print(v_passcode);
        
        if (v_passcode[i]==password[i])
        {
          Serial.println("\nAccess Granted");
          v_passcode="";
          i=-1; 
        }      
    }        
  }
   }else {
  digitalWrite(2, LOW);
     }
  delay(10);
 
}
