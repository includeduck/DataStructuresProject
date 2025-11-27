#ifndef ROUTESTATE_H
#define ROUTESTATE_H

#include "core_ds.h"

class RouteState {
public:
    int portId;
    long long timeEpochMinutes;
    double accumulatedCost;
    ScheduledVoyageCore *viaVoyage;
    RouteState *previous;
    RouteState(): portId(-1), timeEpochMinutes(0), accumulatedCost(0.0), viaVoyage(nullptr), previous(nullptr) {}
};

#endif // ROUTESTATE_H
