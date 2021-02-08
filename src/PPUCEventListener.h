/*
  PPUCEventListener.h
  Created by Markus Kalkbrenner, 2020-2021.

  Play more pinball!
*/

#ifndef EVENTLISTENER_h
#define EVENTLISTENER_h

class PPUCEventListener {
public:
    virtual void handleEvent(char sourceId, word eventId, byte value) = 0;
};

#endif
