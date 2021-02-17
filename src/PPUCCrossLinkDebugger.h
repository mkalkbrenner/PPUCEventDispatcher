/*
  PPUCCrossLinkDebugger.h
  Created by Markus Kalkbrenner, 2020-2021.

  Play more pinball!
*/

#ifndef PPUCCROSSLINKDEBUGGER_h
#define PPUCCROSSLINKDEBUGGER_h

#include <Arduino.h>
#include "PPUCEvent.h"
#include "PPUCEventListener.h"

class PPUCCrossLinkDebugger : public PPUCEventListener {
public:
    PPUCCrossLinkDebugger() {}

    void handleEvent(PPUCEvent* event);
};

#endif
