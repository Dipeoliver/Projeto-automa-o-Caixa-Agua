// Libraries Needed For This Project
//------------------------------------------------------------------------------------
#include <EEPROM.h>         // To Be Able To Store & Read Data Between Power Off
#include <ESP8266WiFi.h>    // The Basic Function Of The ESP NOD MCU
//------------------------------------------------------------------------------------
// Defining I/O Pins
//------------------------------------------------------------------------------------
#define       LED0           D4         // WIFI Module LED

#define       VALVE1         D0        // Indicate Connectivity With Client #1
#define       LEDVALVE1      D5        // Indicate Connectivity With Client #1
#define       VALVE2         D1        // Indicate Connectivity With Client #1
#define       LEDVALVE2      D6        // Indicate Connectivity With Client #1
#define       exhaust_valve  D2        // Indicate Connectivity With Client #1  
//------------------------------------------------------------------------------------
// BUTTON Variables
//------------------------------------------------------------------------------------
int           Valve1State;
int           LastValve1State   = LOW;
int           LastDebounceValve1Time  = 0;

int           Valve2State;
int           LastValve2State   = LOW;
int           LastDebounceValve2Time  = 0;

int           exhaust_valveState;
int           Lastexhaust_valveState   = LOW;
int           LastDebounceexhaust_valveTime  = 0;

int           DebounceDelay     = 50;
const String  ClientType       = "Cliente 01";

//------------------------------------------------------------------------------------
// LED Delay Variables
//------------------------------------------------------------------------------------
int    LEDStateVALVE1    = LOW;
int    LEDStateVALVE2    = LOW;
int    LEDStateexhaust_valve     = LOW;

unsigned long CurrMillis = 0;
unsigned long PrevMillis = 0;
unsigned long Interval   = 1000;
//------------------------------------------------------------------------------------
// WIFI Authentication Variables
//------------------------------------------------------------------------------------
/* This Client Is Going To Connect To A WIFI Server Access Point
   So You Have To Specify Server WIFI SSID & Password In The Code Not Here
   Please See The Function Below Name (WiFi.Begin)
   If WIFI dont need Any password Then WiFi.begin("SSIDNAME")
   If WIFI needs a Password Then WiFi.begin("SSIDNAME", "PASSWORD")
*/
char*         ESPssid;            // Wifi Name
char*         ESPpassword;        // Wifi Password
//------------------------------------------------------------------------------------
// WIFI Module Role & Port
//------------------------------------------------------------------------------------
/* This WIFI Module Code Works As A Client
   That Will Connect To A Server WIFI Modul With (IP ADDress 192.168.4.1)
*/
int             ESPServerPort  = 9001;
IPAddress       ESPServer(192, 168, 4, 1);
WiFiClient      ESPClient;
//====================================================================================

void setup()
{
  // Setting The Serial Port ----------------------------------------------
  Serial.begin(115200);           // Computer Communication

  // Setting The Mode Of Pins ---------------------------------------------
  pinMode(LED0, OUTPUT);          // WIFI OnBoard LED Light

  pinMode(VALVE1, INPUT_PULLUP);          // Indicator For Valve #1 Connectivity
  pinMode(LEDVALVE1, OUTPUT);
  pinMode(VALVE2, INPUT_PULLUP);          // Indicator For Valve #2 Connectivity
  pinMode(LEDVALVE2, OUTPUT);
  pinMode(exhaust_valve, INPUT_PULLUP);

  digitalWrite(LED0, !LOW);       // Turn WiFi LED Off

  // Print Message Of I/O Setting Progress --------------------------------
  Serial.println("\nI/O Pins Modes Set .... Done");

  // Starting To Connect --------------------------------------------------
  if (WiFi.status() == WL_CONNECTED)
  {
    WiFi.disconnect();
    WiFi.mode(WIFI_OFF);
    delay(50);
  }

  /* in this part it should load the ssid and password
     from eeprom they try to connect using them */

  WiFi.mode(WIFI_STA);                // To Avoid Broadcasting An SSID
  WiFi.begin("Water_tank", "12345678L");      // The SSID That We Want To Connect To

  // Printing Message For User That Connetion Is On Process ---------------
  Serial.println("!--- Connecting To " + WiFi.SSID() + " ---!");

  // WiFi Connectivity ----------------------------------------------------
  CheckWiFiConnectivity();        // Checking For Connection

  // Stop Blinking To Indicate Connected ----------------------------------
  digitalWrite(LED0, !HIGH);
  Serial.println("!-- Client Device Connected --!");

  // Printing IP Address --------------------------------------------------
  Serial.println("Connected To      : " + String(WiFi.SSID()));
  Serial.println("Signal Strenght   : " + String(WiFi.RSSI()) + " dBm");
  Serial.print  ("Server IP Address : ");
  Serial.println(ESPServer);
  Serial.print  ("Server Port Num   : ");
  Serial.println(ESPServerPort);
  // Printing MAC Address
  Serial.print  ("Device MC Address : ");
  Serial.println(String(WiFi.macAddress()));
  // Printing IP Address
  Serial.print  ("Device IP Address : ");
  Serial.println(WiFi.localIP());

  // Conecting The Device As A Client -------------------------------------
  ESPRequest();
}

