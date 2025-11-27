#ifndef PORT_H
#define PORT_H

#include "core_ds.h"

class Port {
public:
    int id;
    SimpleString name;
    int dailyDockCharge;
    Port *next;
    Port(): id(-1), dailyDockCharge(0), next(nullptr) {}
    void setName(const char *n) { name.assign(n); }
};

#endif // PORT_H
