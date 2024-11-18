#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <algorithm>

using namespace std;

class Route {
public:
    string sourceCity;
    string destinationCity;
    int capacity;
    double time;
    string mode;
    double cost;                // Cost of the route in USD
    double environmentalImpact; // Environmental impact in kg CO₂ per person
};

class CityNetwork {
public:
    vector<Route> routes;
    set<string> validCities;

    void addRoute(string sourceCity, string destinationCity, int capacity, double time, string mode, double cost, double environmentalImpact) {
        routes.push_back({sourceCity, destinationCity, capacity, time, mode, cost, environmentalImpact});
        validCities.insert(sourceCity);
        validCities.insert(destinationCity);
    }

    bool isValidCity(const string& city) {
        // Allow "DC" to be valid and treat it as "Washington DC"
        if (city == "NYC" || city == "Boston" || city == "Philadelphia" || city == "DC") return true;
        return validCities.find(city) != validCities.end();
    }

    void calculateMaxFlow(string sourceCity, string destinationCity) {
        bool routeFound = false;
        cout << "\nCalculating maximum flow of people between " << sourceCity << " and " << destinationCity << ":\n";

        for (Route& route : routes) {
            if (route.sourceCity == sourceCity && route.destinationCity == destinationCity) {
                routeFound = true;
                cout << "Mode of Transport: " << route.mode << "| ";
                cout << "Capacity: " << route.capacity << " people | ";
                cout << "Travel Time: " << route.time << " hours | Cost: $" << route.cost
                     << " | Environmental Impact: " << route.environmentalImpact << " kg CO₂\n";
            }
        }

        if (!routeFound) {
            cout << "No available routes found between " << sourceCity << " and " << destinationCity << ".\n";
        } else {
            int totalCapacity = 0;
            for (Route& route : routes) {
                if (route.sourceCity == sourceCity && route.destinationCity == destinationCity) {
                    totalCapacity += route.capacity;
                }
            }
            cout << "Maximum flow of people between " << sourceCity << " and " << destinationCity << ": " << totalCapacity << "\n";
        }
    }

    void bookTrip(string sourceCity, string destinationCity, int modeChoice, int numPeople) {
        string mode;
        if (modeChoice == 1) mode = "Airplane";
        else if (modeChoice == 2) mode = "Train";
        else if (modeChoice == 3) mode = "Bus";
        else {
            cout << "Invalid mode of transportation selected! Please select 1, 2, or 3.\n";
            return;
        }

        bool routeFound = false;
        for (Route& route : routes) {
            if (route.sourceCity == sourceCity && route.destinationCity == destinationCity && route.mode == mode) {
                routeFound = true;
                if (numPeople <= route.capacity) {
                    cout << "Booking successful! You have booked " << numPeople << " tickets for the " << mode
                         << " from " << sourceCity << " to " << destinationCity << ".\n";
                } else {
                    cout << "Error: Not enough available seats for the selected mode of transportation. Maximum capacity is "
                         << route.capacity << " people.\n";
                }
                return;
            }
        }

        if (!routeFound) {
            cout << "No route found between " << sourceCity << " and " << destinationCity << " for the selected mode of transportation.\n";
        }
    }

    string getCityInput() {
        string city;
        getline(cin, city);  // To allow spaces in city names
        return city;
    }

    bool exitProgram(const string& input) {
        return input == "0";
    }

    void getCityConnections(string city) {
        cout << "\nCities directly connected to " << city << ":\n";
        bool connectionFound = false;
        for (Route& route : routes) {
            if (route.sourceCity == city) {
                connectionFound = true;
                cout << "Destination: " << route.destinationCity << " | Mode: " << route.mode
                     << " | Travel Time: " << route.time << " hours | Cost: $" << route.cost << "\n";
            }
        }
        if (!connectionFound) {
            cout << "No direct connections found for " << city << ".\n";
        }
    }

    void getBusiestRoutes() {
        if (routes.empty()) {
            cout << "No routes available in the network.\n";
            return;
        }

        sort(routes.begin(), routes.end(), [](Route a, Route b) {
            return a.capacity > b.capacity;
        });

        cout << "\nBusiest Routes by Capacity:\n";
        int limit = routes.size() < 5 ? routes.size() : 5;
        for (int i = 0; i < limit; i++) {
            Route route = routes[i];
            cout << route.sourceCity << " to " << route.destinationCity << " | Mode: " << route.mode
                 << " | Capacity: " << route.capacity << " people | Travel Time: " << route.time << " hours\n";
        }
    }

