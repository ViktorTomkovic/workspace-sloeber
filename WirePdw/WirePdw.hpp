#ifndef _WIRE_PDW_
#define _WIRE_PDW_

#include "Arduino.h"

namespace WirePdw {

extern uint8_t transmittingPin;
//uint8_t receivingPin = 2;

typedef struct Message {
	uint8_t length;
	const char* message;
};

extern void sendMessage(Message& message);

extern volatile Message receivedMessage;
extern volatile bool isMessageValid;

/**
 * Use as interrupt method. Read result when isPwmMessageValid is true from
 * receivedPwmMessage.
 */
extern void receiveMessage();

extern uint16_t counter1;
extern uint16_t counter2;
extern uint16_t counter3;
extern uint8_t length;
extern uint8_t currentByte;
extern uint8_t currentBit;
extern uint8_t incompleteByte;
extern uint8_t completeLength;
extern char completeMessage[128];
extern bool isReadingActive;
extern bool isLengthRead;

extern uint32_t timeOldUs;
extern uint32_t timeUs;
extern uint32_t timeDeltaBig;
extern uint32_t timeDelta2;

}

#endif