//====================================================================================

void loop()
{
  ReadSensor1();
  ReadSensor2();
  Readexhaust_valve();
}

//====================================================================================

void ReadSensor1()
{
  // Reading The Button
  int reading = digitalRead(VALVE1);

  // If It Doest Match The Previous State
  if (reading != LastValve1State)
  {
    LastDebounceValve1Time = millis();
  }

  // To Iliminate Debounce
  if ((millis() - LastDebounceValve1Time) > DebounceDelay)
  {
    if (reading != Valve1State)
    {
      Valve1State = reading;

      if (Valve1State == LOW)
      {
        //LEDStateVALVE1    = !digitalRead(VALVE1);
        //digitalWrite(VALVE1, LEDStateVALVE1);
        //Serial.println   ("<" + ClientType + "-TANK_1_" + LEDStateVALVE1 + ">");
        //ESPClient.println("<" + ClientType + "-TANK_1_" + LEDStateVALVE1 + ">");
        digitalWrite(LEDVALVE1, HIGH);
        Serial.println   ("<" + ClientType + "-TANK_1_1"">");
        ESPClient.println("<" + ClientType + "-TANK_1_1"">");
        ESPClient.flush();
      }
    }
  }

  // Last Button State Concidered
  LastValve1State = reading;
}
//---------------------------------------------------------------------------------------
void ReadSensor2()
{
  // Reading The Button
  int reading = digitalRead(VALVE2);

  // If It Doest Match The Previous State
  if (reading != LastValve2State)
  {
    LastDebounceValve2Time = millis();
  }

  // To Iliminate Debounce
  if ((millis() - LastDebounceValve2Time) > DebounceDelay)
  {
    if (reading != Valve2State)
    {
      Valve2State = reading;

      if (Valve2State == LOW)
      {
        LEDStateVALVE2    = !digitalRead(VALVE2);
        //digitalWrite(VALVE2, LEDStateVALVE2);
        //Serial.println   ("<" + ClientType + "-TANK_2_" + LEDStateVALVE2 + ">");
        //ESPClient.println("<" + ClientType + "-TANK_2_" + LEDStateVALVE2 + ">");
        digitalWrite(LEDVALVE1, LOW);
        Serial.println   ("<" + ClientType + "-TANK_1_0"">");
        ESPClient.println("<" + ClientType + "-TANK_1_0"">");
        ESPClient.flush();
      }
    }
  }

  // Last Button State Concidered
  LastValve2State = reading;
}
//---------------------------------------------------------------------------------------
void Readexhaust_valve()
{
  // Reading The Button
  int reading = digitalRead(exhaust_valve);

  // If It Doest Match The Previous State
  if (reading != Lastexhaust_valveState)
  {
    LastDebounceexhaust_valveTime = millis();
  }

  // To Iliminate Debounce
  if ((millis() - LastDebounceexhaust_valveTime) > DebounceDelay)
  {
    if (reading != exhaust_valveState)
    {
      exhaust_valveState = reading;

      if (exhaust_valveState == LOW)
      {
        LEDStateexhaust_valve    = !digitalRead(exhaust_valve);
        digitalWrite(exhaust_valve, LEDStateexhaust_valve);
        Serial.println   ("<" + ClientType + "-exhaust_valve_" + LEDStateexhaust_valve + ">");
        ESPClient.println("<" + ClientType + "-exhaust_valve_" + LEDStateexhaust_valve + ">");
        ESPClient.flush();
      }
    }
  }
  // Last Button State Concidered
  Lastexhaust_valveState = reading;
}

//====================================================================================

void CheckWiFiConnectivity()
{
  while (WiFi.status() != WL_CONNECTED)
  {
    for (int i = 0; i < 10; i++)
    {
      digitalWrite(LED0, !HIGH);
      delay(250);
      digitalWrite(LED0, !LOW);
      delay(250);
      Serial.print(".");
    }
    Serial.println("");
  }
}

void ESPRequest()
{
  // First Make Sure You Got Disconnected
  ESPClient.stop();

  // If Sucessfully Connected Send Connection Message
  if (ESPClient.connect(ESPServer, ESPServerPort))
  {
    Serial.println    ("<" + ClientType + "- CONNECTED>");
    ESPClient.println ("<" + ClientType + "- CONNECTED>");
  }
}

//====================================================================================
