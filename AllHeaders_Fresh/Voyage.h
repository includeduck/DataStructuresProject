#ifndef VOYAGE_H
#define VOYAGE_H

#include "core_ds.h"

class Voyage {
public:
    int fromPortId;
    int toPortId;
    DateTimeSimple departure;
    DateTimeSimple arrival;
    long costUsd;
    SimpleString company;
    Voyage *next;
    Voyage(): fromPortId(-1), toPortId(-1), costUsd(0), next(nullptr) {}
    void setCompany(const char *c) { company.assign(c); }
};

#endif // VOYAGE_H
