#include "PPUCCrossLinkDebugger.h"

void PPUCCrossLinkDebugger::handleEvent(PPUCEvent* event) {
    // On Teensy Serial is the USB Serial.
    Serial.print("handleEvent: sourceId ");
    Serial.print(event->sourceId);
    Serial.print(", eventId ");
    Serial.print(event->eventId);
    Serial.print(", value ");
    Serial.println(event->value);
}
