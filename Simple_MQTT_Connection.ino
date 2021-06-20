// Library for w5100 or similar
//#include <Ethernet.h>
// Library for Enc28j60 or similar
#include <UIPEthernet.h>
// Bliblioteca Mqtt
#include <PubSubClient.h>

//Mac address
byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};

// Configures Broker Access
const char *mqtt_broker = "server"; // server
const char *mqtt_username = "usename"; // usename
const char *mqtt_password = "password"; // password
const int mqtt_port = 1883; // port tcp


char topic_A = "topic";
char sub_A = "sub";

// client id
#define client_id  "client_id"

EthernetClient ethClient;
PubSubClient client(ethClient);

void setup() {
  Serial.begin(9600);
  Ethernet.begin(mac);

  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to connect network: mac"); // prints on serial network connection error, possibly mac(?) error
    Ethernet.begin(mac);
    while (true);
  }

  Serial.print("Device IP: ");
  Serial.println(Ethernet.localIP()); //Device IP DHCP

  // Prepare connection to Broker
  client.setServer(mqtt_broker, mqtt_port);
  client.setCallback(callback);

}

//starts connection to broker
void reconnect() {
  while (!client.connected()) {
    Serial.println("Connecting to the broker.....");
    if (client.connect(client_id, mqtt_username, mqtt_password)) {
      Serial.println("Connected to broker");

      // subscribe to topic
      client.subscribe(sub_A);

    } else {
      Serial.print("Failure: ");
      Serial.print(client.state());
      delay(3000);
    }
  }
}

void callback(char *topic, byte *payload, unsigned int length) {
  Serial.print("Payload: ");
  Serial.print(topic);
  Serial.print("/");

  if (strcmp(topic, topic_A) == 0) {
    String message;
    for (int i = 0; i < length; i++) {
      message = message + (char) payload[i];  // convert *byte to string
    }
    Serial.println(message);
    /*if (message == "exemple") {
      //code...
      }*/
  }

}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}
