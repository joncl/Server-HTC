#include "ObserverManager.h"

std::unordered_map<ObserverBase::id_t, void (*)(ObserverBase*)> ObserverManager::unsubscribeMap;
