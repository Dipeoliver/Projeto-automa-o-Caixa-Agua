//-- Libraries Included --------------------------------------------------------------
#include <ESP8266WiFi.h>
//------------------------------------------------------------------------------------
// Define I/O Pins
#define LED0 2 // WIFI Module LED

#define VALVE1 D0    // Connectivity With Valve1   #1
#define LEDVALVE1 D1 // Connectivity With LEDValve #1
#define VALVE2 D2    // Connectivity With Valve    #2
#define LEDVALVE2 D3 // Connectivity With LEDValve #2
#define MOTOR 3     // Connectivity With Motor
#define LEDMOTOR D5  // Connectivity With LEDMotor

#define CLIENT1 D6 // Connectivity With Client #1
#define CLIENT2 D7 // Connectivity With Client #2

#define BUTTON D8 // Connectivity ReInitiate Button
//------------------------------------------------------------------------------------
// Authentication Variables
char* ESPssid = "Water_tank";    // SERVER WIFI NAME
char* ESPpassword = "12345678L"; // SERVER PASSWORD
String Message;                  // VARIABLE RECEIVE DATA FROM OTHER CLIENTS
int Tank1ON = 0;                 // VARIABLE TO CONTROL THE TANK 1
int Tank2ON = 0;                 // VARIABLE TO CONTROL THE TANK 2
//------------------------------------------------------------------------------------
#define MAXSC 4 // MAXIMUM NUMBER OF CLIENTS

WiFiServer ESPServer(9001);  // THE SERVER AND THE PORT NUMBER
WiFiClient ESPClient[MAXSC]; // THE SERVER CLIENTS (Devices)
//====================================================================================

void setup()
{
  // Setting The Serial Port
  Serial.begin(115200); // Computer Communication

  // Setting The Mode Of Pins
  pinMode(LED0, OUTPUT);      // WIFI OnBoard LED Light
  pinMode(VALVE1, OUTPUT);    // Indicator For Client  Connectivity
  pinMode(LEDVALVE1, OUTPUT); // Indicator For Client  Connectivity
  pinMode(VALVE2, OUTPUT);    // Indicator For Client  Connectivity
  pinMode(LEDVALVE2, OUTPUT); // Indicator For Client  Connectivity
  pinMode(MOTOR, OUTPUT);     // Indicator For Client  Connectivity
  pinMode(LEDMOTOR, OUTPUT);  // Indicator For Client  Connectivity
  pinMode(CLIENT1, OUTPUT);   // Indicator For Client  Connectivity
  pinMode(CLIENT2, OUTPUT);   // Indicator For Client  Connectivity

  pinMode(BUTTON, INPUT_PULLUP); // Initiate Connectivity

  // Print Message Of I/O Setting Progress
  Serial.println();
  Serial.println("I/O Pins Modes Set .... Done");

  // Setting Up A Wifi Access Point
  SetWifi(ESPssid, ESPpassword);
}

//====================================================================================

void loop()
{
  AvailableClients(); // Checking For Available Clients
  AvailableMessage(); // Checking For Available Client Messages
}

//====================================================================================

void SetWifi(char* Name, char* Password)
{
  // Stop Any Previous WIFI
  WiFi.disconnect();

  // Setting The Wifi Mode
  WiFi.mode(WIFI_AP_STA);
  Serial.println("WIFI Mode : AccessPoint Station");

  // Setting The AccessPoint Name & Password
  ESPssid = Name;
  ESPpassword = Password;

  // Starting The Access Point
  WiFi.softAP(ESPssid, ESPpassword);
  Serial.println("WIFI < " + String(ESPssid) + " > ... Started");

  // Wait For Few Seconds
  delay(500);

  // Getting Server IP
  IPAddress IP = WiFi.softAPIP();

  // Printing The Server IP Address
  Serial.print("AccessPoint IP : ");
  Serial.println(IP);

  // Printing MAC Address
  Serial.print("AccessPoint MC : ");
  Serial.println(String(WiFi.softAPmacAddress()));

  // Starting Server
  ESPServer.begin();
  ESPServer.setNoDelay(true);
  Serial.println("Server Started");
}

//====================================================================================

