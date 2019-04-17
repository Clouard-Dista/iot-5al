#include <FS.h> // pour le SPIFFS
const char * nomDeFichier = "/index.html";
// provient de https://github.com/esp8266/Arduino 
// télécharger et installer à la main la dernière version
#include <ESP8266WiFi.h>               

const char* ssid = "Sonde"; // <<--- METTRE ICI VOTRE NOM RESEAU WIFI
const char* password = "12345678"; // <<--- METTRE ICI VOTRE MOT DE PASSE WIFI

const uint16_t HTTPPort = 80;
WiFiServer serveurWeb(HTTPPort); // crée un serveur sur le port HTTP standard

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

void testRequeteWeb()
{
  boolean currentLineIsBlank = true;

  WiFiClient client = serveurWeb.available();
  if (!client) return; // pas de client connecté

  while (client.connected()) {
    if (client.available()) {
      // on lit toute la trame HTPP, ici sans se soucier de la reqête
      char c = client.read();

      if (c == '\n' && currentLineIsBlank) { // une requête HTTP se termine par une ligne vide
        // ON GENERE LA PAGE WEB
        // On envoie un en tête de réponse HTTP standard
        client.println(F("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n"));

                if (SPIFFS.exists(nomDeFichier)) {
          File pageWeb = SPIFFS.open(nomDeFichier, "r");
          client.write(pageWeb);
          pageWeb.close();
        } else {
          Serial.println(F("Erreur de fichier"));
        }
        // on sort du while et termine la requête
        break;
      }
      if (c == '\n') currentLineIsBlank = true;
      else if (c != '\r') currentLineIsBlank = false;
    } // end if available
  } // end while
  delay(1);
  client.stop(); // termine la connexion
}

void setup() {
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

}

void loop() {
  testRequeteWeb();
}
