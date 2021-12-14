#include <WiFi.h>
#include <WebServer.h>
#include <Keypad.h>
#include <ESP32Servo.h>

const char* ssid = ".........";
const char* password = "............";
WebServer server(80);

int Door = 2;
bool etatDoor = 0;
bool etatPassword=0;
String msg;
char texteEtatPassword[2][100] = {"Correct!", "Incorrect!"};
char texteEtatDoor[2][10] = {"Open!", "Closed!"};

//------------------------------------------------




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
  page += "        <a href='/on' class='w3-bar-item w3-button w3-border w3-jumbo' style='width:50%; height:50%;'>Open</a>";
  page += "        <a href='/off' class='w3-bar-item w3-button w3-border w3-jumbo' style='width:50%; height:50%;'>Closed</a>";
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
  digitalWrite(Door, HIGH);
  server.sendHeader("Location", "/");
  server.send(303);
}

void handleClosed()
{
  etatDoor = 1;
  digitalWrite(Door, LOW);
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
  Serial.println("\n");

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

  server.on("/", handleRoot);
  server.on("/Open", handleOpen);
  server.on("/Closed", handleClosed);
  server.onNotFound(handleNotFound);
  server.begin();

  Serial.println("Serveur web actif!");
}

void loop()
{
  if(msg=="correct"){
      handlePass();
    }else{
      handleInco();
      }
  server.handleClient();
}
