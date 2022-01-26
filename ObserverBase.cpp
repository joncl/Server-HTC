#include "ObserverBase.h"

#include "ObserverManager.h"

ObserverBase::ObserverBase() {
    setNextObserverId();
}

ObserverBase::ObserverBase(const ObserverBase&) {
    setNextObserverId();
}

ObserverBase::ObserverBase(ObserverBase&&) {
    setNextObserverId();
}

ObserverBase& ObserverBase::operator=(const ObserverBase&) {
    // retain observerId
    return *this;
}

ObserverBase& ObserverBase::operator=(ObserverBase&&) {
    // retain observerId
    return *this;
}

ObserverBase::~ObserverBase() {
    ObserverManager::unsubscribe(this);
}

void ObserverBase::setNextObserverId() {
    static id_t observersCount = 0;
    observerId = ++observersCount;
}