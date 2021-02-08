#include "PPUCEventDispatcher.h"

PPUCEventDispatcher::PPUCEventDispatcher() {
}

void PPUCEventDispatcher::setCrossLinkSerial(HardwareSerial &reference) {
    hwSerial = &reference;
    ((HardwareSerial*) hwSerial)->begin(115200);
    crossLink = true;
}

void PPUCEventDispatcher::addListener(PPUCEventListener* eventListener) {
    addListener(eventListener, EVENT_SOURCE_ANY);
}

void PPUCEventDispatcher::addListener(PPUCEventListener* eventListener, char sourceId) {
    if (numListeners < (MAX_EVENT_LISTENERS - 1)) {
        eventListeners[++numListeners] = eventListener;
        eventListenerFilters[numListeners] = sourceId;
    }
}

void PPUCEventDispatcher::dispatch(char sourceId, word eventId) {
    dispatch(sourceId, eventId, 1);
}

void PPUCEventDispatcher::dispatch(char sourceId, word eventId, byte value) {
    noInterrupts();
    stackSourceIds[++stackCounter] = sourceId;
    stackEventIds[stackCounter] = eventId;
    stackValues[stackCounter] = value;
    interrupts();
}

void PPUCEventDispatcher::callListeners(char sourceId, word eventId, byte value, bool send) {
    for (byte i = 0; i <= numListeners; i++) {
        if (sourceId == eventListenerFilters[i] || EVENT_SOURCE_ANY == eventListenerFilters[i]) {
            eventListeners[i]->handleEvent(sourceId, eventId, value);
        }

        if (send) {
            byte msg[4];

            msg[0] = (byte) sourceId;
            msg[1] = highByte(eventId);
            msg[2] = lowByte(eventId);
            msg[3] = value;

            hwSerial->write(msg, 4);
        }
    }
}

void PPUCEventDispatcher::update() {
    while (stackCounter > 0) {
        noInterrupts();
        char sourceId = stackSourceIds[stackCounter];
        word eventId = stackEventIds[stackCounter];
        byte value = stackValues[stackCounter--];
        interrupts();

        callListeners((char) sourceId, eventId, value, crossLink);
    }

    if (crossLink && hwSerial->available() > 3) {
        byte sourceId = hwSerial->read();
        if (sourceId != 0) {
            byte eventIdHighByte = hwSerial->read();
            if (eventIdHighByte != 0) {
                byte eventIdLowByte = hwSerial->read();
                if (eventIdLowByte != 0) {
                    byte value = hwSerial->read();
                    if (value != 0) {
                        byte nullByte = hwSerial->read();
                        if (nullByte == 0) {
                            callListeners((char) sourceId, word(eventIdHighByte, eventIdLowByte), value, false);
                        }
                    }
                }
            }
        }
    }
}