    double calculateRouteScore(const Route& route, double timeWeight, double costWeight, double impactWeight) {
        return (route.time * timeWeight) + (route.cost * costWeight) + (route.environmentalImpact * impactWeight);
    }

    void calculateOptimizedFlow(string sourceCity, string destinationCity, double timeWeight, double costWeight, double impactWeight) {
        vector<Route> validRoutes;

        for (const Route& route : routes) {
            if (route.sourceCity == sourceCity && route.destinationCity == destinationCity) {
                validRoutes.push_back(route);
            }
        }

        if (validRoutes.empty()) {
            cout << "No routes found between " << sourceCity << " and " << destinationCity << ".\n";
            return;
        }

        sort(validRoutes.begin(), validRoutes.end(), [&](const Route& a, const Route& b) {
            return calculateRouteScore(a, timeWeight, costWeight, impactWeight) < calculateRouteScore(b, timeWeight, costWeight, impactWeight);
        });

        const Route& bestRoute = validRoutes.front();
        cout << "\nBest Route from " << sourceCity << " to " << destinationCity << ":\n";
        cout << "Mode: " << bestRoute.mode << " | Time: " << bestRoute.time << " hours | Cost: $" << bestRoute.cost
             << " | Environmental Impact: " << bestRoute.environmentalImpact << " kg CO₂ | Capacity: "
             << bestRoute.capacity << " people\n";
    }
};

void print_map(){
    cout << "||==========================================================================================||" << endl;
    cout << "||     |     |             |                              |        |        /       ___/    ||" << endl;
    cout << "||     |     \\              |              ______________/         |_______/_______/ _/     ||" << endl;
    cout << "||_____|      \\             |             /                        |               _/       ||" << endl;
    cout << "||     \\     _/            /           __/                         |         BOS o|_        ||" << endl;
    cout << "|| CHI o\\___/____________/       _____/  |__________________       |_____________   \\   \\   ||" << endl;
    cout << "||      |           |     \\_____/     |                     \\_     |          |  \\   \\___\\  ||" << endl;
    cout << "||      |           |                 |                       /--__|  ________|__/\\___/   \\ ||" << endl;
    cout << "||      |           |                 |                      /NYC o/_/_________             ||" << endl;
    cout << "||      |           |                /|                      \\   /_ /______/                ||" << endl;
    cout << "||      |           |               | |                  PHI o\\    |                        ||" << endl;
    cout << "||      |           |              _/ |_______________________/    /                        ||" << endl;
    cout << "||      |       __/ \\____      __/        |__/ \\__      _/||\\ \\___/                         ||" << endl;
    cout << "||      /   ___/         \\____/              ____/\\WDC o| || \\  //                          ||" << endl;
    cout << "||     /___/                |              _/        \\_ | ||__|                             ||" << endl;
    cout << "||     |                    \\             /            \\|  \\__/                             ||" << endl;
    cout << "||==========================================================================================||" << endl;
}

bool display_routes( string source, string destination ){
    string cities[2] = {source, destination};
    string firstLine = "";
    string secondLine = "";
    for(int i = 0; i < 2; i++ ){
        if( cities[i].compare("wdc") == 0 || cities[i].compare("WDC") == 0 ){
            firstLine += "\tWashington DC\t\t";
            secondLine += "  \"The Nation's Capital\"\t";
        }else if( cities[i].compare("phi") == 0 || cities[i].compare("PHI") == 0 ){
            firstLine += "\tPhiladelphia\t\t";
            secondLine += "  \"The City of Brotherly Love\"\t";
        }else if( cities[i].compare("nyc") == 0 || cities[i].compare("NYC") == 0 ){
            firstLine += "\tNew York City\t\t";
            secondLine += "  \"The Big Apple\"\t";
        }else if( cities[i].compare("bos") == 0 || cities[i].compare("BOS") == 0 ){
            firstLine += "\tBoston\t\t";
            secondLine += "\t\"Beantown\"\t";
        }else if( cities[i].compare("chi") == 0 || cities[i].compare("CHI") == 0 ){
            firstLine += "\tChicago\t\t";
            secondLine += "  \"The Windy City\"\t";
        }else{
            return false;
        }
        if( i == 0 ){
            firstLine += "-->";
        }
    }
    cout << firstLine << "\n" << secondLine;    
    return true;
}

