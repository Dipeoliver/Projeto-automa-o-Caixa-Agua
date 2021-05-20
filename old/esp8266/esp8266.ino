#include <ESP8266WiFi.h>

const char* ssid = "Aula_AP";
const char* password = "12345678";
WiFiServer server(80);

int Caixa1Ligada = 0;
int Caixa2Ligada = 0;

void setup() {

  pinMode(4, OUTPUT); // valvula 1
  pinMode(14, OUTPUT); // valvula 2
  pinMode(12, OUTPUT); // motor

  digitalWrite(4, LOW);
  digitalWrite(12, LOW);
  digitalWrite(14, LOW);

  Serial.begin(9600);

  IPAddress staticIP(192, 168, 4, 2);
  IPAddress gateway(192, 168, 4, 1);
  IPAddress subnet(255, 255, 255, 0);

  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid, password, 2, 0);
  WiFi.config(staticIP, gateway, subnet);

  server.begin();
  Serial.println("Server started");
  Serial.println(WiFi.softAPIP());

}

void loop() {
  // fica no loop enquanto nao tiver ninguem conectado
  WiFiClient client = server.available();
  if (!client) {
    return;
  }

  // fica no loop enquanto niguem enviar nada
  while (!client.available()) {
    delay(1);
  }

  String req = client.readStringUntil('\r');
  req = req.substring(req.indexOf("/") + 1, req.indexOf ("HTTP") - 1);
  Serial.println(req);
  client.flush();
  // -------------------------------------------
  if (req.indexOf("CAIXA_1_ON") != -1)
  {
    Caixa1Ligada = 1;
    digitalWrite(4, HIGH);
    delay(3000);
    digitalWrite(12, HIGH);
    client.print("CAIXA 1 ON");
  }
  else if (req.indexOf("CAIXA_1_OFF") != -1)
  {
    Caixa1Ligada = 0;
    if (Caixa2Ligada == 0 ) // se a caixa 2 estiver ligada nao desliga motor
    {
      digitalWrite(12, LOW);
      delay(3000);
    }
    digitalWrite(4, LOW);
    client.print("CAIXA 1 OFF");
  }

  // -------------------------------------------
  if (req.indexOf("CAIXA_2_ON") != -1)
  {
    // abrir valvula 1 e ligar bomba
    Caixa2Ligada = 1;
    digitalWrite(14, HIGH);
    delay(3000);
    digitalWrite(12, HIGH);
    client.print("CAIXA 2 ON");
  }
  else if (req.indexOf("CAIXA_2_OFF") != -1)
  {
    Caixa2Ligada = 0;
    if (Caixa1Ligada == 0) { // se a caixa 1 estiver ligada nao desliga motor
      digitalWrite(12, LOW);
      delay(3000);
    }
    digitalWrite(14, LOW);

    client.print("CAIXA 2 OFF");
  }
  // -------------------------------------------


  else
  {
    client.flush();
    client.stop();
    return;
  }

  client.print(" HTTP/1.1 200 OK\n\n");
  client.flush();


}
