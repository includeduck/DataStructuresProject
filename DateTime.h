#ifndef DATETIME_H
#define DATETIME_H

#include "core_ds.h"

static bool parseDateToken(const char *dateToken, DateTimeSimple &out) {
    int d=0,m=0,y=0; if (sscanf(dateToken, "%d/%d/%d", &d, &m, &y) != 3) return false; out.day=d; out.month=m; out.year=y; return true;
}
static bool parseTimeToken(const char *timeToken, DateTimeSimple &out) {
    int hh=0,mm=0; if (sscanf(timeToken, "%d:%d", &hh, &mm) != 2) return false; out.hour=hh; out.minute=mm; return true;
}
static long long datetimeToEpochMinutes(const DateTimeSimple &dt) { return dt.toEpochMinutes(); }

#endif // DATETIME_H
