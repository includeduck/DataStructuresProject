#ifndef MAIN_H
#define MAIN_H

#include "MaritimeSystem.h"
#include "Ship.h"
#include <iostream>
#include <string>
#include <limits>

class MenuSystem {
private:
    MaritimeSystem system;
    Ship* currentShip;

    void clearInputBuffer() {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    std::string getStringInput(const std::string& prompt) {
        std::string input;
        while (true) {
            std::cout << prompt;
            std::getline(std::cin, input);

            if (!input.empty()) {
                return input;
            }
            std::cout << "Error: Input cannot be empty. Please try again.\n";
        }
    }

    double getDoubleInput(const std::string& prompt) {
        double value;
        while (true) {
            std::cout << prompt;
            std::cin >> value;

            if (std::cin.fail()) {
                std::cin.clear();
                clearInputBuffer();
                std::cout << "Error: Invalid input. Please enter a valid number.\n";
            }
            else {
                clearInputBuffer();
                return value;
            }
        }
    }

    int getIntInput(const std::string& prompt, int min = INT_MIN, int max = INT_MAX) {
        int value;
        while (true) {
            std::cout << prompt;
            std::cin >> value;

            if (std::cin.fail()) {
                std::cin.clear();
                clearInputBuffer();
                std::cout << "Error: Invalid input. Please enter a valid integer.\n";
            }
            else if (value < min || value > max) {
                std::cout << "Error: Please enter a value between " << min << " and " << max << ".\n";
            }
            else {
                clearInputBuffer();
                return value;
            }
        }
    }

    std::string getDateInput(const std::string& prompt) {
        std::string date;
        while (true) {
            date = getStringInput(prompt);
            if (DateTime::isValidDate(date)) {
                return date;
            }
            std::cout << "Error: Invalid date format. Please use dd/mm/yyyy format.\n";
        }
    }

    void displayPath(const LinkedList<Route*>& path, const std::string& pathType) {
        if (path.empty()) {
            std::cout << "No " << pathType << " path found.\n";
            return;
        }

        double totalCost, totalTime, totalLayoverCost;
        system.calculatePathMetrics(path, totalCost, totalTime, totalLayoverCost);
        double totalExpense = totalCost + totalLayoverCost;

        std::cout << "\n=== " << pathType << " Path Found ===\n";
        std::cout << "Total Voyage Time: " << totalTime << " hours\n";
        std::cout << "Total Voyage Cost: $" << totalCost << "\n";
        std::cout << "Total Layover Cost: $" << totalLayoverCost << "\n";
        std::cout << "Total Expense: $" << totalExpense << "\n\n";

        std::cout << "Route Details:\n";
        int leg = 1;
        Route* prevRoute = nullptr;

        for (auto it = path.begin(); it != path.end(); ++it) {
            Route* route = *it;
            std::cout << "Leg " << leg << ":\n";
            std::cout << "  " << route->origin << " -> " << route->destination << "\n";
            std::cout << "  Company: " << route->company << "\n";
            std::cout << "  Departure: " << route->date << " " << route->departureTime << "\n";
            std::cout << "  Arrival: " << route->getArrivalDateTime().getDate() << " " << route->arrivalTime << "\n";
            std::cout << "  Duration: " << route->getVoyageDuration() << " hours\n";
            std::cout << "  Cost: $" << route->cost << "\n";

            // Show layover information
            if (prevRoute) {
                DateTime prevArrival = prevRoute->getArrivalDateTime();
                DateTime currentDeparture = route->getDepartureDateTime();
                double layoverHours = prevArrival.hoursDifference(currentDeparture);

                std::cout << "  Layover at " << prevRoute->destination << ": " << layoverHours << " hours";
                if (layoverHours > 12.0) {
                    std::cout << " (Port charges apply: $" << system.calculateLayoverCost(prevRoute->destination, layoverHours) << ")";
                }
                std::cout << "\n";
            }
            std::cout << "\n";

            prevRoute = route;
            leg++;
        }

        // Ask if user wants to book this path for current ship
        if (currentShip) {
            std::cout << "Would you like to book this path for your current ship? (y/n): ";
            char choice;
            std::cin >> choice;
            clearInputBuffer();

            if (choice == 'y' || choice == 'Y') {
                currentShip->clearJourney();
                for (auto it = path.begin(); it != path.end(); ++it) {
                    currentShip->addRouteToJourney(*it);
                }
                std::cout << "Path booked successfully for ship '" << currentShip->name << "'!\n";
            }
        }
    }

    void displayMainMenu() {
        std::cout << "\n=== OceanRoute Nav - Maritime Navigation Optimizer ===\n";
        std::cout << "1. Find Direct Routes\n";
        std::cout << "2. Find Connecting Routes\n";
        std::cout << "3. Find Shortest Path (Time - Dijkstra)\n";
        std::cout << "4. Find Cheapest Path (Cost - Dijkstra)\n";
        std::cout << "5. Find Optimal Path (A* Algorithm)\n";
        std::cout << "6. Ship Management\n";
        std::cout << "7. Port Queue Management\n";
        std::cout << "8. Multi-leg Journey Planning\n";
        std::cout << "9. System Information\n";
        std::cout << "10. Exit\n";
        std::cout << "Choose option (1-10): ";
    }

    void handleDirectRoutes() {
        std::cout << "\n--- Find Direct Routes ---\n";

        std::string origin = getStringInput("Enter origin port: ");
        std::string destination = getStringInput("Enter destination port: ");
        std::string date = getDateInput("Enter voyage date (dd/mm/yyyy): ");

        auto routes = system.findDirectRoutes(origin, destination, date);

        std::cout << "\nFound " << routes.size() << " direct route(s):\n";
        for (size_t i = 0; i < routes.size(); ++i) {
            Route* route = routes[i];
            std::cout << "  " << (i + 1) << ". " << route->company
                << " | " << route->departureTime << " - " << route->arrivalTime
                << " | Cost: $" << route->cost
                << " | Duration: " << route->getVoyageDuration() << " hours\n";
        }

        if (routes.empty()) {
            std::cout << "No direct routes found for the given criteria.\n";
        }
    }

    void handleConnectingRoutes() {
        std::cout << "\n--- Find Connecting Routes ---\n";

        std::string origin = getStringInput("Enter origin port: ");
        std::string destination = getStringInput("Enter destination port: ");
        std::string date = getDateInput("Enter voyage date (dd/mm/yyyy): ");

        auto routes = system.findConnectingRoutes(origin, destination, date);

        std::cout << "\nFound " << routes.size() << " connecting route(s):\n";
        for (size_t i = 0; i < routes.size(); ++i) {
            std::cout << "Route " << (i + 1) << ":\n";
            double totalCost = 0.0;
            double totalTime = 0.0;

            int leg = 1;
            for (auto it = routes[i].begin(); it != routes[i].end(); ++it) {
                Route* route = *it;
                std::cout << "  Leg " << leg << ": " << route->origin << " -> " << route->destination
                    << " | " << route->company << " | " << route->departureTime << " - " << route->arrivalTime
                    << " | Cost: $" << route->cost << "\n";
                totalCost += route->cost;
                totalTime += route->getVoyageDuration();
                leg++;
            }
            std::cout << "  Total Cost: $" << totalCost << " | Total Time: " << totalTime << " hours\n\n";
        }

        if (routes.empty()) {
            std::cout << "No connecting routes found for the given criteria.\n";
        }
    }

    void handleShortestPath() {
        std::cout << "\n--- Find Shortest Path (Dijkstra - Time Based) ---\n";

        std::string origin = getStringInput("Enter origin port: ");
        std::string destination = getStringInput("Enter destination port: ");
        std::string date = getDateInput("Enter voyage date (dd/mm/yyyy): ");

        auto path = system.findShortestPathDijkstra(origin, destination, date);
        displayPath(path, "Shortest Time");
    }

    void handleCheapestPath() {
        std::cout << "\n--- Find Cheapest Path (Dijkstra - Cost Based) ---\n";

        std::string origin = getStringInput("Enter origin port: ");
        std::string destination = getStringInput("Enter destination port: ");
        std::string date = getDateInput("Enter voyage date (dd/mm/yyyy): ");

        auto path = system.findCheapestPathDijkstra(origin, destination, date);
        displayPath(path, "Cheapest Cost");
    }

    void handleAStarPath() {
        std::cout << "\n--- Find Optimal Path (A* Algorithm) ---\n";

        std::string origin = getStringInput("Enter origin port: ");
        std::string destination = getStringInput("Enter destination port: ");
        std::string date = getDateInput("Enter voyage date (dd/mm/yyyy): ");

        std::cout << "Optimize for:\n";
        std::cout << "1. Time (Fastest Route)\n";
        std::cout << "2. Cost (Most Economical)\n";
        int choice = getIntInput("Choose optimization (1-2): ", 1, 2);

        bool useCost = (choice == 2);
        auto path = system.findPathAStar(origin, destination, date, useCost);

        std::string pathType = useCost ? "Most Economical (A*)" : "Fastest (A*)";
        displayPath(path, pathType);
    }

    void handleShipManagement() {
        std::cout << "\n--- Ship Management ---\n";

        if (!currentShip) {
            std::cout << "1. Create New Ship\n";
            std::cout << "2. Back to Main Menu\n";

            int choice = getIntInput("Choose option: ", 1, 2);

            if (choice == 1) {
                std::string name = getStringInput("Enter ship name: ");
                std::string company = getStringInput("Enter preferred company (leave empty for any): ");
                double maxTime = getDoubleInput("Enter maximum voyage time in hours (0 for no limit): ");

                currentShip = new Ship(name, company, maxTime);
                std::cout << "Ship '" << name << "' created successfully!\n";
            }
        }
        else {
            std::cout << "Current Ship: " << currentShip->name << "\n";
            std::cout << "1. View Ship Details\n";
            std::cout << "2. Set Preferred Company\n";
            std::cout << "3. Set Maximum Voyage Time\n";
            std::cout << "4. Manage Avoided Ports\n";
            std::cout << "5. View Current Journey\n";
            std::cout << "6. Clear Current Ship\n";
            std::cout << "7. Back to Main Menu\n";

            int choice = getIntInput("Choose option: ", 1, 7);

            switch (choice) {
            case 1:
                std::cout << "Ship Name: " << currentShip->name << "\n";
                std::cout << "Preferred Company: " << (currentShip->company.empty() ? "Any" : currentShip->company) << "\n";
                std::cout << "Max Voyage Time: " << (currentShip->maxVoyageTime > 0 ? std::to_string(currentShip->maxVoyageTime) + " hours" : "No limit") << "\n";
                std::cout << "Current Journey Legs: " << currentShip->currentJourney.size() << "\n";
                break;
            case 2: {
                std::string company = getStringInput("Enter preferred company (leave empty for any): ");
                currentShip->company = company;
                std::cout << "Preferred company updated.\n";
                break;
            }
            case 3: {
                double maxTime = getDoubleInput("Enter maximum voyage time in hours (0 for no limit): ");
                currentShip->maxVoyageTime = maxTime;
                std::cout << "Maximum voyage time updated.\n";
                break;
            }
            case 4:
                handleAvoidedPorts();
                break;
            case 5:
                if (currentShip->currentJourney.empty()) {
                    std::cout << "No journey planned for this ship.\n";
                }
                else {
                    std::cout << "Current Journey for " << currentShip->name << ":\n";
                    double totalCost = currentShip->getTotalJourneyCost();
                    double totalTime = currentShip->getTotalJourneyTime();
                    std::cout << "Total Time: " << totalTime << " hours\n";
                    std::cout << "Total Cost: $" << totalCost << "\n";
                }
                break;
            case 6:
                delete currentShip;
                currentShip = nullptr;
                std::cout << "Current ship cleared.\n";
                break;
            }
        }
    }

    void handleAvoidedPorts() {
        std::cout << "\n--- Manage Avoided Ports ---\n";

        while (true) {
            std::cout << "Current avoided ports: ";
            if (currentShip->avoidedPorts.empty()) {
                std::cout << "None\n";
            }
            else {
                for (auto it = currentShip->avoidedPorts.begin(); it != currentShip->avoidedPorts.end(); ++it) {
                    std::cout << *it << " ";
                }
                std::cout << "\n";
            }

            std::cout << "1. Add Port to Avoid\n";
            std::cout << "2. Remove Port from Avoid List\n";
            std::cout << "3. Back to Ship Management\n";

            int choice = getIntInput("Choose option: ", 1, 3);

            if (choice == 1) {
                std::string port = getStringInput("Enter port name to avoid: ");
                currentShip->addAvoidedPort(port);
                std::cout << "Port '" << port << "' added to avoid list.\n";
            }
            else if (choice == 2) {
                if (currentShip->avoidedPorts.empty()) {
                    std::cout << "No ports to remove.\n";
                }
                else {
                    std::string port = getStringInput("Enter port name to remove from avoid list: ");
                    currentShip->removeAvoidedPort(port);
                    std::cout << "Port '" << port << "' removed from avoid list.\n";
                }
            }
            else {
                break;
            }
        }
    }

    void handlePortQueues() {
        std::cout << "\n--- Port Queue Management ---\n";

        std::cout << "1. View Port Queue Status\n";
        std::cout << "2. Add Ship to Port Queue\n";
        std::cout << "3. Process Next Ship in Queue\n";
        std::cout << "4. Back to Main Menu\n";

        int choice = getIntInput("Choose option: ", 1, 4);

        switch (choice) {
        case 1: {
            std::string portName = getStringInput("Enter port name: ");
            auto status = system.getPortQueueStatus(portName);
            for (size_t i = 0; i < status.size(); ++i) {
                std::cout << status[i] << "\n";
            }
            break;
        }
        case 2: {
            if (!currentShip) {
                std::cout << "Error: No current ship selected. Please create a ship first.\n";
                break;
            }
            std::string portName = getStringInput("Enter port name: ");
            if (system.addShipToPortQueue(portName, currentShip)) {
                std::cout << "Ship '" << currentShip->name << "' added to " << portName << " queue.\n";
            }
            else {
                std::cout << "Error: Could not add ship to queue. Port may not exist.\n";
            }
            break;
        }
        case 3: {
            std::string portName = getStringInput("Enter port name: ");
            Ship* processedShip = system.processNextShip(portName);
            if (processedShip) {
                std::cout << "Processed ship: " << processedShip->name << "\n";
            }
            else {
                std::cout << "No ships in queue for " << portName << "\n";
            }
            break;
        }
        }
    }

    void handleMultiLegJourney() {
        std::cout << "\n--- Multi-leg Journey Planning ---\n";
        std::cout << "This feature allows you to plan complex multi-stop journeys.\n";

        if (!currentShip) {
            std::cout << "Error: Please create a ship first to plan a journey.\n";
            return;
        }

        std::cout << "Current stops in journey: " << currentShip->currentJourney.size() << "\n";
        std::cout << "1. Add Leg to Journey\n";
        std::cout << "2. Clear Journey\n";
        std::cout << "3. View Journey Details\n";
        std::cout << "4. Back to Main Menu\n";

        int choice = getIntInput("Choose option: ", 1, 4);

        switch (choice) {
        case 1: {
            std::string origin, destination, date;
            if (currentShip->currentJourney.empty()) {
                origin = getStringInput("Enter starting port: ");
            }
            else {
                origin = currentShip->currentJourney.back()->destination;
                std::cout << "Next leg starts from: " << origin << "\n";
            }

            destination = getStringInput("Enter destination port: ");
            date = getDateInput("Enter voyage date (dd/mm/yyyy): ");

            auto routes = system.findDirectRoutes(origin, destination, date);
            if (routes.empty()) {
                std::cout << "No direct routes found. Try connecting routes instead.\n";
            }
            else {
                std::cout << "Available routes:\n";
                for (size_t i = 0; i < routes.size(); ++i) {
                    Route* route = routes[i];
                    std::cout << "  " << (i + 1) << ". " << route->company
                        << " | " << route->departureTime << " - " << route->arrivalTime
                        << " | Cost: $" << route->cost << "\n";
                }

                int routeChoice = getIntInput("Select route (0 to cancel): ", 0, routes.size());
                if (routeChoice > 0) {
                    currentShip->addRouteToJourney(routes[routeChoice - 1]);
                    std::cout << "Leg added to journey.\n";
                }
            }
            break;
        }
        case 2:
            currentShip->clearJourney();
            std::cout << "Journey cleared.\n";
            break;
        case 3:
            if (currentShip->currentJourney.empty()) {
                std::cout << "No journey planned.\n";
            }
            else {
                std::cout << "Current Journey:\n";
                int leg = 1;
                for (auto it = currentShip->currentJourney.begin(); it != currentShip->currentJourney.end(); ++it) {
                    Route* route = *it;
                    std::cout << "Leg " << leg << ": " << route->origin << " -> " << route->destination
                        << " (" << route->company << ")\n";
                    leg++;
                }
                std::cout << "Total Time: " << currentShip->getTotalJourneyTime() << " hours\n";
                std::cout << "Total Cost: $" << currentShip->getTotalJourneyCost() << "\n";
            }
            break;
        }
    }

    void handleSystemInfo() {
        std::cout << "\n--- System Information ---\n";

        auto ports = system.getAllPorts();
        std::cout << "Total Ports: " << ports.size() << "\n";

        std::cout << "\nAvailable Ports:\n";
        for (size_t i = 0; i < ports.size(); ++i) {
            std::cout << "  " << ports[i] << " - Charge: $" << system.getPortCharge(ports[i]) << "/day\n";
        }

        if (currentShip) {
            std::cout << "\nCurrent Ship: " << currentShip->name << "\n";
            std::cout << "Journey Legs: " << currentShip->currentJourney.size() << "\n";
        }
        else {
            std::cout << "\nNo current ship selected.\n";
        }
    }

public:
    MenuSystem() : currentShip(nullptr) {
        // Load data on initialization
        std::cout << "Loading maritime data...\n";
        if (!system.loadPortCharges("PortCharges.txt")) {
            std::cout << "Warning: Could not load port charges data.\n";
        }
        if (!system.loadRoutes("Routes.txt")) {
            std::cout << "Warning: Could not load routes data.\n";
        }
    }

    ~MenuSystem() {
        if (currentShip) {
            delete currentShip;
        }
    }

    void run() {
        std::cout << "=== Welcome to OceanRoute Nav ===\n";
        std::cout << "Maritime Navigation and Logistics Visualization System\n";

        while (true) {
            displayMainMenu();

            int choice = getIntInput("", 1, 10);

            switch (choice) {
            case 1: handleDirectRoutes(); break;
            case 2: handleConnectingRoutes(); break;
            case 3: handleShortestPath(); break;
            case 4: handleCheapestPath(); break;
            case 5: handleAStarPath(); break;
            case 6: handleShipManagement(); break;
            case 7: handlePortQueues(); break;
            case 8: handleMultiLegJourney(); break;
            case 9: handleSystemInfo(); break;
            case 10:
                std::cout << "Thank you for using OceanRoute Nav. Goodbye!\n";
                return;
            }

            std::cout << "\nPress Enter to continue...";
            std::cin.get();
        }
    }
};

#endif