void AvailableClients()
{
  if (ESPServer.hasClient())
  {
    // Read LED0 Switch To Low If High.
    if (digitalRead(LED0) == HIGH)
      digitalWrite(LED0, LOW);

    // Light Up VALVE1
    //digitalWrite(VALVE1, HIGH);
    // ------------------------------------------------
    // LED indicator client connect
    if (ESPClient[1].connected())
    {
      digitalWrite(CLIENT1, HIGH);
    }
    else
    {
      digitalWrite(CLIENT1, LOW);
    }

    if (ESPClient[2].connected())
    {
      digitalWrite(CLIENT2, HIGH);
    }
    else
    {
      digitalWrite(CLIENT2, LOW);
    }
    // ------------------------------------------------
    for (uint8_t i = 0; i < MAXSC; i++)
    {
      //find free/disconnected spot
      if (!ESPClient[i] || !ESPClient[i].connected())
      {
        // Checks If Previously The Client Is Taken
        if (ESPClient[i])
        {
          ESPClient[i].stop();
        }

        // Checks If Clients Connected To The Server
        if (ESPClient[i] = ESPServer.available())
        {
          Serial.println("New Client: " + String(i + 1));
        }

        // Continue Scanning
        continue;
      }
    }
    //no free/disconnected spot so reject
    WiFiClient ESPClient = ESPServer.available();
    ESPClient.stop();
  }
  else
  {
    // This LED Blinks If No Clients Where Available
    digitalWrite(LED0, HIGH);
    delay(250);
    digitalWrite(LED0, LOW);
    delay(250);
  }
}

//====================================================================================

void AvailableMessage()
{
  //check clients for data
  for (uint8_t i = 0; i < MAXSC; i++)
  {
    if (ESPClient[i] && ESPClient[i].connected() && ESPClient[i].available())
    {
      while (ESPClient[i].available())
      {
        Message = ESPClient[i].readStringUntil('\r');
        ESPClient[i].flush();
        ClientNumber();
        Serial.println("Client No " + String(i + 1) + " - " + Message);
      }
    }
  }
}

// =================================================================================

void ClientNumber()
{

  // AJUSTAR PARA ENTRAR DENTRO DO if
  if (Message == "<Cliente 01-exhaust_valve_1>" || Message == "<Cliente 02-exhaust_valve_1>")
  {
    Serial.print("Ladrão acionado");
  }
  else if (Message == "<Cliente 01-exhaust_valve_0>" || Message == "<Cliente 02-exhaust_valve_0>")
  {
    Serial.print("Ladrão Desligado");
  }
  else
  {

    if (Message == "<Cliente 01-TANK_1_1>")
    { // Check clent number, 01 = client 1, and after trace, status 1 = on, 0 = off
      Tank1ON = 1;
      digitalWrite(VALVE1, HIGH);
      digitalWrite(LEDVALVE1, HIGH);
      delay(3000);
      digitalWrite(MOTOR, HIGH);
      digitalWrite(LEDMOTOR, HIGH);
      Serial.print("Tank 1 ligado");
    }

    if (Message == "<Cliente 01-TANK_1_0>")
    {
      Tank1ON = 0;
      if (Tank2ON == 0)
      {
        digitalWrite(MOTOR, LOW);
        digitalWrite(LEDMOTOR, LOW);
        delay(3000);
      }
      digitalWrite(VALVE1, LOW);
      digitalWrite(LEDVALVE1, LOW);
      Serial.print("Tank 1 desligado");
    }
    //------------------------------------------------
    if (Message == "<Cliente 02-TANK_2_1>")
    {
      Tank2ON = 1;
      digitalWrite(VALVE2, HIGH);
      digitalWrite(LEDVALVE2, HIGH);
      delay(3000);
      digitalWrite(MOTOR, HIGH);
      digitalWrite(LEDMOTOR, HIGH);
      Serial.print("Tank 2 ligado");
    }
    if (Message == "<Cliente 02-TANK_2_0>")
    {
      Tank2ON = 0;
      if (Tank1ON == 0)
      {
        digitalWrite(MOTOR, LOW);
        digitalWrite(LEDMOTOR, LOW);
        delay(3000);
      }
      digitalWrite(VALVE2, LOW);
      digitalWrite(LEDVALVE2, LOW);
      Serial.print("Tank 2 desligado");
    }
    //------------------------------------------------
  }
}
