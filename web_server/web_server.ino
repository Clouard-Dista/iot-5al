// Adafruit_DHT library
#include <DHT.h>

// Declaration input pin
#define DHTPIN 5 //Temperature+Humidity//

// Initialisation capteur
#define DHTTYPE DHT11   // DHT 11
#define Digital_polution A0 //polution
DHT dht(DHTPIN, DHTTYPE);

const int but_pin = D5;
const int vert_pin = D7;
const int jaune_pin = D6;
const int rouge_pin = D0;

// WEB //
#include <FS.h> // pour le SPIFFS
const char * nomDeFichier = "/index.html";
// provient de https://github.com/esp8266/Arduino 
// télécharger et installer à la main la dernière version
#include <ESP8266WiFi.h>           

const byte maxHTTPLine = 100;    
char httpLine[maxHTTPLine + 1]; // +1 pour avoir la place du '\0'
const char* ssid = "Sonde"; // <<--- METTRE ICI VOTRE NOM RESEAU WIFI
const char* password = "12345678"; // <<--- METTRE ICI VOTRE MOT DE PASSE WIFI

const uint16_t HTTPPort = 80;
const byte maxURL = 50;
char urlRequest[maxURL + 1]; // +1 pour avoir la place du '\0'

WiFiServer serveurWeb(HTTPPort); // crée un serveur sur le port HTTP standard
// FIN WEB //
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


void getStateSensor(WiFiClient &cl, float &h , float &t, float &p){
  cl.print(h);
  cl.print(";");
  cl.print(t);
  cl.print(";");
  cl.println(p);
}

void testRequeteWeb(float &h ,float &t,float &p)
{
  boolean currentLineIsBlank = true;
  byte indexMessage = 0;
  boolean requeteHTTPRecue = false;
  
  char * ptrGET, *ptrEspace;

  WiFiClient client = serveurWeb.available();
  if (!client) return ; // pas de client connecté

  while (client.connected()) {
    if (client.available()) {
      // on lit toute la trame HTPP, ici sans se soucier de la reqête
      char c = client.read();

      if (c == '\n' && currentLineIsBlank) { // une requête HTTP se termine par une ligne vide
        // ON GENERE LA PAGE WEB
        // On envoie un en tête de réponse HTTP standard
        client.println(F("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n"));
          if(strstr(urlRequest, "/getStateSensor")){
            getStateSensor(client, h ,t, p);
          }
          else {
            if(SPIFFS.exists(nomDeFichier)){              
              File pageWeb = SPIFFS.open(nomDeFichier, "r");
              client.write(pageWeb);
              pageWeb.close();
            }else {
              Serial.println(F("Erreur de fichier"));
            }
          }
        // on sort du while et termine la requête
        break;
      }
      if (c == '\n') {
        currentLineIsBlank = true;

        httpLine[indexMessage] = '\0'; // on termine la ligne correctement (c-string)
        indexMessage = 0; // on se reprépre pour la prochaine ligne
        if (ptrGET = strstr(httpLine, "GET")) {
          // c'est la requête GET, la ligne continent "GET /URL HTTP/1.1", on extrait l'URL
          ptrEspace = strstr(ptrGET + 4, " ");
          *ptrEspace = '\0';
          strncpy(urlRequest, ptrGET + 4, maxURL);
          urlRequest[maxURL] = '\0'; // par précaution si URL trop longue
        }
      }
      else if (c != '\r') {
        currentLineIsBlank = false;
        if (indexMessage <= maxHTTPLine - 1) {
          httpLine[indexMessage++] =  c; // sinon on ignore le reste de la ligne
        }
      }
    } // end if available
  } // end while
  delay(1);
  client.stop(); // termine la connexion

}

void setup() {

  
  pinMode(but_pin, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
 
  // demare les mesures
  dht.begin();

  // WEB //
  Serial.begin(74880); // parce que mon Wemos et par défaut à peu près à cette vitesse, évite les caractères bizarre au boot
  Serial.println("\n\nTest SPIFFS\n");

  // on démarre le SPIFSS
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

  // on démarre le serveur
  serveurWeb.begin();
  printHTTPServerInfo();

  // FIN WEB //

}

void loop() {

  buttonToggleLed();
  delay(100);
  if(digitalRead(LED_BUILTIN)!=1{
  // if (digitalRead(LED_BUILTIN) != 1) {
    //humidity
    float h = dht.readHumidity();
    //temperature
    float t = dht.readTemperature();
    //pollution//Digital_polution/10 > 75 = aie
    float p = analogRead (Digital_polution)/10;
     
    // affichÃ© qu'il y a une erreur si une des valeur n'est pas prÃ©sente
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

    int lv =(p>75?1:0)+(t>30?1:0)+(h<20?1:0);
    digitalWrite(vert_pin,0);
    digitalWrite(jaune_pin,0);
    digitalWrite(rouge_pin,0);
    if(lv==1){
      digitalWrite(jaune_pin,1);
    }else if(lv>1){
      digitalWrite(rouge_pin,1);
    }else{
      digitalWrite(vert_pin,1);
    }
  }

  // WEB //
  testRequeteWeb(h, t, p);
}

void buttonToggleLed() {
  static bool old_but_state = 0;
  if (digitalRead(but_pin) == 1 && old_but_state == 0) {
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
    delay(200);
  }

  //Serial.println(digitalRead(but_pin));
  old_but_state = digitalRead(but_pin);
}
