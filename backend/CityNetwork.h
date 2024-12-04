#ifndef CITY_NETWORK
#define CITY_NETWORK

#include <string>
#include <vector>
#include <map>
#include <queue>

struct FlowResult{
    int flow;
    std::vector<std::vector<std::string>> connections;
    std::vector<int> capacities;
};

class Route{
    public:
        std::string sourceCity;
        std::string destinationCity;
        int capacity;
        double time;
        std::string mode;
        double cost;
        double environmentalImpact;
};

class CityNetwork{
    public: 
        // These add attributes to the network
        void addCity( std::string city );
        void addRoute( Route route );

        // Get Routes
        std::vector<Route> getAllRoutes();
        std::vector<Route> getCityRoutes( std::string city );

        // Calculate the max flow between two cities
        FlowResult calculateMaxFlow( std::string source, std::string destination );

    private:
        std::vector<Route> routes;          // List that stores the routes
        std::vector<std::string> cities;

        std::vector<std::vector<double>> capacity; //Adjacency matrix for capacities
        std::vector<std::vector<double>> residual; // Residual graph

        // Check if a city is valid
        bool isInNetwork( std::string city );
        bool findAugmentingPath(int source, int sink, std::vector<int>& parent);
        int getIndex(std::string city);
};

#endif