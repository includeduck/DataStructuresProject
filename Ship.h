#ifndef SHIP_H
#define SHIP_H

#include <string>
#include "LinkedList.h"
#include "DateTime.h"
#include "GraphStructures.h"

struct Ship {
    std::string name;
    std::string company;
    double maxVoyageTime;
    LinkedList<std::string> avoidedPorts;
    LinkedList<Route*> currentJourney;
    DateTime arrivalTime;

    Ship(const std::string& n, const std::string& comp = "",
        double maxTime = 0.0, const LinkedList<std::string>& avoided = LinkedList<std::string>())
        : name(n), company(comp), maxVoyageTime(maxTime), avoidedPorts(avoided) {
    }

    bool canUseRoute(const Route& route) const {
        if (!company.empty() && route.company != company) {
            return false;
        }
        if (avoidsPort(route.origin) || avoidsPort(route.destination)) {
            return false;
        }
        if (maxVoyageTime > 0 && route.getVoyageDuration() > maxVoyageTime) {
            return false;
        }
        return true;
    }

    void addRouteToJourney(Route* route) {
        currentJourney.push_back(route);
    }

    void clearJourney() {
        currentJourney.clear();
    }

    double getTotalJourneyTime() const {
        double total = 0.0;
        for (auto it = currentJourney.begin(); it != currentJourney.end(); ++it) {
            total += (*it)->getVoyageDuration();
        }
        return total;
    }

    double getTotalJourneyCost() const {
        double total = 0.0;
        for (auto it = currentJourney.begin(); it != currentJourney.end(); ++it) {
            total += (*it)->cost;
        }
        return total;
    }

    bool avoidsPort(const std::string& portName) const {
        for (auto it = avoidedPorts.begin(); it != avoidedPorts.end(); ++it) {
            if (*it == portName) return true;
        }
        return false;
    }

    void addAvoidedPort(const std::string& portName) {
        if (!avoidsPort(portName)) {
            avoidedPorts.push_back(portName);
        }
    }

    void removeAvoidedPort(const std::string& portName) {
        LinkedList<std::string> newList;
        for (auto it = avoidedPorts.begin(); it != avoidedPorts.end(); ++it) {
            if (*it != portName) {
                newList.push_back(*it);
            }
        }
        avoidedPorts = newList;
    }
};

#endif