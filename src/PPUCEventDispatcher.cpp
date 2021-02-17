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

void PPUCEventDispatcher::dispatch(PPUCEvent* event) {
    stackEvents[++stackCounter] = event;
}

void PPUCEventDispatcher::callListeners(PPUCEvent* event, bool send) {
    for (byte i = 0; i <= numListeners; i++) {
        if (event->sourceId == eventListenerFilters[i] || EVENT_SOURCE_ANY == eventListenerFilters[i]) {
            eventListeners[i]->handleEvent(event);
        }

        if (send) {
            byte msg[4];

            msg[0] = (byte) event->sourceId;
            msg[1] = highByte(event->eventId);
            msg[2] = lowByte(event->eventId);
            msg[3] = event->value;

            hwSerial->write(msg, 4);
        }
    }

    // delete the event and free the memory
    delete event;
}

void PPUCEventDispatcher::update() {
    while (stackCounter > 0) {
        PPUCEvent* event = stackEvents[stackCounter--];
        callListeners(event, crossLink);
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
                            callListeners(new PPUCEvent((char) sourceId, word(eventIdHighByte, eventIdLowByte), value), false);
                        }
                    }
                }
            }
        }
    }
}
