#include <WiFi.h>
#include <WebSocketsClient.h>
#include <ArduinoJson.h>

const char* ssid = "votre_ssid_wifi";
const char* password = "votre_mot_de_passe_wifi";
const char* websocket_server = "adresse_ip_de_votre_serveur";
const int websocket_port = 3000;

WebSocketsClient webSocket;

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connexion au WiFi...");
  }
  
  Serial.println("Connecté au WiFi");
  
  webSocket.begin(websocket_server, websocket_port, "/socket.io/?EIO=4");
  webSocket.onEvent(webSocketEvent);
  webSocket.setReconnectInterval(5000);
}

void loop() {
  webSocket.loop();
}

void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {
  switch(type) {
    case WStype_DISCONNECTED:
      Serial.println("Déconnecté du WebSocket");
      break;
    case WStype_CONNECTED:
      Serial.println("Connecté au WebSocket");
      break;
    case WStype_TEXT:
      handleTikTokEvent(payload, length);
      break;
  }
}

void handleTikTokEvent(uint8_t * payload, size_t length) {
  DynamicJsonDocument doc(1024);
  deserializeJson(doc, payload, length);
  
  String eventType = doc[1]["type"];
  JsonObject data = doc[1]["data"];
  
  if (eventType == "chat") {
    String username = data["uniqueId"];
    String message = data["comment"];
    Serial.printf("Chat: %s dit: %s\n", username.c_str(), message.c_str());
  } else if (eventType == "gift") {
    String username = data["uniqueId"];
    String giftName = data["giftName"];
    int giftCount = data["repeatCount"];
    Serial.printf("Cadeau: %s a envoyé %d %s\n", username.c_str(), giftCount, giftName.c_str());
  } else if (eventType == "like") {
    String username = data["uniqueId"];
    int likeCount = data["likeCount"];
    Serial.printf("Like: %s a envoyé %d likes\n", username.c_str(), likeCount);
  }
  // Ajoutez d'autres types d'événements selon vos besoins
}
