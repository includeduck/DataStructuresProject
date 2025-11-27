#ifndef GRAPHALGORITHMS_H
#define GRAPHALGORITHMS_H

#include "Graph.h"
#include "RouteState.h"
#include "MinHeap.h"
#include <cstdio>
#include <cstdlib>

static double computeDockCharge(const PortNodeCore *portNode, long long arrivedEpoch, long long nextDepartEpoch) {
    if (!portNode) return 0.0; long long layover = nextDepartEpoch - arrivedEpoch; if (layover <= 12*60) return 0.0; long long days = (layover + (24*60-1))/(24*60); return (double)(portNode->dailyDockCharge) * (double)days;
}

static int dijkstraFindRoute(Graph *g, const SimpleString &startName, const DateTimeSimple &startDT, const SimpleString &endName) {
    int startId = g->nameToId->get(startName.c_str());
    int endId = g->nameToId->get(endName.c_str());
    if (startId < 0 || endId < 0) { printf("start or end missing\n"); return 0; }
    long long startEpoch = startDT.toEpochMinutes();
    // best table
    struct BestCostRecord { long long timeEpoch; double cost; BestCostRecord *next; };
    BestCostRecord **bestTable = (BestCostRecord**)calloc(g->portsByIdCapacity, sizeof(BestCostRecord*));
    auto costAccessor = [](const RouteState *s)->double { return s->accumulatedCost; };
    MinHeapGeneric<RouteState> heap(256, (double(*)(const RouteState*))costAccessor);
    RouteState *initial = new RouteState(); initial->portId = startId; initial->timeEpochMinutes = startEpoch; initial->accumulatedCost = 0.0; initial->viaVoyage = nullptr; initial->previous = nullptr;
    heap.push(initial);
    BestCostRecord *b = (BestCostRecord*)malloc(sizeof(BestCostRecord)); b->timeEpoch = startEpoch; b->cost = 0.0; b->next = nullptr; bestTable[startId] = b;
    while (!heap.empty()) {
        RouteState *cur = heap.pop();
        if (cur->portId == endId) {
            RouteState *r = cur; RouteState *arr[1024]; int c=0; while (r && c<1024) { arr[c++]=r; r=r->previous; }
            printf("Found route cost %.2f\n", cur->accumulatedCost);
            for (int i=c-1;i>=0;--i) {
                RouteState *s = arr[i]; if (!s->viaVoyage) { printf("Start %d epoch %lld\n", s->portId, s->timeEpochMinutes); } else {
                    ScheduledVoyageCore *v = s->viaVoyage; PortNodeCore *pf = g->portsById[v->originID]; PortNodeCore *pt = g->portsById[v->destinationID]; printf("%s -> %s cost %ld\n", pf->name.c_str(), pt->name.c_str(), v->costUSD);
                }
            }
            return 1;
        }
        ScheduledVoyageCore *v = g->adjacencyHeads[cur->portId];
        while (v) {
            long long dep = v->departure.toEpochMinutes(); if (dep >= cur->timeEpochMinutes) {
                long long arr = v->arrival.toEpochMinutes(); PortNodeCore *pn = g->portsById[v->originID]; double dock = computeDockCharge(pn, cur->timeEpochMinutes, dep); double newCost = cur->accumulatedCost + (double)v->costUSD + dock;
                RouteState *ns = new RouteState(); ns->portId = v->destinationID; ns->timeEpochMinutes = arr; ns->accumulatedCost = newCost; ns->viaVoyage = v; ns->previous = cur;
                heap.push(ns);
            }
            v = v->next;
        }
    }
    return 0;
}

#endif // GRAPHALGORITHMS_H
