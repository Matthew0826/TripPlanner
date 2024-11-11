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

void print_map(){
    cout << "    |     |             |                              |        |        /       ___/" << endl;
    cout << "    |     \\              |              ______________/         |_______/_______/ _/" << endl;
    cout << "____|      \\             |             /                        |               _/" << endl;
    cout << "    \\     _/            /           __/                         |         BOS o|_" << endl;
    cout << "CHI o\\___/____________/       _____/  |__________________       |_____________   \\   \\" << endl;
    cout << "     |           |     \\_____/     |                     \\_     |          |  \\   \\___\\" << endl;
    cout << "     |           |                 |                       /--__|  ________|--/\\___/---\\" << endl;
    cout << "     |           |                 |                      /NYC o/_/ ________ " << endl;
    cout << "     |           |                /|                      \\   /_ /______/" << endl;
    cout << "     |           |               | |                  PHI o\\    /" << endl;
    cout << "     |           |              _/ |____________________/-/    /" << endl;
    cout << "     |       __/ \\____      __/        |__/ \\__      _/|\\\\__ /" << endl;
    cout << "     /   ___/         \\____/              ____/ \\DC o| || | //" << endl;
    cout << "    /___/                |              _/        \\_ | ||__|" << endl;
    cout << "    |                    \\             /            \\|  \\__/" << endl;
}

int main() {
    CityNetwork network;
    cout << "Welcome to the Trip Planner!" << endl;
    print_map();
    cout << endl << "Routes: ";
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
    network.printRoutes();
    return 0;
}
