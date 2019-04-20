// Adafruit_DHT library
#include <DHT.h>
//Include the SSL client
#include <WiFiClientSecure.h>
// Declaration input pin
#define DHTPIN 5 //Temperature+Humidity//
#define but_pin D5
#define power_pin D3
#define vert_pin D7
#define jaune_pin D6
#define rouge_pin D0

#define INTERVAL_MESSAGE 10000
// Init sensor
#define DHTTYPE DHT11   // DHT 11&
#define Digital_polution A0 //polution
DHT dht(DHTPIN, DHTTYPE);

// WEB //
#include <FS.h> // for SPIFFS
const char * nomDeFichier = "/index.html";
// see : https://github.com/esp8266/Arduino , download and install the latest version
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

const byte maxHTTPLine = 100;
char httpLine[maxHTTPLine + 1]; // +1 for '\0'
const char* ssid = "Sonde"; // <<--- INSERT YOUR SSID HERE
const char* password = "12345678"; // <<--- INSERT YOUR WIFI ROOTER PASSWORD HERE

const uint16_t HTTPPort = 80;
const byte maxURL = 50;
char urlRequest[maxURL + 1]; // +1 for '\0'
long timeThresold = 0;
WiFiClientSecure clientSecure;
WiFiServer serveurWeb(HTTPPort); // create http server on standard port

void printHTTPServerInfo()
{
  Serial.print(F("Site web http://"));
  Serial.print(WiFi.localIP());
  if (HTTPPort != 80) {
    Serial.print(F(":"));
    Serial.print(HTTPPort);
  }
  Serial.println();
}

void buttonToggleLed() {
  static bool old_but_state = 0;
  if (digitalRead(but_pin) == 1 && old_but_state == 0) {
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));

    digitalWrite(power_pin, !digitalRead(LED_BUILTIN));
    digitalWrite(jaune_pin, LOW);
    digitalWrite(rouge_pin, LOW);
    digitalWrite(vert_pin, LOW);

    delay(200);
  }

  //Serial.println(digitalRead(but_pin));
  old_but_state = digitalRead(but_pin);
}

void getStateSensor(WiFiClient &cl, float &h , float &t, float &p) {
  cl.print(h);
  cl.print(";");
  cl.print(t);
  cl.print(";");
  cl.println(p);
}

void testRequeteWeb(float &h , float &t, float &p)
{
  boolean currentLineIsBlank = true;
  byte indexMessage = 0;
  boolean requeteHTTPRecue = false;

  char * ptrGET, *ptrEspace;

  WiFiClient client = serveurWeb.available();
  if (!client) return ; // no client available

  while (client.connected()) {
    if (client.available()) {
      // read all http trame, without check the request
      char c = client.read();

      if (c == '\n' && currentLineIsBlank) { // une requête HTTP se termine par une ligne vide
        // generate web page
        // send standard http header  
        client.println(F("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n"));
        if (strstr(urlRequest, "/getStateSensor")) {
          getStateSensor(client, h , t, p);
        }
        else {
          if (SPIFFS.exists(nomDeFichier)) {
            File pageWeb = SPIFFS.open(nomDeFichier, "r");
            client.write(pageWeb);
            pageWeb.close();
          } else {
            Serial.println(F("Erreur de fichier"));
          }
        }
        // request ended
        break;
      }
      if (c == '\n') {
        currentLineIsBlank = true;

        httpLine[indexMessage] = '\0'; // close line properly 
        indexMessage = 0; // prepare for the next line 
        if (ptrGET = strstr(httpLine, "GET")) {
          // GET request, line contains "GET /URL HTTP/1.1"
          ptrEspace = strstr(ptrGET + 4, " ");
          *ptrEspace = '\0';
          strncpy(urlRequest, ptrGET + 4, maxURL);
          urlRequest[maxURL] = '\0'; // if URL was too longer
        }
      }
      else if (c != '\r') {
        currentLineIsBlank = false;
        if (indexMessage <= maxHTTPLine - 1) {
          httpLine[indexMessage++] =  c; 
        }
      }
    } // end if available
  } // end while
  delay(1);
  client.stop(); // close connection 
}

void setup() {

  pinMode(power_pin, OUTPUT);
  pinMode(vert_pin, OUTPUT);
  pinMode(jaune_pin, OUTPUT);
  pinMode(rouge_pin, OUTPUT);
  pinMode(but_pin, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);

 
  digitalWrite(power_pin, HIGH);
  // start sensor monitoring
  dht.begin();

  Serial.println("\n\nTest SPIFFS\n");

  // startSPIFSS
  if (!SPIFFS.begin()) {
    Serial.println("erreur SPIFFS");
    while (true); // on ne va pas plus loin
  }

  WiFi.begin(ssid, password);

  Serial.println();
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.write('.');
  }
  Serial.println();

  // start web server
  serveurWeb.begin();
  printHTTPServerInfo();

 

}

void loop() {
  HTTPClient http;
  char str[256];
  int httpCode;

  // Serial.println("In loop start ...");
  char* user = "X";
  char* pass = "X";

  char string_h[100];
  char string_t[100];
  char string_p[100];

  buttonToggleLed();
  delay(100);

  float h = 0;
  float t = 0;
  float p = 0;

  if (digitalRead(LED_BUILTIN) != 1) {

    //humidity
    h = dht.readHumidity();
    //temperature
    t = dht.readTemperature();
    //pollution//Digital_polution/10 > 75 
    p = analogRead (Digital_polution) / 10;

    // Show an error, if value is missing 
    if (isnan(h) || isnan(t)) {
      Serial.println("Error while reading the sensor");
      return;
    }

    //Serial.print("Humidity(%): ");
    Serial.print(h);
    Serial.print(" ");

    //Serial.print("temperature(C): ");
    Serial.print(t);
    Serial.print(" ");

    //Serial.print("polution: ");
    Serial.println(p);

    int lv = (p > 75 ? 1 : 0) + (t > 30 ? 1 : 0) + (h < 20 ? 1 : 0);
    Serial.print("LV :");
    Serial.println(lv);
    digitalWrite(jaune_pin, LOW);
    digitalWrite(rouge_pin, LOW);
    digitalWrite(vert_pin, LOW);

    if (lv == 1) {
      digitalWrite(jaune_pin, 1);
      digitalWrite(jaune_pin, HIGH);
      
    } else if (lv > 1) {
      digitalWrite(rouge_pin, HIGH);      
    } else {
      digitalWrite(vert_pin, HIGH);      
    }

    // WEB //
    testRequeteWeb(h, t, p);
  }
}
