#include "Arduino.h"
#include "DigiKeyboard.h"
#include "../WirePdw/WirePdw.hpp"

const uint8_t receivingPin = 2;
const uint8_t BOARD_LED_PIN = 1;

void setup()
{
	noInterrupts(); //disable interrupts during setup
	pinMode(receivingPin, INPUT);
	//WirePdw::receivingPin = receivingPin;
	attachInterrupt(0, WirePdw::receiveMessage, FALLING); // Use INT0(P2) on the Digispark
	DigiKeyboard.update();
	interrupts(); // enable interrupts as a last command of setup
}

const char* uintToStr(uint32_t value) {
  if (value == 0) {
    return "0";
  }
  static char buffer[32] = {0};
  int i = 30;
  for(; value && i ; --i, value /= 10)
  buffer[i] = "0123456789abcdef"[value % 10];
  return &buffer[i+1];
}

void debugInfo() {
	DigiKeyboard.print(WirePdw::counter1);
	DigiKeyboard.print("\t");
	DigiKeyboard.print(WirePdw::counter2);
	DigiKeyboard.print("\t");
	DigiKeyboard.print(WirePdw::counter3);
	DigiKeyboard.print("\t");
	DigiKeyboard.print((uint32_t) WirePdw::currentBit);
	DigiKeyboard.print("\t");
	DigiKeyboard.print((uint32_t) WirePdw::currentByte);
	DigiKeyboard.print("\t");
	DigiKeyboard.print((uint32_t) WirePdw::length);
	DigiKeyboard.print("\tB");
	DigiKeyboard.print((uint32_t) WirePdw::isReadingActive);
	DigiKeyboard.print("\tB");
	DigiKeyboard.print((uint32_t) WirePdw::isLengthRead);
	DigiKeyboard.print("\t");
	DigiKeyboard.print((uint32_t) WirePdw::timeUs);
	DigiKeyboard.print("\t");
	DigiKeyboard.print((uint32_t) WirePdw::timeDeltaBig);
	DigiKeyboard.print("\t");
	DigiKeyboard.print((uint32_t) WirePdw::timeDelta2);
	DigiKeyboard.print("\t\"");
	DigiKeyboard.print(WirePdw::completeMessage);
	DigiKeyboard.print("\"\n");

}

void loop()
{
	if (WirePdw::isMessageValid) {
		DigiKeyboard.delay(128);
		lightTheLed(128);
		DigiKeyboard.delay(128);
		lightTheLed(128);
		DigiKeyboard.delay(128);
		lightTheLed(128);

		DigiKeyboard.print(WirePdw::receivedMessage.message);
		WirePdw::isMessageValid = false;
	}
	DigiKeyboard.delay(2048);
	lightTheLed(512);
	debugInfo();
}

void lightTheLed(uint16_t timeMs) {
  digitalWrite(BOARD_LED_PIN, HIGH);
  DigiKeyboard.delay(timeMs);
  digitalWrite(BOARD_LED_PIN, LOW);
}
