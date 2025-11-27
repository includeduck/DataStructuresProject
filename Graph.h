#ifndef GRAPH_H
#define GRAPH_H

#include "core_ds.h"
#include "containers.h"
#include "HashMap.h"

class Graph {
public:
    PortNodeCore *portsListHead;
    int portCount;
    PortNodeCore **portsById;
    int portsByIdCapacity;
    ScheduledVoyageCore **adjacencyHeads;
    HashMapStringToInt *nameToId;
    int nextId;
    Graph(int initialCapacity=64) {
        portsListHead = nullptr; portCount = 0; nextId = 0;
        portsByIdCapacity = initialCapacity>16?initialCapacity:16;
        portsById = (PortNodeCore**)calloc(portsByIdCapacity, sizeof(PortNodeCore*));
        adjacencyHeads = (ScheduledVoyageCore**)calloc(portsByIdCapacity, sizeof(ScheduledVoyageCore*));
        nameToId = new HashMapStringToInt(4096);
    }
    ~Graph() {
        PortNodeCore *p = portsListHead; while (p) { PortNodeCore *n=p->next; delete p; p=n; }
        for (int i=0;i<portsByIdCapacity;i++) { ScheduledVoyageCore *v=adjacencyHeads[i]; while (v) { ScheduledVoyageCore *nv=v->next; delete v; v=nv; } }
        free(portsById); free(adjacencyHeads); delete nameToId;
    }
    void ensureCapacity(int id) {
        if (id < portsByIdCapacity) return;
        int newcap = portsByIdCapacity; while (newcap <= id) newcap *= 2;
        portsById = (PortNodeCore**)realloc(portsById, newcap * sizeof(PortNodeCore*));
        for (int i=portsByIdCapacity;i<newcap;i++) portsById[i]=nullptr;
        adjacencyHeads = (ScheduledVoyageCore**)realloc(adjacencyHeads, newcap * sizeof(ScheduledVoyageCore*));
        for (int i=portsByIdCapacity;i<newcap;i++) adjacencyHeads[i]=nullptr;
        portsByIdCapacity = newcap;
    }
    int addPortIfMissing(const SimpleString &name) {
        int existing = nameToId->get(name.c_str());
        if (existing >= 0) return existing;
        int id = nextId++;
        PortNodeCore *pn = new PortNodeCore(); pn->identifier = id; pn->name = SimpleString(name.c_str()); pn->dailyDockCharge = 0; pn->next = portsListHead; portsListHead = pn;
        ensureCapacity(id);
        portsById[id] = pn;
        nameToId->put(pn->name.c_str(), id);
        ++portCount;
        return id;
    }
    void setPortCharge(const SimpleString &name, int charge) {
        int id = addPortIfMissing(name);
        portsById[id]->dailyDockCharge = charge;
    }
    void addVoyage(int fromId, int toId, const DateTimeSimple &dep, const DateTimeSimple &arr, long cost, const SimpleString &company) {
        ScheduledVoyageCore *v = new ScheduledVoyageCore(); v->originID = fromId; v->destinationID = toId; v->departure = dep; v->arrival = arr; v->costUSD = cost; v->company = SimpleString(company.c_str()); v->next = adjacencyHeads[fromId]; adjacencyHeads[fromId] = v;
    }
};

#endif // GRAPH_H
