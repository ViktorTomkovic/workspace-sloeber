#include "WirePdw.hpp"

#include "Arduino.h"

namespace WirePdw {

const uint16_t pushedDownUs = 128*6;

const uint16_t lengthBeginUs = 8192*6;
const uint16_t pauseBeginUs = lengthBeginUs - pushedDownUs;

const uint16_t lengthEndUs = 512*6;
const uint16_t pauseEndUs = lengthEndUs - pushedDownUs;

const uint16_t lengthHighUs = 2048*6;
const uint16_t pauseHighUs = lengthHighUs - pushedDownUs;

const uint16_t lengthLowUs = 1024*6 + 16;
const uint16_t pauseLowUs = lengthLowUs - pushedDownUs;

const uint16_t errorUs = 512*6;
const uint16_t offsetUs = 256*6;

const uint16_t posErrorUs = errorUs + offsetUs;
const uint16_t negErrorUs = errorUs - offsetUs;

uint8_t transmittingPin;
volatile Message receivedMessage;
volatile bool isMessageValid;

void sendSignal(uint16_t micros) {
	digitalWrite(transmittingPin, LOW);
	delayMicroseconds(pushedDownUs);
	digitalWrite(transmittingPin, HIGH);
	delayMicroseconds(micros);
}

void sendBegin() {
	sendSignal(pauseBeginUs);
}

void sendEnd() {
	sendSignal(pauseEndUs);
}

void sendHigh() {
	sendSignal(pauseHighUs);
}
 
void sendLow() {
	sendSignal(pauseLowUs);
}

void sendUint8(uint8_t value) {
	//uint8_t mask = 1 << 7;
	uint8_t mask = 0b10000000;
	do {
		if (value & mask) {
			sendHigh();
		} else {
			sendLow();
		}
		// mask >>= 1;
		mask = mask / 2;
	} while (mask);
}

void sendChar(char* c) {
	sendUint8(*((uint8_t*) c));
}

void sendMessage(Message& message) {
	sendBegin();
	sendUint8(message.length);
	/*
	char* pc = (char*)message.message;
	while (*pc) {
		sendChar(pc);
		pc++;
	}
	*/
	for (int i = 0; i < message.length; i++) {
		sendUint8((uint8_t)message.message[i]);
	}
	sendEnd();
}

uint8_t length = 0;
uint8_t currentByte = 0;
uint8_t currentBit = 0;

uint32_t timeOldUs = 0;
uint32_t timeUs = 0;

uint8_t incompleteByte = 0;
uint8_t completeLength = 0;
char completeMessage[128];

bool isReadingActive = false;
bool isLengthRead = false;

uint16_t counter1 = 0;
uint16_t counter2 = 0;
uint16_t counter3 = 0;
uint32_t timeDeltaBig = 0;
uint32_t timeDelta2 = 0;

void receiveMessage() {
	if (isMessageValid) {
		return;
	}
	timeUs = micros();
	timeDeltaBig = timeUs - timeOldUs;
	timeOldUs = timeUs;

	if (timeDeltaBig > (lengthBeginUs + posErrorUs)) {
		timeDelta2 = timeDeltaBig;
		isReadingActive = false;
		return;
	}

	uint16_t timeDelta = (uint16_t) timeDeltaBig;

	if (timeDelta <= (lengthHighUs + posErrorUs)) {
	    if (!isReadingActive || (timeDelta < (lengthLowUs - negErrorUs))) {
	      isReadingActive = false;
	      return;
	    }

	    // read bit
	    uint8_t bitValue = (timeDelta > ((lengthLowUs + lengthHighUs) / 2)) ? 1 : 0;
	    counter3 = timeDelta;
	    incompleteByte <<= 1;
	    incompleteByte |= bitValue;
	    counter2 = bitValue;
	    counter1 = incompleteByte;
	    currentBit++;

	    if (currentBit == 8) {
		    if (isLengthRead) {
		    	completeMessage[currentByte] = (char) incompleteByte;
		    	currentBit = 0;
		    	currentByte++;
		    	if (currentByte == length) {
		    		completeMessage[currentByte] = 0;
		    		receivedMessage.length = length;
		    		receivedMessage.message = completeMessage;
		    		isMessageValid = true;
		    	}
		    } else {
		    	length = incompleteByte;
		    	currentBit = 0;
		    	isLengthRead = true;
		    }
	    }

	} else if (timeDelta < (lengthBeginUs - negErrorUs)) {
		isReadingActive = false;
	} else {
		incompleteByte = 0;
		currentBit = 0;

		currentByte = 0;
		isLengthRead = false;
		length = 0;
		isReadingActive = true;
	}
}

}
