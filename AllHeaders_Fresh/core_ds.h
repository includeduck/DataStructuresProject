#ifndef CORE_DS_H
#define CORE_DS_H

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include <ctime>

// Minimal replacement for std::string
class SimpleString {
private:
    char *buffer;
    size_t length;
    void allocateAndCopy(const char *s) {
        if (!s) { buffer = nullptr; length = 0; return; }
        length = strlen(s);
        buffer = (char*)malloc(length + 1);
        memcpy(buffer, s, length + 1);
    }
public:
    SimpleString(): buffer(nullptr), length(0) {}
    SimpleString(const char *s) { allocateAndCopy(s); }
    SimpleString(const SimpleString &other) { allocateAndCopy(other.buffer); }
    SimpleString(SimpleString &&other) noexcept { buffer = other.buffer; length = other.length; other.buffer = nullptr; other.length = 0; }
    SimpleString &operator=(const SimpleString &other) { if (this == &other) return *this; free(buffer); allocateAndCopy(other.buffer); return *this; }
    SimpleString &operator=(SimpleString &&other) noexcept { if (this == &other) return *this; free(buffer); buffer = other.buffer; length = other.length; other.buffer = nullptr; other.length = 0; return *this; }
    ~SimpleString() { if (buffer) free(buffer); }
    const char *c_str() const { return buffer ? buffer : ""; }
    size_t size() const { return length; }
    void clear() { if (buffer) { free(buffer); buffer = nullptr; length = 0; } }
    void assign(const char *s) { free(buffer); allocateAndCopy(s); }
    int compare(const SimpleString &other) const { return strcmp(c_str(), other.c_str()); }
    bool equals(const SimpleString &other) const { return compare(other) == 0; }
    void trimInPlace() {
        if (!buffer) return;
        char *p = buffer;
        while (*p && isspace((unsigned char)*p)) ++p;
        if (p != buffer) memmove(buffer, p, strlen(p) + 1);
        size_t len = strlen(buffer);
        while (len > 0 && isspace((unsigned char)buffer[len - 1])) { buffer[len - 1] = '\\0'; --len; }
        length = len;
    }
};

class DateTimeSimple {
public:
    int year, month, day, hour, minute;
    DateTimeSimple(): year(0), month(0), day(0), hour(0), minute(0) {}
    long long toEpochMinutes() const {
        struct tm t = {0};
        t.tm_year = year - 1900;
        t.tm_mon = month - 1;
        t.tm_mday = day;
        t.tm_hour = hour;
        t.tm_min = minute;
        #ifdef _GNU_SOURCE
        time_t s = timegm(&t);
        #else
        time_t s = mktime(&t);
        #endif
        if (s == (time_t)-1) return -1;
        return (long long)(s / 60);
    }
};

class PortNodeCore {
public:
    int identifier;
    SimpleString name;
    int dailyDockCharge;
    PortNodeCore *next;
    PortNodeCore(): identifier(-1), dailyDockCharge(0), next(nullptr) {}
};

class ScheduledVoyageCore {
public:
    int originID;
    int destinationID;
    DateTimeSimple departure;
    DateTimeSimple arrival;
    long costUSD;
    SimpleString company;
    ScheduledVoyageCore *next;
    ScheduledVoyageCore(): originID(-1), destinationID(-1), costUSD(0), next(nullptr) {}
};

#endif // CORE_DS_H
