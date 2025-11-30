#ifndef MARITIME_SYSTEM_H
#define MARITIME_SYSTEM_H

#include "GraphStructures.h"
#include "PathFinding.h"
#include "Map.h"
#include "Set.h"
#include "Vector.h"
#include "LinkedList.h"
#include "Queue.h"
#include "PriorityQueue.h"
#include "Ship.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <functional>

class MaritimeSystem {
private:
    Map<std::string, Port*> ports;
    Map<std::string, Queue<Ship*>> portQueues;
    Vector<Route*> allRoutes;

    bool isConnectionValid(const Route& first, const Route& second) const {
        if (first.destination != second.origin) return false;

        DateTime firstArrival = first.getArrivalDateTime();
        DateTime secondDeparture = second.getDepartureDateTime();

        // Ensure second departure is after first arrival with reasonable buffer
        return secondDeparture > firstArrival;
    }

    // Helper function to get all routes from a port on a specific date
    Vector<Route*> getRoutesFromPortOnDate(const std::string& portName, const std::string& date) const {
        Vector<Route*> result;
        if (!ports.contains(portName)) return result;

        Port* port = ports.at(portName);
        for (auto it = port->outgoingRoutes.begin(); it != port->outgoingRoutes.end(); ++it) {
            Route* route = *it;
            if (route->operatesOnDate(date)) {
                result.push_back(route);
            }
        }
        return result;
    }

public:
    MaritimeSystem() = default;

    ~MaritimeSystem() {
        // Cleanup all routes
        for (size_t i = 0; i < allRoutes.size(); ++i) {
            delete allRoutes[i];
        }
        allRoutes.clear();

        // Cleanup all ports
        auto portKeys = ports.keys();
        for (size_t i = 0; i < portKeys.size(); ++i) {
            delete ports.at(portKeys[i]);
        }
        ports.clear();
    }

    double calculateVoyageDuration(const Route& route) const {
        return route.getVoyageDuration();
    }

    double calculateLayoverCost(const std::string& portName, double hours) const {
        if (!ports.contains(portName)) return 0.0;
        double days = hours / 24.0;
        return ports.at(portName)->charge * days;
    }

    bool loadPortCharges(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Error: Could not open " << filename << std::endl;
            return false;
        }

        std::string line;
        while (std::getline(file, line)) {
            std::stringstream ss(line);
            std::string portName;
            double charge;

            if (ss >> portName >> charge) {
                ports.insert(portName, new Port(portName, charge));
            }
        }

