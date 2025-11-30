#ifndef DATETIME_H
#define DATETIME_H

#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <stdexcept>

class DateTime {
private:
    int day, month, year;
    int hour, minute;

    bool isLeapYear(int year) const {
        return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
    }

    int daysInMonth(int month, int year) const {
        switch (month) {
        case 1: case 3: case 5: case 7: case 8: case 10: case 12: return 31;
        case 4: case 6: case 9: case 11: return 30;
        case 2: return isLeapYear(year) ? 29 : 28;
        default: return 0;
        }
    }

    int toMinutes() const {
        int totalDays = 0;
        for (int y = 2024; y < year; y++) {
            totalDays += isLeapYear(y) ? 366 : 365;
        }
        for (int m = 1; m < month; m++) {
            totalDays += daysInMonth(m, year);
        }
        totalDays += (day - 1);
        return totalDays * 24 * 60 + hour * 60 + minute;
    }

public:
    DateTime() : day(1), month(1), year(2024), hour(0), minute(0) {}

    DateTime(const std::string& date, const std::string& time) {
        char slash;
        std::stringstream date_ss(date);
        date_ss >> day >> slash >> month >> slash >> year;

        char colon;
        std::stringstream time_ss(time);
        time_ss >> hour >> colon >> minute;

        if (!isValid()) {
            throw std::invalid_argument("Invalid date or time provided");
        }
    }

    DateTime(const std::string& datetime) {
        std::string date = datetime.substr(0, 10);
        std::string time = datetime.substr(11, 5);

        char slash;
        std::stringstream date_ss(date);
        date_ss >> day >> slash >> month >> slash >> year;

        char colon;
        std::stringstream time_ss(time);
        time_ss >> hour >> colon >> minute;

        if (!isValid()) {
            throw std::invalid_argument("Invalid datetime provided");
        }
    }

    std::string getDate() const {
        std::stringstream ss;
        ss << std::setw(2) << std::setfill('0') << day << "/"
            << std::setw(2) << std::setfill('0') << month << "/"
            << year;
        return ss.str();
    }

    std::string getTime() const {
        std::stringstream ss;
        ss << std::setw(2) << std::setfill('0') << hour << ":"
            << std::setw(2) << std::setfill('0') << minute;
        return ss.str();
    }

    std::string toString() const {
        return getDate() + " " + getTime();
    }

    double hoursDifference(const DateTime& other) const {
        int thisMinutes = toMinutes();
        int otherMinutes = other.toMinutes();
        return (otherMinutes - thisMinutes) / 60.0;
    }

    bool isSameDay(const DateTime& other) const {
        return day == other.day && month == other.month && year == other.year;
    }

    DateTime addHours(double hours) const {
        DateTime result = *this;
        int totalMinutes = toMinutes() + static_cast<int>(hours * 60);

        int minutesPerDay = 24 * 60;
        int days = totalMinutes / minutesPerDay;
        int remainingMinutes = totalMinutes % minutesPerDay;

        result.hour = remainingMinutes / 60;
        result.minute = remainingMinutes % 60;

        result.day += days;
        while (result.day > daysInMonth(result.month, result.year)) {
            result.day -= daysInMonth(result.month, result.year);
            result.month++;
            if (result.month > 12) {
                result.month = 1;
                result.year++;
            }
        }

        return result;
    }

    bool isValid() const {
        if (month < 1 || month > 12) return false;
        if (day < 1 || day > daysInMonth(month, year)) return false;
        if (hour < 0 || hour > 23) return false;
        if (minute < 0 || minute > 59) return false;
        return true;
    }

    static bool isValidDate(const std::string& date) {
        try {
            DateTime dt(date, "00:00");
            return dt.isValid();
        }
        catch (...) {
            return false;
        }
    }

    static bool isValidTime(const std::string& time) {
        try {
            DateTime dt("01/01/2024", time);
            return dt.isValid();
        }
        catch (...) {
            return false;
        }
    }

    // Comparison operators
    bool operator<(const DateTime& other) const { return toMinutes() < other.toMinutes(); }
    bool operator<=(const DateTime& other) const { return toMinutes() <= other.toMinutes(); }
    bool operator>(const DateTime& other) const { return toMinutes() > other.toMinutes(); }
    bool operator>=(const DateTime& other) const { return toMinutes() >= other.toMinutes(); }
    bool operator==(const DateTime& other) const { return toMinutes() == other.toMinutes(); }
    bool operator!=(const DateTime& other) const { return toMinutes() != other.toMinutes(); }

    friend std::ostream& operator<<(std::ostream& os, const DateTime& dt) {
        os << dt.toString();
        return os;
    }
};

#endif