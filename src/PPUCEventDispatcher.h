/*
  PPUCEventDispatcher.h
  Created by Markus Kalkbrenner, 2020-2021.

  Play more pinball!
*/

#ifndef PPUCEVENTDISPATCHER_h
#define PPUCEVENTDISPATCHER_h

#include <Arduino.h>

#include "PPUCEvent.h"
#include "PPUCEventListener.h"

#ifndef MAX_EVENT_LISTENERS
#define MAX_EVENT_LISTENERS 5
#endif

#ifndef EVENT_STACK_SIZE
#define EVENT_STACK_SIZE 100
#endif

class PPUCEventDispatcher {
public:
    PPUCEventDispatcher();

    void setCrossLinkSerial(HardwareSerial &reference);

    void addListener(PPUCEventListener* eventListener, char sourceId);

    void addListener(PPUCEventListener* eventListener);

    void dispatch(PPUCEvent* event);

    void update();

private:
    void callListeners(PPUCEvent* event, bool send);

    PPUCEvent* stackEvents[EVENT_STACK_SIZE];
    int stackCounter = -1;

    PPUCEventListener* eventListeners[MAX_EVENT_LISTENERS];
    char eventListenerFilters[MAX_EVENT_LISTENERS];
    int numListeners = -1;

    bool crossLink = false;
    HardwareSerial* hwSerial;
};

#endif
