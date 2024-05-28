/*
#include<Arduino.h>
#include <AsyncTCP.h>

#define SSID "ESP-TEST"
#define PASSWORD "123456789"

#define SERVER_HOST_NAME "esp_server"

#define TCP_PORT 7050
#define DNS_PORT 53

static void replyToServer(void* arg) {
	AsyncClient* client = reinterpret_cast<AsyncClient*>(arg);

	// send reply
	if (client->space() > 32 && client->canSend()) {
		char message[32];
		client->add(message, strlen(message));
		client->send();
	}
}

static void handleData(void* arg, AsyncClient* client, void *data, size_t len) {
	Serial.printf("\n data received from %s \n", client->remoteIP().toString().c_str());
	Serial.write((uint8_t*)data, len);

}

void onConnect(void* arg, AsyncClient* client) {
	Serial.printf("\n client has been connected to %s on port %d \n", SERVER_HOST_NAME, TCP_PORT);
	replyToServer(client);
}


void setup() {
	Serial.begin(115200);
	delay(20);

	AsyncClient* client = new AsyncClient;
	client->onData(&handleData, client);
	client->onConnect(&onConnect, client);
	client->connect(SERVER_HOST_NAME, TCP_PORT);

}

void loop() {

}
*/