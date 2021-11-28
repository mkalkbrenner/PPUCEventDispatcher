#include "PPUCEventDispatcher.h"

PPUCEventDispatcher::PPUCEventDispatcher() {
    msg[0] = (byte) 255;
    msg[5] = (byte) 255;
}

void PPUCEventDispatcher::setCrossLinkSerial(HardwareSerial &reference) {
    hwSerial = (HardwareSerial*) &reference;
    hwSerial->begin(115200);
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
    if (stackCounter < (EVENT_STACK_SIZE - 1)) {
        stackEvents[++stackCounter] = event;
    }
    else {
        // Too many events stacked, delete the event and free the memory.
        delete event;
    }
}

void PPUCEventDispatcher::callListeners(PPUCEvent* event, bool send) {
    for (byte i = 0; i <= numListeners; i++) {
        if (event->sourceId == eventListenerFilters[i] || EVENT_SOURCE_ANY == eventListenerFilters[i]) {
            eventListeners[i]->handleEvent(event);
        }
    }

    if (send) {
        //     = (byte) 255;
        msg[1] = (byte) event->sourceId;
        msg[2] = highByte(event->eventId);
        msg[3] = lowByte(event->eventId);
        msg[4] = event->value;
        //     = (byte) 255;

        hwSerial->write(msg, 6);
    }

    // delete the event and free the memory
    delete event;
}

void PPUCEventDispatcher::update() {
    while (stackCounter >= 0) {
        PPUCEvent* event = stackEvents[stackCounter--];
        callListeners(event, crossLink);
    }

    if (crossLink && hwSerial->available() >= 6) {
        byte startByte = hwSerial->read();
        if (startByte == 255) {
            byte sourceId = hwSerial->read();
            if (sourceId != 0) {
                word eventId = word(hwSerial->read(), hwSerial->read());
                if (eventId != 0) {
                    byte value = hwSerial->read();
                    byte stopByte = hwSerial->read();
                    if (stopByte == 255) {
                        callListeners(new PPUCEvent((char) sourceId, eventId, value), false);
                    }
                }
            }
        }
    }
}
