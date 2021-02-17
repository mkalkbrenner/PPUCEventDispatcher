/*
  PPUCEvent.h
  Created by Markus Kalkbrenner, 2021.

  Play more pinball!
*/

#ifndef PPUCEVENT_h
#define PPUCEVENT_h

#define EVENT_SOURCE_ANY 42 // "*"
#define EVENT_SOURCE_EVENT 69 // "E", common event from different system, like VPX, DOF, PUP
#define EVENT_SOURCE_DMD 68 // "D"
#define EVENT_SOURCE_SWITCH 87 // "W"
#define EVENT_SOURCE_LIGHT 76 // "L", mainly playfield inserts
#define EVENT_SOURCE_SOLENOID 83 // "S", includes flashers

struct PPUCEvent {
    char sourceId;
    word eventId;
    byte value;

    PPUCEvent(char sId, word eId) {
        sourceId = sId;
        eventId = eId;
        value = 1;
    }

    PPUCEvent(char sId, word eId, byte v) {
        sourceId = sId;
        eventId = eId;
        value = v;
    }
};

#endif
