/*
  PPUCEventListener.h
  Created by Markus Kalkbrenner, 2020-2021.

  Play more pinball!
*/

#ifndef EVENTLISTENER_h
#define EVENTLISTENER_h

#include "PPUCEvent.h"

class PPUCEventListener {
public:
    virtual void handleEvent(PPUCEvent* event) = 0;
};

#endif
