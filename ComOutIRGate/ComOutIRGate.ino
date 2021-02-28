#include "Arduino.h"
#include "../WirePdw/WirePdw.hpp"

const uint8_t transmittingPin = 3;
uint8_t length = 1;

const uint8_t BOARD_LED_PIN = 1;

void setup() {
	WirePdw::transmittingPin = transmittingPin;
	pinMode(WirePdw::transmittingPin, OUTPUT);
}


void lightTheLed(uint16_t timeMs) {
  digitalWrite(BOARD_LED_PIN, HIGH);
  delay(timeMs);
  digitalWrite(BOARD_LED_PIN, LOW);
}

void loop() {
	delay(2000);
	length = length + 1;
	WirePdw::Message message;
	message.length = length;
	message.message = "123456789";
	delay(256);
	lightTheLed(256);
	delay(256);
	lightTheLed(256);
	delay(256);
	lightTheLed(256);
	WirePdw::sendMessage(message);
	if (length == 9) {
		length = 0;
	}
}
