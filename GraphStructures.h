#ifndef GRAPH_STRUCTURES_H
#define GRAPH_STRUCTURES_H

#include <string>
#include "LinkedList.h"
#include "Vector.h"
#include "DateTime.h"

struct Route {
    std::string origin;
    std::string destination;
    std::string date;
    std::string departureTime;
    std::string arrivalTime;
    double cost;
    std::string company;

    Route(const std::string& o, const std::string& d, const std::string& dt,
        const std::string& dep, const std::string& arr, double c, const std::string& comp)
        : origin(o), destination(d), date(dt), departureTime(dep),
        arrivalTime(arr), cost(c), company(comp) {
    }

    double getVoyageDuration() const {
        DateTime dep(date, departureTime);
        DateTime arr(date, arrivalTime);
        return arr.hoursDifference(dep);
    }

    DateTime getDepartureDateTime() const {
        return DateTime(date, departureTime);
    }

    DateTime getArrivalDateTime() const {
        DateTime dep = getDepartureDateTime();
        double duration = getVoyageDuration();
        return dep.addHours(duration);
    }

    bool operatesOnDate(const std::string& targetDate) const {
        return date == targetDate;
    }
};

struct Port {
    std::string name;
    double charge;
    LinkedList<Route*> outgoingRoutes;

    Port(const std::string& n, double c) : name(n), charge(c) {}

    void addRoute(Route* route) {
        outgoingRoutes.push_back(route);
    }

    Route* findRouteTo(const std::string& destination, const std::string& date) const {
        for (auto it = outgoingRoutes.begin(); it != outgoingRoutes.end(); ++it) {
            Route* route = *it;
            if (route->destination == destination && route->operatesOnDate(date)) {
                return route;
            }
        }
        return nullptr;
    }

    Vector<Route*> findRoutesTo(const std::string& destination, const std::string& date) const {
        Vector<Route*> result;
        for (auto it = outgoingRoutes.begin(); it != outgoingRoutes.end(); ++it) {
            Route* route = *it;
            if (route->destination == destination && route->operatesOnDate(date)) {
                result.push_back(route);
            }
        }
        return result;
    }
};

struct PathNode {
    std::string portName;
    double cost;
    double heuristic;
    Route* routeUsed;
    PathNode* previous;

    PathNode(const std::string& name, double c = 0.0, double h = 0.0,
        Route* route = nullptr, PathNode* prev = nullptr)
        : portName(name), cost(c), heuristic(h), routeUsed(route), previous(prev) {
    }

    bool operator<(const PathNode& other) const {
        return (cost + heuristic) < (other.cost + other.heuristic);
    }

    bool operator>(const PathNode& other) const {
        return (cost + heuristic) > (other.cost + other.heuristic);
    }
};

#endif