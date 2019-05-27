#include <Arduino.h>
#include <WiFi.h>

const char* ssid     = "ALIENWARE-3WF8362";
const char* password = "123456789";

WiFiServer server(80);
String header;
String output26State = "off";
String output27State = "off";
const int output0 = 0;
const int output26 = 26;
const int output27 = 27;

void setup()
{
  pinMode(output0, INPUT_PULLUP);
  pinMode(output26, OUTPUT);
  pinMode(output27, OUTPUT);
  digitalWrite(output26, LOW);
  digitalWrite(output27, LOW);
  connectWiFi();
}

void loop()
{
  int button0 = digitalRead(output0);
  int button26 = digitalRead(output26);
  int button27 = digitalRead(output27);

  WiFiClient client = server.available();

  if(client)
  {
    Serial.println("New Client.");
    String currentLine = "";
    while(client.connected())
    {
      if(client.available())
      {
        char new_byte = client.read();
        Serial.write(new_byte);
        if(new_byte == '\n')
        {
          if(currentLine.length() == 0)
          {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            if(output26State == "off")
            {
              client.print("Click <a href=\"/26/on\">here</a> to toggle the LED 26 on/off.<br>");
            }
            else
            {
              client.print("Click <a href=\"/26/off\">here</a> to toggle the LED 26 on/off.<br>");
            }

            if(output27State == "off")
            {
              client.println("Click <a href=\"/27/on\">here</a> to toggle the LED 27 on/off.<br>");
            }
            else
            {
              client.println("Click <a href=\"/27/off\">here</a> to toggle the LED 27 on/off.<br>");
            }
            client.println();
            break;
          }
          else
          {
            currentLine = "";
          }
        }
        else if (new_byte != '\r')
        {
          currentLine += new_byte;
        }
        if((currentLine.endsWith("GET /26/on")))
        {
          Serial.println("GPIO 26 on\n");
          output26State = "on";
          digitalWrite(output26, HIGH);
        }
        else if((currentLine.endsWith("GET /26/off")))
        {
          Serial.println("GPIO 26 off\n");
          output26State = "off";
          digitalWrite(output26, LOW);
        }
        else if((currentLine.endsWith("GET /27/on")))
        {
          Serial.println("GPIO 27 on\n");
          output27State = "on";
          digitalWrite(output27, HIGH);
        }
        else if(currentLine.endsWith(("GET /27/off")))
        {
          Serial.println("GPIO 27 off\n");
          output27State = "off";
          digitalWrite(output27, LOW);
        }
      }
    }
        Serial.println(button0);
        Serial.println(button26);
        Serial.println(button27);
        Serial.println();
    client.stop();
    Serial.println("Client disconnected.\n");
  }
}

void connectWiFi(void)
{
  Serial.begin(115200);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println("Press button 0 to turn on/off both LEDs");
  server.begin();
}
