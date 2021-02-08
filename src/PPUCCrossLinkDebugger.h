/*
  PPUCCrossLinkDebugger.h
  Created by Markus Kalkbrenner, 2020-2021.

  Play more pinball!
*/

#ifndef CROSSLINKDEBUGGER_h
#define CROSSLINKDEBUGGER_h

#include <Arduino.h>
#include "PPUCEventListener.h"

class PPUCCrossLinkDebugger : public PPUCEventListener {
public:
    PPUCCrossLinkDebugger() {}

    void handleEvent(char sourceId, word eventId, byte value);
};

#endif