        file.close();
        std::cout << "Loaded " << ports.size() << " ports from " << filename << std::endl;
        return true;
    }

    bool loadRoutes(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Error: Could not open " << filename << std::endl;
            return false;
        }

        std::string line;
        while (std::getline(file, line)) {
            std::stringstream ss(line);
            std::string origin, destination, date, departureTime, arrivalTime, company;
            double cost;

            if (ss >> origin >> destination >> date >> departureTime >> arrivalTime >> cost >> company) {
                Route* newRoute = new Route(origin, destination, date, departureTime, arrivalTime, cost, company);
                allRoutes.push_back(newRoute);

                // Add to port's outgoing routes
                if (ports.contains(origin)) {
                    ports.at(origin)->addRoute(newRoute);
                }
                else {
                    // Create port if it doesn't exist (with default charge 0)
                    ports.insert(origin, new Port(origin, 0.0));
                    ports.at(origin)->addRoute(newRoute);
                }

                // Ensure destination port exists
                if (!ports.contains(destination)) {
                    ports.insert(destination, new Port(destination, 0.0));
                }
            }
        }

        file.close();
        std::cout << "Loaded " << allRoutes.size() << " routes from " << filename << std::endl;
        return true;
    }

    Vector<Route*> findDirectRoutes(const std::string& origin, const std::string& destination,
        const std::string& date) const {
        Vector<Route*> result;

        if (!ports.contains(origin)) return result;

        Port* originPort = ports.at(origin);
        for (auto it = originPort->outgoingRoutes.begin(); it != originPort->outgoingRoutes.end(); ++it) {
            Route* route = *it;
            if (route->destination == destination && route->operatesOnDate(date)) {
                result.push_back(route);
            }
        }

        return result;
    }

    Vector<LinkedList<Route*>> findConnectingRoutes(const std::string& origin,
        const std::string& destination,
        const std::string& date) const {
        Vector<LinkedList<Route*>> result;

        // Simple BFS for connecting routes (max 2 connections for now)
        if (!ports.contains(origin)) return result;

        Port* originPort = ports.at(origin);

        // Check direct routes first
        Vector<Route*> directRoutes = originPort->findRoutesTo(destination, date);
        for (size_t i = 0; i < directRoutes.size(); ++i) {
            LinkedList<Route*> routeList;
            routeList.push_back(directRoutes[i]);
            result.push_back(routeList);
        }

        // Check one-connection routes
        for (auto it1 = originPort->outgoingRoutes.begin(); it1 != originPort->outgoingRoutes.end(); ++it1) {
            Route* firstLeg = *it1;
            if (!firstLeg->operatesOnDate(date)) continue;

            std::string intermediate = firstLeg->destination;
            if (!ports.contains(intermediate)) continue;

            Port* intermediatePort = ports.at(intermediate);
            Vector<Route*> secondLegs = intermediatePort->findRoutesTo(destination, date);

            for (size_t i = 0; i < secondLegs.size(); ++i) {
                Route* secondLeg = secondLegs[i];
                if (isConnectionValid(*firstLeg, *secondLeg)) {
                    LinkedList<Route*> routeList;
                    routeList.push_back(firstLeg);
                    routeList.push_back(secondLeg);
                    result.push_back(routeList);
                }
            }
        }

        return result;
    }

    // Dijkstra's algorithm for shortest path (time-based)
    LinkedList<Route*> findShortestPathDijkstra(const std::string& origin,
        const std::string& destination,
        const std::string& date) const {
        if (!ports.contains(origin) || !ports.contains(destination)) {
            return LinkedList<Route*>();
        }

        Map<std::string, double> dist;
        Map<std::string, PathFinding::DijkstraNode*> nodes;
        Set<std::string> visited;
        PriorityQueue<PathFinding::DijkstraNode> pq;

        // Initialize
        auto startNode = new PathFinding::DijkstraNode(origin, 0.0);
        nodes.insert(origin, startNode);
        dist.insert(origin, 0.0);
        pq.push(*startNode);

        while (!pq.empty()) {
            PathFinding::DijkstraNode current = pq.top();
            pq.pop();

            if (visited.contains(current.portName)) continue;
            visited.insert(current.portName);

            if (current.portName == destination) {
                break;
            }

            // Get all routes from current port
            Vector<Route*> routes = getRoutesFromPortOnDate(current.portName, date);
            for (size_t i = 0; i < routes.size(); ++i) {
                Route* route = routes[i];
                std::string neighbor = route->destination;

                if (visited.contains(neighbor)) continue;

                double newCost = current.cost + route->getVoyageDuration();

                if (!dist.contains(neighbor) || newCost < dist.at(neighbor)) {
                    dist.insert(neighbor, newCost);
                    auto newNode = new PathFinding::DijkstraNode(neighbor, newCost, route, nodes.at(current.portName));
                    nodes.insert(neighbor, newNode);
                    pq.push(*newNode);
                }
            }
        }

        // Reconstruct path
        LinkedList<Route*> path;
        if (nodes.contains(destination)) {
            path = PathFinding::reconstructPath(nodes.at(destination));
        }

        // Cleanup nodes
        auto nodeKeys = nodes.keys();
        for (size_t i = 0; i < nodeKeys.size(); ++i) {
            delete nodes.at(nodeKeys[i]);
        }

        return path;
    }

    // Dijkstra's algorithm for cheapest path (cost-based)
    LinkedList<Route*> findCheapestPathDijkstra(const std::string& origin,
        const std::string& destination,
        const std::string& date) const {
        if (!ports.contains(origin) || !ports.contains(destination)) {
            return LinkedList<Route*>();
        }

        Map<std::string, double> dist;
        Map<std::string, PathFinding::DijkstraNode*> nodes;
        Set<std::string> visited;
        PriorityQueue<PathFinding::DijkstraNode> pq;

        // Initialize
        auto startNode = new PathFinding::DijkstraNode(origin, 0.0);
        nodes.insert(origin, startNode);
        dist.insert(origin, 0.0);
        pq.push(*startNode);

        while (!pq.empty()) {
            PathFinding::DijkstraNode current = pq.top();
            pq.pop();

            if (visited.contains(current.portName)) continue;
            visited.insert(current.portName);

            if (current.portName == destination) {
                break;
            }

            // Get all routes from current port
            Vector<Route*> routes = getRoutesFromPortOnDate(current.portName, date);
            for (size_t i = 0; i < routes.size(); ++i) {
                Route* route = routes[i];
                std::string neighbor = route->destination;

                if (visited.contains(neighbor)) continue;

                double newCost = current.cost + route->cost;

                if (!dist.contains(neighbor) || newCost < dist.at(neighbor)) {
                    dist.insert(neighbor, newCost);
                    auto newNode = new PathFinding::DijkstraNode(neighbor, newCost, route, nodes.at(current.portName));
                    nodes.insert(neighbor, newNode);
                    pq.push(*newNode);
                }
            }
        }

        // Reconstruct path
        LinkedList<Route*> path;
        if (nodes.contains(destination)) {
            path = PathFinding::reconstructPath(nodes.at(destination));
        }

        // Cleanup nodes
        auto nodeKeys = nodes.keys();
        for (size_t i = 0; i < nodeKeys.size(); ++i) {
            delete nodes.at(nodeKeys[i]);
        }

        return path;
    }

    // A* algorithm for optimal path finding
    LinkedList<Route*> findPathAStar(const std::string& origin,
        const std::string& destination,
        const std::string& date,
        bool useCost = false) const {
        if (!ports.contains(origin) || !ports.contains(destination)) {
            return LinkedList<Route*>();
        }

        Map<std::string, double> costSoFar;
        Map<std::string, PathFinding::AStarNode*> nodes;
        Set<std::string> visited;
        PriorityQueue<PathFinding::AStarNode> pq;

        // Initialize
        double heuristic = useCost ? PathFinding::costHeuristic(origin, destination)
            : PathFinding::timeHeuristic(origin, destination);

        auto startNode = new PathFinding::AStarNode(origin, 0.0, heuristic);
        nodes.insert(origin, startNode);
        costSoFar.insert(origin, 0.0);
        pq.push(*startNode);

        while (!pq.empty()) {
            PathFinding::AStarNode current = pq.top();
            pq.pop();

            if (visited.contains(current.portName)) continue;
            visited.insert(current.portName);

            if (current.portName == destination) {
                break;
            }

            // Get all routes from current port
            Vector<Route*> routes = getRoutesFromPortOnDate(current.portName, date);
            for (size_t i = 0; i < routes.size(); ++i) {
                Route* route = routes[i];
                std::string neighbor = route->destination;

                if (visited.contains(neighbor)) continue;

                double newCost = current.cost + (useCost ? route->cost : route->getVoyageDuration());
                double newHeuristic = useCost ? PathFinding::costHeuristic(neighbor, destination)
                    : PathFinding::timeHeuristic(neighbor, destination);

                if (!costSoFar.contains(neighbor) || newCost < costSoFar.at(neighbor)) {
                    costSoFar.insert(neighbor, newCost);
                    auto newNode = new PathFinding::AStarNode(neighbor, newCost, newHeuristic, route, nodes.at(current.portName));
                    nodes.insert(neighbor, newNode);
                    pq.push(*newNode);
                }
            }
        }

        // Reconstruct path
        LinkedList<Route*> path;
        if (nodes.contains(destination)) {
            path = PathFinding::reconstructPath(nodes.at(destination));
        }

        // Cleanup nodes
        auto nodeKeys = nodes.keys();
        for (size_t i = 0; i < nodeKeys.size(); ++i) {
            delete nodes.at(nodeKeys[i]);
        }

        return path;
    }

    // Calculate total metrics for a path
    void calculatePathMetrics(const LinkedList<Route*>& path,
        double& totalCost,
        double& totalTime,
        double& totalLayoverCost) const {
        totalCost = 0.0;
        totalTime = 0.0;
        totalLayoverCost = 0.0;

        if (path.empty()) return;

        Route* prevRoute = nullptr;
        for (auto it = path.begin(); it != path.end(); ++it) {
            Route* route = *it;
            totalCost += route->cost;
            totalTime += route->getVoyageDuration();

            // Calculate layover cost between routes
            if (prevRoute) {
                DateTime prevArrival = prevRoute->getArrivalDateTime();
                DateTime currentDeparture = route->getDepartureDateTime();
                double layoverHours = prevArrival.hoursDifference(currentDeparture);

                if (layoverHours > 12.0) {
                    totalLayoverCost += calculateLayoverCost(prevRoute->destination, layoverHours);
                }
            }

            prevRoute = route;
        }
    }

    // Getters
    Vector<std::string> getAllPorts() const {
        return ports.keys();
    }

    Vector<Route*> getRoutesFromPort(const std::string& portName) const {
        Vector<Route*> result;
        if (!ports.contains(portName)) return result;

        Port* port = ports.at(portName);
        for (auto it = port->outgoingRoutes.begin(); it != port->outgoingRoutes.end(); ++it) {
            result.push_back(*it);
        }
        return result;
    }

    double getPortCharge(const std::string& portName) const {
        if (!ports.contains(portName)) return 0.0;
        return ports.at(portName)->charge;
    }

    // Queue management
    bool addShipToPortQueue(const std::string& portName, Ship* ship) {
        if (!ports.contains(portName)) return false;

        if (!portQueues.contains(portName)) {
            portQueues.insert(portName, Queue<Ship*>());
        }

        portQueues.at(portName).push(ship);
        return true;
    }

    Ship* processNextShip(const std::string& portName) {
        if (!portQueues.contains(portName) || portQueues.at(portName).empty()) {
            return nullptr;
        }

        Ship* nextShip = portQueues.at(portName).front();
        portQueues.at(portName).pop();
        return nextShip;
    }

    Vector<std::string> getPortQueueStatus(const std::string& portName) const {
        Vector<std::string> result;

        if (portQueues.contains(portName)) {
            result.push_back("Queue size: " + std::to_string(portQueues.at(portName).size()));
        }
        else {
            result.push_back("No queue for this port");
        }

        return result;
    }
};

#endif