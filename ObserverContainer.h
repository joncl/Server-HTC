// ObserverContainer.h

#ifndef _OBSERVERCONTAINER_h
#define _OBSERVERCONTAINER_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#endif

#include "ObserverBase.h"

#include <unordered_set>

template <typename T>
class ObserverContainer {
    friend class ObserverManager;

private:
    static std::unordered_set<T*> observers;

    static void addObserver(T* obs) {
        observers.insert(obs);
    }

    static void removeObserver(ObserverBase* observer) {
        observers.erase(static_cast<T*>(observer));
    }

    static void clear() {
        observers.clear();
    }
};

template <typename T>
std::unordered_set<T*> ObserverContainer<T>::observers;

#endif