#ifndef MATHUTILS_H
#define MATHUTILS_H

static long long ceilDivideLongLong(long long a, long long b) { if (b==0) return 0; return (a + b - 1) / b; }
static long long minutesBetween(long long aEpochMin, long long bEpochMin) { return bEpochMin - aEpochMin; }

#endif // MATHUTILS_H
