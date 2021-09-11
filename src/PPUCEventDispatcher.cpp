#include "PPUCEventDispatcher.h"

PPUCEventDispatcher::PPUCEventDispatcher() {
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

        if (send) {
            byte msg[5];

            msg[0] = (byte) 255;
            msg[1] = (byte) event->sourceId;
            msg[2] = highByte(event->eventId);
            msg[3] = lowByte(event->eventId);
            msg[4] = event->value;
            //     = (byte) 255; stop byte added automatically by write()

            hwSerial->write(msg, 5);
        }
    }

    // delete the event and free the memory
    delete event;
}

void PPUCEventDispatcher::update() {
    while (stackCounter >= 0) {
        PPUCEvent* event = stackEvents[stackCounter--];
        callListeners(event, crossLink);
    }

    if (crossLink && hwSerial->available() > 4) {
        byte startByte = hwSerial->read();
        //Serial.println("startByte");
        //Serial.println(startByte);
        if (startByte == 255 && startByte != 0) {
            byte sourceId = hwSerial->read();
            //Serial.println("sourceId");
            //Serial.println(sourceId);
            if (sourceId != 0) {
                word eventId = word(hwSerial->read(), hwSerial->read());
                //Serial.println("eventId");
                //Serial.println(eventId);
                if (eventId != 0) {
                    byte value = hwSerial->read();
                    //Serial.println("value");
                    //Serial.println(value);
                    byte stopByte = hwSerial->read();
                    //Serial.println("stopByte");
                    //Serial.println(stopByte);
                    if (stopByte == 255) {
                        callListeners(new PPUCEvent((char) sourceId, eventId, value), false);
                    }
                }
            }
        }
    }
}