string getCity( string input ){
    if( input.compare("Washington") == 0 || input.compare("DC") == 0 || input.compare("WDC") == 0 || input.compare("wdc") == 0 || input.compare("Washington DC") == 0){
        return "DC";
    }else if( input.compare("Philadelphia") == 0 || input.compare("Philly") == 0 || input.compare("PHI") == 0 || input.compare("phi") == 0 ){
        return "Philadelphia";
    }else if( input.compare("NYC") == 0 || input.compare("New York City") == 0 || input.compare("nyc") == 0 || input.compare("New York") == 0 ){
        return "NYC";
    }else if( input.compare("Chicago") == 0 || input.compare("CHI") == 0 || input.compare("chi") == 0 ){
        return "Chicago";
    }else{
        return "";
    }
}

int main() {
    CityNetwork network;

    network.addRoute("NYC", "Chicago", 200, 2, "Airplane", 300, 500);
    network.addRoute("NYC", "Chicago", 100, 24, "Train", 150, 300);
    network.addRoute("NYC", "Chicago", 50, 20, "Bus", 100, 120);
    network.addRoute("NYC", "Boston", 200, 1, "Airplane", 200, 400);
    network.addRoute("NYC", "Boston", 100, 4, "Train", 120, 200);
    network.addRoute("NYC", "Boston", 40, 5, "Bus", 60, 70);
    network.addRoute("NYC", "DC", 180, 1.5, "Airplane", 250, 450);
    network.addRoute("NYC", "DC", 120, 3.5, "Train", 150, 250);
    network.addRoute("NYC", "DC", 50, 4, "Bus", 80, 100);
    network.addRoute("NYC", "Philadelphia", 180, 1.5, "Airplane", 220, 400);
    network.addRoute("NYC", "Philadelphia", 80, 2.5, "Train", 100, 150);
    network.addRoute("NYC", "Philadelphia", 50, 3, "Bus", 70, 80);

    network.addRoute("Chicago", "NYC", 200, 2, "Airplane", 300, 500);
    network.addRoute("Chicago", "NYC", 90, 24, "Train", 140, 300);
    network.addRoute("Chicago", "NYC", 60, 20, "Bus", 90, 110);
    network.addRoute("Chicago", "Boston", 180, 1, "Airplane", 250, 400);
    network.addRoute("Chicago", "Boston", 90, 4, "Train", 120, 200);
    network.addRoute("Chicago", "Boston", 70, 5, "Bus", 80, 90);
    network.addRoute("Chicago", "DC", 150, 1.5, "Airplane", 260, 450);
    network.addRoute("Chicago", "DC", 80, 3.5, "Train", 140, 250);
    network.addRoute("Chicago", "DC", 70, 4, "Bus", 90, 110);
    network.addRoute("Chicago", "Philadelphia", 200, 1.5, "Airplane", 280, 500);
    network.addRoute("Chicago", "Philadelphia", 120, 2.5, "Train", 180, 300);
    network.addRoute("Chicago", "Philadelphia", 40, 3, "Bus", 100, 120);

    network.addRoute("Boston", "NYC", 180, 1, "Airplane", 240, 400);
    network.addRoute("Boston", "NYC", 100, 4, "Train", 120, 200);
    network.addRoute("Boston", "NYC", 70, 5, "Bus", 80, 90);
    network.addRoute("Boston", "Chicago", 160, 1, "Airplane", 220, 380);
    network.addRoute("Boston", "Chicago", 90, 4, "Train", 140, 250);
    network.addRoute("Boston", "Chicago", 60, 5, "Bus", 100, 110);
    network.addRoute("Boston", "DC", 130, 1.5, "Airplane", 200, 350);
    network.addRoute("Boston", "DC", 80, 3, "Train", 120, 210);
    network.addRoute("Boston", "DC", 50, 4, "Bus", 80, 90);
    network.addRoute("Boston", "Philadelphia", 150, 1, "Airplane", 210, 370);
    network.addRoute("Boston", "Philadelphia", 120, 2.5, "Train", 160, 280);
    network.addRoute("Boston", "Philadelphia", 50, 3, "Bus", 100, 110);

    network.addRoute("DC", "NYC", 150, 1.5, "Airplane", 270, 450);
    network.addRoute("DC", "NYC", 120, 3.5, "Train", 150, 250);
    network.addRoute("DC", "NYC", 80, 4, "Bus", 90, 100);
    network.addRoute("DC", "Chicago", 130, 1.5, "Airplane", 250, 400);
    network.addRoute("DC", "Chicago", 100, 3, "Train", 170, 300);
    network.addRoute("DC", "Chicago", 60, 4, "Bus", 90, 110);
    network.addRoute("DC", "Boston", 180, 1, "Airplane", 240, 400);
    network.addRoute("DC", "Boston", 90, 3, "Train", 150, 250);
    network.addRoute("DC", "Boston", 70, 4, "Bus", 100, 120);
    network.addRoute("DC", "Philadelphia", 180, 1, "Airplane", 210, 380);
    network.addRoute("DC", "Philadelphia", 100, 2.5, "Train", 130, 220);
    network.addRoute("DC", "Philadelphia", 50, 3, "Bus", 90, 100);

    network.addRoute("Philadelphia", "NYC", 150, 1.5, "Airplane", 230, 400);
    network.addRoute("Philadelphia", "NYC", 90, 2.5, "Train", 110, 180);
    network.addRoute("Philadelphia", "NYC", 60, 3, "Bus", 80, 90);
    network.addRoute("Philadelphia", "Chicago", 140, 1.5, "Airplane", 250, 420);
    network.addRoute("Philadelphia", "Chicago", 70, 3.5, "Train", 180, 320);
    network.addRoute("Philadelphia", "Chicago", 50, 4, "Bus", 100, 120);
    network.addRoute("Philadelphia", "Boston", 130, 1, "Airplane", 220, 370);
    network.addRoute("Philadelphia", "Boston", 90, 3, "Train", 140, 250);
    network.addRoute("Philadelphia", "Boston", 60, 4, "Bus", 90, 100);
    network.addRoute("Philadelphia", "DC", 180, 1.5, "Airplane", 240, 400);
    network.addRoute("Philadelphia", "DC", 120, 2.5, "Train", 150, 250);
    network.addRoute("Philadelphia", "DC", 50, 3, "Bus", 80, 90);


    string source, destination;
    int choice;
    print_map();

    while (true) {
        cout << "Welcome to the Trip Planner." << endl;
        cout << "\nMenu:\n";
        cout << "0. Exit\n";
        cout << "1. Calculate maximum flow of people between two cities\n";
        cout << "2. Book a trip\n";
        cout << "3. Display all cities connected to a specified city\n";
        cout << "4. Display busiest routes by capacity\n";
        cout << "5. Calculate optimized flow based on preferences\n";
        cout << "6. Display map\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore();

        if (choice == 0) {
            cout << "Exiting the program.\n";
            break;
        } else if (choice == 1) {
            cout << "Enter source city: ";
            getline(cin, source);
            source = getCity(source);
            cout << "Enter destination city: ";
            getline(cin, destination);
            destination = getCity(destination);
            network.calculateMaxFlow(source, destination);
        } else if (choice == 2) {
            int modeChoice, numPeople;
            cout << "Enter source city: ";
            getline(cin, source);
            source = getCity(source);
            cout << "Enter destination city: ";
            getline(cin, destination);
            destination = getCity(destination);
            cout << "Choose mode (1: Airplane, 2: Train, 3: Bus): ";
            cin >> modeChoice;
            cout << "Enter number of people: ";
            cin >> numPeople;
            network.bookTrip(source, destination, modeChoice, numPeople);
        } else if (choice == 3) {
            cout << "Enter city: ";
            getline(cin, source);
            source = getCity(source);
            network.getCityConnections(source);
        } else if (choice == 4) {
            network.getBusiestRoutes();
        } else if (choice == 5) {
            double timeWeight, costWeight, impactWeight;
            cout << "Enter source city: ";
            getline(cin, source);
            source = getCity(source);
            cout << "Enter destination city: ";
            getline(cin, destination);
            destination = getCity(destination);
            cout << "Enter weight for travel time: ";
            cin >> timeWeight;
            cout << "Enter weight for cost: ";
            cin >> costWeight;
            cout << "Enter weight for environmental impact: ";
            cin >> impactWeight;
            network.calculateOptimizedFlow(source, destination, timeWeight, costWeight, impactWeight);
        } else if (choice == 6 ){
            print_map();
        }else {
            cout << "Invalid choice. Try again.\n";
        }
    }

    return 0;
}
