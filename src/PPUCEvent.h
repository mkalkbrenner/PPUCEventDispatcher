/*
  PPUCEvent.h
  Created by Markus Kalkbrenner, 2021.

  Play more pinball!
*/

#ifndef PPUCEVENT_h
#define PPUCEVENT_h

#define PLATFORM_WPC           1
#define PLATFORM_DATA_EAST     2

#define EVENT_SOURCE_ANY      42 // "*" PPUC
#define EVENT_SOURCE_DEBUG    66 // "B" PPUC Debug
#define EVENT_SOURCE_DMD      68 // "D" VPX/DOF/PUP
#define EVENT_SOURCE_EVENT    69 // "E" VPX/DOF/PUP common event from different system, like
#define EVENT_SOURCE_EFFECT   70 // "F" PPUC        event from running PPUCEffect
#define EVENT_SOURCE_GI       71 // "G" PPUC
#define EVENT_SOURCE_LIGHT    76 // "L" VPX/DOF/PUP lights, mainly playfield inserts
#define EVENT_SOURCE_SOUND    79 // "O" PPUC
#define EVENT_SOURCE_SOLENOID 83 // "S" VPX/DOF/PUP includes flashers
#define EVENT_SOURCE_SWITCH   87 // "W" VPX/DOF/PUP

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

    bool operator==(const PPUCEvent &other) const {
        return this->sourceId == other.sourceId
            && this->eventId == other.eventId
            && this->value == other.value;
    }

    bool operator!=(const PPUCEvent &other) const {
        return !(*this == other);
    }
};

#endif
