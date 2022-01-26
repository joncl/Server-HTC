// ObserverBase.h

#ifndef _OBSERVERBASE_h
#define _OBSERVERBASE_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#endif

#include <cstdint>

class ObserverBase {
    friend class ObserverManager;

public:
    ObserverBase();
    ObserverBase(const ObserverBase&);
    ObserverBase(ObserverBase&&);
    ObserverBase& operator=(const ObserverBase&);
    ObserverBase& operator=(ObserverBase&&);
    virtual ~ObserverBase();

private:
    void setNextObserverId();

private:
    using id_t = uint64_t;
    id_t observerId;
};

#endif