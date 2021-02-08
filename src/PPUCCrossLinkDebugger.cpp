#include "PPUCCrossLinkDebugger.h"

void PPUCCrossLinkDebugger::handleEvent(char sourceId, word eventId, byte value) {
    // On Teensy Serial is the USB Serial.
    Serial.print("handleEvent: sourceId ");
    Serial.print(sourceId);
    Serial.print(", eventId ");
    Serial.println(eventId);
}
