#include <iostream>
#include <string>
#include <vector>

using namespace std;

// route struct
struct Route {
    string sourceCity;
    string destinationCity;
    int capacity;    
    int travelTime;  
    string mode;     
};

// class to manage all routes between cities
class CityNetwork {
public:
    vector<Route> routes;  // list of all routes

    // add route between two cities 
    void addRoute(string sourceCity, string destinationCity, int capacity, int time, string mode) {
        routes.push_back({sourceCity, destinationCity, capacity, time, mode});
    }

    // print routes
    void printRoutes() {
        for (int i = 0; i < routes.size(); i++) {
            Route &route = routes[i];  
            cout << "Route from " << route.sourceCity << " to " << route.destinationCity << " | ";
            cout << "Capacity: " << route.capacity;
            cout << ", Travel Time: " << route.travelTime;
            cout << ", Mode: " << route.mode << endl;
        }
    }
};

int main() {
    CityNetwork network;

    // addint routes
   network.addRoute("NYC", "Chicago", 200, 2, "airplane");
network.addRoute("NYC", "Boston", 100, 4, "train");
network.addRoute("NYC", "Washington DC", 180, 1.5, "airplane");
network.addRoute("NYC", "Philadelphia", 50, 22, "bus");
network.addRoute("Chicago", "NYC", 200, 2, "airplane");
network.addRoute("Chicago", "Boston", 90, 15, "train");
network.addRoute("Chicago", "Washington DC", 180, 1.5, "airplane");
network.addRoute("Chicago", "Philadelphia", 80, 12, "train");
network.addRoute("Boston", "NYC", 40, 4, "bus");
network.addRoute("Boston", "Chicago", 70, 10, "train");
network.addRoute("Boston", "Washington DC", 160, 1.5, "airplane");
network.addRoute("Boston", "Philadelphia", 30, 6, "bus");
network.addRoute("Washington DC", "NYC", 120, 3, "train");
network.addRoute("Washington DC", "Chicago", 150, 3, "airplane");
network.addRoute("Washington DC", "Boston", 90, 8, "train");
network.addRoute("Washington DC", "Philadelphia", 130, 1, "airplane");
network.addRoute("Philadelphia", "NYC", 80, 1, "train");
network.addRoute("Philadelphia", "Chicago", 150, 2.5, "airplane");
network.addRoute("Philadelphia", "Boston", 35, 6, "bus");
network.addRoute("Philadelphia", "Washington DC", 80, 2, "train");



    // print routes
    network.printRoutes();

    return 0;
}
