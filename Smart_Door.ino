#include <WiFi.h>
#include <WebServer.h>
#include <Keypad.h>
#include <ESP32Servo.h>

const char* ssid = ".........";
const char* password = "............";
WebServer server(80);

int Door = 11;
bool etatDoor = 0;
bool etatPassword=0;
String msg;
char texteEtatPassword[2][100] = {"Correct!", "Incorrect!"};
char texteEtatDoor[2][10] = {"Open!", "Closed!"};

//------------------------------------------------


int pos = 0;

Servo servo;

const byte ROWS = 4; //Four rows
const byte COLS = 4; //Four columns
String code ="3628";
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


//-----------------------------------------------

void handleRoot()
{
  String page = "<!DOCTYPE html>";

  page += "<html lang='fr'>";

  page += "<head>";
  page += "    <title>Serveur ESP32</title>";
  page += "    <meta http-equiv='refresh' content='60' name='viewport' content='width=device-width, initial-scale=1' charset='UTF-8' />";
  page += "    <link rel='stylesheet' href='https://www.w3schools.com/w3css/4/w3.css'>";
  page += "</head>";

  page += "<body>";
  page += "    <div class='w3-card w3-blue w3-padding-small w3-jumbo w3-center'>";
  page += "        <p>Password: "; page += texteEtatPassword[etatPassword]; + "</p>";
  page += "        <p>DOOR STATE: "; page += texteEtatDoor[etatDoor]; + "</p>";
  page += "    </div>";

  page += "    <div class='w3-bar'>";
  page += "        <a href='/Open' class='w3-bar-item w3-button w3-border w3-jumbo' style='width:50%; height:50%;'>Open</a>";
  page += "        <a href='/Closed' class='w3-bar-item w3-button w3-border w3-jumbo' style='width:50%; height:50%;'>Closed</a>";
  page += "    </div>";

  page += "    <div class='w3-center w3-padding-16'>";
  page += "        <p>Mini_Project :Smart Door</p>";
  page += "        <i>Created by : <br> Mazen Boujezza <br> Mohamed Ali Abdallah</i>";
  page += "    </div>";

  page += "</body>";

  page += "</html>";

  server.setContentLength(page.length());
  server.send(200, "text/html", page);
}

void handleOpen()
{
  etatDoor = 0;
  servo.write(90);
  server.sendHeader("Location", "/");
  server.send(303);
}

void handlePass(){
  etatPassword = 0;
  }
void handleInco(){
  etatPassword =1 ;
  }

void handleClosed()
{
  etatDoor = 1;
  servo.write(0);
  server.sendHeader("Location", "/");
  server.send(303);
}

void handleNotFound()
{
  server.send(404, "text/plain", "404: Not found");
}

void setup()
{
  Serial.begin(115200);
  delay(1000);
  //-------------------------
  servo.attach(Door);
  test=0;
  pinMode(2, OUTPUT);
  Serial.println("\n");
//--------------------------------
  pinMode(Door, OUTPUT);
  digitalWrite(Door, LOW);

  WiFi.persistent(false);
  WiFi.begin(ssid, password);
  Serial.print("Tentative de connexion...");

  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(100);
  }

  Serial.println("\n");
  Serial.println("Connexion etablie!");
  Serial.print("Adresse IP: ");
  Serial.println(WiFi.localIP());
//------------------------------
//---------------------------------
  server.on("/", handleRoot);
  server.on("/Open", handleOpen);
  server.on("/Closed", handleClosed);
  server.onNotFound(handleNotFound);
  server.begin();

  Serial.println("Serveur web actif!");
}

void loop()
{
  //---------------------------------------------------------
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
    if (v_passcode[i]!=code[i]){
    Serial.println("\nAccess Denied");
          v_passcode="";
          handleInco();
          server.sendHeader("Location", "/");
          i=-1;}
    if((key=='8')&&(i==3))
    {
      Serial.println("\nValidate the Password");
        Serial.print(v_passcode);
        msg=="correct";
        
        if (v_passcode[i]==code[i])
        {
          Serial.println("\nAccess Granted");
          v_passcode="";
          handlePass();
          server.sendHeader("Location", "/");
          i=-1; 
        }      
    }        
  }
   }else {
  digitalWrite(2, LOW);
     }
  delay(10);
 //----------------------------------------------------------
  
  server.handleClient();
}
