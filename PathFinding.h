#ifndef PATHFINDING_H
#define PATHFINDING_H

#include "GraphStructures.h"
#include "Map.h"
#include "Vector.h"
#include "LinkedList.h"
#include "PriorityQueue.h"
#include "Set.h"
#include <cmath>

class PathFinding {
public:
    struct DijkstraNode {
        std::string portName;
        double cost;
        Route* routeUsed;
        DijkstraNode* previous;

        DijkstraNode(const std::string& name = "", double c = 0.0,
            Route* route = nullptr, DijkstraNode* prev = nullptr)
            : portName(name), cost(c), routeUsed(route), previous(prev) {
        }

        bool operator<(const DijkstraNode& other) const {
            return cost < other.cost;
        }

        bool operator>(const DijkstraNode& other) const {
            return cost > other.cost;
        }
    };

    struct AStarNode {
        std::string portName;
        double cost;
        double heuristic;
        double totalCost;
        Route* routeUsed;
        AStarNode* previous;

        AStarNode(const std::string& name = "", double c = 0.0, double h = 0.0,
            Route* route = nullptr, AStarNode* prev = nullptr)
            : portName(name), cost(c), heuristic(h), totalCost(c + h),
            routeUsed(route), previous(prev) {
        }

        bool operator<(const AStarNode& other) const {
            return totalCost < other.totalCost;
        }

        bool operator>(const AStarNode& other) const {
            return totalCost > other.totalCost;
        }
    };

    static LinkedList<Route*> reconstructPath(DijkstraNode* node) {
        LinkedList<Route*> path;
        DijkstraNode* current = node;
        while (current && current->routeUsed) {
            path.push_front(current->routeUsed);
            current = current->previous;
        }
        return path;
    }

    static LinkedList<Route*> reconstructPath(AStarNode* node) {
        LinkedList<Route*> path;
        AStarNode* current = node;
        while (current && current->routeUsed) {
            path.push_front(current->routeUsed);
            current = current->previous;
        }
        return path;
    }

    // Simple heuristic based on port name length (placeholder for real distance)
    static double distanceHeuristic(const std::string& current, const std::string& target) {
        // Simple heuristic: assume ports with similar names are closer
        // This is a placeholder - in reality you'd use actual coordinates
        int diff = std::abs(static_cast<int>(current.length()) - static_cast<int>(target.length()));
        return diff * 100.0; // Scale factor
    }

    static double costHeuristic(const std::string& current, const std::string& target) {
        // For cost-based search, use a small constant heuristic
        return 0.0;
    }

    static double timeHeuristic(const std::string& current, const std::string& target) {
        // For time-based search, use a small constant heuristic
        return 0.0;
    }
};

#endif