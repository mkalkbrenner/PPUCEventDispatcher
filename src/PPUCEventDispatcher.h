/*
  PPUCEventDispatcher.h
  Created by Markus Kalkbrenner, 2020-2021.

  Play more pinball!
*/

#ifndef EVENTDISPATCHER_h
#define EVENTDISPATCHER_h

#include <Arduino.h>
#include "PPUCEventListener.h"

#ifndef MAX_EVENT_LISTENERS
#define MAX_EVENT_LISTENERS 5
#endif

#ifndef EVENT_STACK_SIZE
#define EVENT_STACK_SIZE 10
#endif

#define EVENT_SOURCE_ANY 42 // "*"
#define EVENT_SOURCE_EVENT 69 // "E", common event from different system, like VPX, DOF, PUP
#define EVENT_SOURCE_DMD 68 // "D"
#define EVENT_SOURCE_SWITCH 87 // "W"
#define EVENT_SOURCE_LIGHT 76 // "L", mainly playfield inserts
#define EVENT_SOURCE_SOLENOID 83 // "S", includes flashers

class PPUCEventDispatcher {
public:
    PPUCEventDispatcher();

    void setCrossLinkSerial(HardwareSerial &reference);

    void addListener(PPUCEventListener* eventListener, char sourceId);

    void addListener(PPUCEventListener* eventListener);

    void dispatch(char sourceId, word eventId);
    void dispatch(char sourceId, word eventId, byte value);

    void update();

private:
    void callListeners(char sourceId, word eventId, byte value, bool send);

    char stackSourceIds[EVENT_STACK_SIZE];
    word stackEventIds[EVENT_STACK_SIZE];
    byte stackValues[EVENT_STACK_SIZE];
    int stackCounter = -1;

    PPUCEventListener* eventListeners[MAX_EVENT_LISTENERS];
    char eventListenerFilters[MAX_EVENT_LISTENERS];
    int numListeners = -1;

    bool crossLink = false;
    Stream* hwSerial;
};

#endif
