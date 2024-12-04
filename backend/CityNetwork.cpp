#include "CityNetwork.h"
#include <iostream>



//This adds a city to the network
void CityNetwork::addCity(std::string city){
    // Check if the city is already in network
    if( isInNetwork(city)){ return; }

    //Add the city to the network
    cities.push_back(city);

    // Resize the capacity matrix
    for (auto& row : capacity) row.push_back(0);
    capacity.push_back(std::vector<double>(cities.size(), 0));
}

//This adds a route to the network
void CityNetwork::addRoute(Route route){
    //Add the cities
    addCity(route.sourceCity);
    addCity(route.destinationCity);
    
    //Add to capacity matrix
    capacity[getIndex(route.sourceCity)][getIndex(route.destinationCity)] += route.capacity;
    capacity[getIndex(route.destinationCity)][getIndex(route.sourceCity)] += route.capacity;
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

FlowResult CityNetwork::calculateMaxFlow( std::string source, std::string destination){
    if(!isInNetwork(source) || !isInNetwork(destination)){ return {}; }

    // Calculate the source and sink indices
    int sourceIdx = getIndex(source);
    int sinkIdx = getIndex(destination);

    std::vector<int> parent(cities.size(), -1);
    std::vector<std::vector<double>> residual = capacity; // Copy the capacity matrix as the residual graph
    int maxFlow = 0;
    FlowResult result;

    while (true) {
        // BFS for finding an augmenting path
        std::fill(parent.begin(), parent.end(), -1);
        std::vector<bool> visited(cities.size(), false);
        std::queue<int> q;
        q.push(sourceIdx);
        visited[sourceIdx] = true;

        while (!q.empty()) {
            int curr = q.front();
            q.pop();

            for (int next = 0; next < residual.size(); ++next) {
                if (!visited[next] && residual[curr][next] > 0) {
                    q.push(next);
                    parent[next] = curr;
                    visited[next] = true;
                    if (next == sinkIdx) break;
                }
            }
        }

        // If no augmenting path, break
        if (parent[sinkIdx] == -1) break;

        // Find bottleneck capacity
        double pathFlow = INT_MAX;
        for (int v = sinkIdx; v != sourceIdx; v = parent[v]) {
            int u = parent[v];
            pathFlow = std::min(pathFlow, residual[u][v]);
        }

        // Print the augmenting path with flow
        result.capacities.push_back(pathFlow);
        std::vector<std::string> output;
        for (int v = sinkIdx; v != -1; v = parent[v]) {
            output.push_back(cities.at(v));
        }
        result.connections.push_back(output);

        // Update residual graph
        for (int v = sinkIdx; v != sourceIdx; v = parent[v]) {
            int u = parent[v];
            residual[u][v] -= pathFlow;
            residual[v][u] += pathFlow;
        }

        maxFlow += pathFlow;
    }
    result.flow = maxFlow;
    return result;
}

//This determines the optimal path to travel from point
std::vector<Route> CityNetwork::calculateOptimizedPath(std::string source, std::string dest, int timeWeight, int costWeight, int impactWeight){
    std::vector<double> weights(cities.size(), 1000000);
    std::vector<Route> source_route(cities.size(), Route());
    weights.at(getIndex(source)) = 0;
    if( timeWeight == 0 && costWeight == 0 && impactWeight == 0 ){
        timeWeight = 1; costWeight = 1; impactWeight = 1;
    }

    for( int j = 0; j < weights.size(); j++ ){
        for( int i = 0; i < weights.size(); i++ ){
            if( weights.at(i) <= 100000  ){
                // This means we are going to process a node
                for (Route curr_route : routes ){

                    // Check if the node is relevant
                    std::string otherNode;
                    if( curr_route.sourceCity.compare( cities.at(i) ) == 0){otherNode = curr_route.destinationCity; }
                    else if( curr_route.destinationCity.compare( cities.at(i) ) == 0){otherNode = curr_route.sourceCity; }
                    else{ continue; }

                    // Now generate the node's score
                    double node_score = timeWeight*(curr_route.time/TIME_SCALE) + 
                        costWeight*(curr_route.cost/COST_SCALE) + 
                        impactWeight*(curr_route.environmentalImpact/CARBON_SCALE) + weights.at(i);
                    
                    // Check if the other Node's score is less than the current
                    if( weights.at(getIndex(otherNode)) > node_score ){
                        source_route.at(getIndex(otherNode)) = curr_route;
                        weights.at(getIndex(otherNode)) = node_score;
                    }
                }
            }
        }
    }

    std::vector<Route> final_route;
    std::string curr_node = dest;
    do {
        Route curr_route = source_route.at(getIndex(curr_node));
        final_route.push_back(curr_route);
        if( curr_route.sourceCity.compare(curr_node) == 0 ){
            curr_node = curr_route.destinationCity;
        }else{
            curr_node = curr_route.sourceCity;
        }
    }while( curr_node.compare(source) != 0);

    return final_route;
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



    

    