#include "CityNetwork.h"

//This adds a city to the network
void CityNetwork::addCity(std::string city){
    // Check if the city is already in network
    if( isInNetwork(city) ){ return; }

    //Add the city to the network
    cities.push_back(city);

    // Resize the capacity matrix
    for (auto& row : capacity) row.push_back(0);
    capacity.push_back(std::vector<int>(cities.size(), 0));
}

//This adds a route to the network
void CityNetwork::addRoute(Route route){
    //Add the cities
    addCity(route.sourceCity);
    addCity(route.destinationCity);
    
    //Add to capacity matrix
    capacity[getIndex(route.sourceCity)][getIndex(route.destinationCity)] += route.capacity;
    
    //Push back the route
    routes.push_back(route);
}

std::vector<Route> CityNetwork::getAllRoutes(){
    return routes;
}

std::vector<Route> CityNetwork::getCityRoutes(std::string city){
    std::vector<Route> cityRoutes;
    for (Route route : routes ){
        if( route.sourceCity.compare( city ) == 0 || route.destinationCity.compare( city ) == 0 ){
            cityRoutes.push_back(route);
        }
    }
    return cityRoutes;
}

int CityNetwork::calculateMaxFlow( std::string source, std::string destination){
    if(!isInNetwork(source) || !isInNetwork(destination)){ return -1; }

    // Calculate the source and sink indices
    int sourceIdx = getIndex(source);
    int sinkIdx = getIndex(destination);

    std::vector<int> parent(cities.size(), -1);
    std::vector<std::vector<int>> residual = capacity; // Copy the capacity matrix as the residual graph
    int maxFlow = 0;

    // Ford-Fulkerson algorithm
    while (findAugmentingPath(sourceIdx, sinkIdx, parent)) {
        int pathFlow = INT_MAX;

        // Find the minimum residual capacity in the augmenting path
        for (int v = sinkIdx; v != sourceIdx; v = parent[v]) {
            int u = parent[v];
            pathFlow = std::min(pathFlow, residual[u][v]);
        }

        // If path flow is zero, break the loop (safety check)
        if (pathFlow == 0) { break;}

        // Update residual capacities
        for (int v = sinkIdx; v != sourceIdx; v = parent[v]) {
            int u = parent[v];
            residual[u][v] -= pathFlow;
            residual[v][u] += pathFlow;
        }

        maxFlow += pathFlow;
    }
    return maxFlow;
}

/*
--------------PRIVATE METHODS---------------
 */

bool CityNetwork::isInNetwork( std::string city ){
    for( std::string element : cities){
        if( element.compare( city ) == 0 ){
            return true;
        }
    }
    return false;
}

int CityNetwork::getIndex(std::string city){
    int i = 0;
    for( std::string element : cities){
        if( element.compare( city ) == 0 ){
            return i;
        }
        i++;
    }
    return -1;
}

bool CityNetwork::findAugmentingPath(int source, int sink, std::vector<int>& parent) {
    std::vector<bool> visited(cities.size(), false);
    std::queue<int> q;
    q.push(source);
    visited[source] = true;
    parent[source] = -1;

    while (!q.empty()) {
        int curr = q.front();
        q.pop();

        for (int next = 0; next < cities.size(); next++) {
            if (!visited[next] && capacity[curr][next] > 0) {
                q.push(next);
                visited[next] = true;
                parent[next] = curr;

                if (next == sink) {
                    return true;
                }
            }
        }
    }

    return false;
}