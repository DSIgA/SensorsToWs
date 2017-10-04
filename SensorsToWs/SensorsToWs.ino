/* DHT */
#include <DHT_U.h>
#include <DHT.h>
#define DHTPIN 2
#define DHTTYPE  DHT11

/* jSON */
#include <ArduinoJson.h>
#include "jsonIoT.h"

/* WiFi ESP8266 */
#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>

/* DHT sensor */

//Values
DHT dht(DHTPIN, DHTTYPE, 15);

void setup() {

	Serial.begin(115200);

	WiFi.begin("Conra", "asdqwe123");   //WiFi connection

	while (WiFi.status() != WL_CONNECTED) {  //Wait for the WiFI connection completion 
		delay(1000);
		Serial.println("Waiting for connection...");
	}

	// Initialize sensor
	dht.begin();
	delay(1000);

}

void loop() {
	/* Read DHT11 sensor */
	float humidity = dht.readHumidity();
	float temperature = dht.readTemperature();

	if (isnan(humidity) || isnan(temperature)) {
		return;
	}

	/* Build jSON */
	StaticJsonBuffer<BUFFER_SIZE> jsonBuffer;
	char jsonMessage[BUFFER_SIZE];
	JsonObject& jsonObjMsg = jsonBuffer.createObject();

	jsonObjMsg[F_TEMP] = temperature;
	jsonObjMsg[F_HUM] = humidity;
	jsonObjMsg[F_WU] = "1995-04-20 23:58:36.900";
	jsonObjMsg[F_USR] = "Conra";

	jsonObjMsg.prettyPrintTo(jsonMessage);

	if (WiFi.status() == WL_CONNECTED) {   //Check WiFi connection status

										   /* POST information to REST Web Service */
		HTTPClient http;    //Declare object of class HTTPClient

		Serial.println("Begin http");
		http.begin("http://200.41.164.91/ga-iotpoc/api/distance");      //Specify request destination
		http.addHeader("Content-Type", "application/json");  //Specify content-type header

		Serial.println("Send post");
		int httpCode = http.POST("jsonMessage");   //Send the request
		String payload = http.getString();                  //Get the response payload

		Serial.println("Http Code");   //Print HTTP return code
		Serial.println(httpCode);   //Print HTTP return code
		Serial.println("Payload");   //Print HTTP return code
		Serial.println(payload);    //Print request response payload

		http.end();  //Close connection

	}
	else {
		Serial.println("Error in WiFi connection");
	}

	delay(10000);  //Send a request every 10 seconds

}
