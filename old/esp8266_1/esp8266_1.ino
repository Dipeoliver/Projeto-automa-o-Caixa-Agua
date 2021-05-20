#include <ESP8266WiFi.h>

#define SendKey 0  //Button to send data Flash BTN on NodeMCU

const int inputPin = 16;

int port = 80;  //Port number
WiFiServer server(port);

//Server connect to WiFi Network
const char *ssid = "Aula_AP";  //Enter your wifi SSID
const char *password = "12345678";  //Enter your wifi Password

WiFiClient client = server.available();

int count = 0;
//=======================================================================
//                    Power on setup
//=======================================================================
void setup()
{
  pinMode(inputPin, INPUT);



  Serial.begin(9600);
  pinMode(SendKey, INPUT_PULLUP); //Btn to send data
  Serial.println();

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password); //Connect to wifi

  // Wait for connection
  Serial.println("Connecting to Wifi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    delay(500);
  }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);

  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
  Serial.print("Open Telnet and connect to IP:");
  Serial.print(WiFi.localIP());
  Serial.print(" on port ");
  Serial.println(port);


  if (client) {
    if (client.connected())
    {
      Serial.println("Client Connected");
    }
    else
    {
      return;
    }
  }
}
//=======================================================================
//                    Loop
//=======================================================================

void loop()
{
  int val = digitalRead(inputPin); // le o valor na entrada
  if (val == LOW) // se valor está em zero( tecla pressionada)
  {
    Serial.println("Baixo");
    Serial.print(String("GET ") + "192.168.4.1" + "/CAIXA_1_ON"+"\r\n");
    delay(3000);
    client.print(String("GET ") + "192.168.4.1" + "/CAIXA_1_ON"+"\r\n");
  }
}
