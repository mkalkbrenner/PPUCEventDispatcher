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

#ifndef MAX_CROSS_LINKS
#define MAX_CROSS_LINKS 8
#endif

#ifndef EVENT_STACK_SIZE
#define EVENT_STACK_SIZE 100
#endif

class PPUCEventDispatcher {
public:
    PPUCEventDispatcher();

    void setCrossLinkSerial(HardwareSerial &reference);

    void addCrossLinkSerial(HardwareSerial &reference);

    void addListener(PPUCEventListener* eventListener, char sourceId);

    void addListener(PPUCEventListener* eventListener);

    void dispatch(PPUCEvent* event);

    void update();

private:
    void callListeners(PPUCEvent* event, int sender);

    PPUCEvent* stackEvents[EVENT_STACK_SIZE];
    int stackCounter = -1;

    PPUCEventListener* eventListeners[MAX_EVENT_LISTENERS];
    char eventListenerFilters[MAX_EVENT_LISTENERS];
    int numListeners = -1;

    byte msg[6] = {0};

    int crossLink = -1;
    HardwareSerial* hwSerial[MAX_CROSS_LINKS];
};

#endif
