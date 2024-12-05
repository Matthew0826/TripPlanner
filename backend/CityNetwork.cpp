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

//This just returns all the routes. This is for abstraction purposes.
std::vector<Route> CityNetwork::getAllRoutes(){
    return routes;
}

//Gets the routes coming from a city
std::vector<Route> CityNetwork::getCityRoutes(std::string city){
    std::vector<Route> cityRoutes;                  //Create blank vector of routes
    
    //Go through each route and add it if it comes from the city
    for (Route route : routes ){
        if( route.sourceCity.compare( city ) == 0 || route.destinationCity.compare( city ) == 0 ){
            cityRoutes.push_back(route);
        }
    }
    return cityRoutes;
}

//Get the maximum flow using the Ford-Fulkerson Algorithm
FlowResult CityNetwork::calculateMaxFlow( std::string source, std::string destination){
    if(!isInNetwork(source) || !isInNetwork(destination)){ return {}; }

    // Calculate the source and sink indices
    int sourceIdx = getIndex(source);
    int sinkIdx = getIndex(destination);

    //Create a parent vector filled with -1
    std::vector<int> parent(cities.size(), -1);
    std::vector<std::vector<double>> residual = capacity; // Copy the capacity matrix as the residual graph
    
    //Create empty maxFlow and FlowResult
    int maxFlow = 0;
    FlowResult result;

    while (true) {
        // BFS for finding an augmenting path
        std::fill(parent.begin(), parent.end(), -1);

        //Initialize a visited vector and a queue
        std::vector<bool> visited(cities.size(), false);
        std::queue<int> q;

        //Start with our source and make it visited
        q.push(sourceIdx);
        visited[sourceIdx] = true;

        //Go keep going until the queue is empty
        while (!q.empty()) {
            //Get the current city and remove it from the queue
            int curr = q.front();
            q.pop();

            //Go through each element in residual
            for (int next = 0; next < residual.size(); ++next) {
                //Go through each and check if is visited and its risidual is > 0
                if (!visited[next] && residual[curr][next] > 0) {
                    //Add to queue, set the parent
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
        //Go through and check each element to see if its the bottleneck capacity
        for (int v = sinkIdx; v != sourceIdx; v = parent[v]) {
            int u = parent[v];
            pathFlow = std::min(pathFlow, residual[u][v]);
        }

        //Add the flow found to the capacities
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

        //Add the path's flow to the maximum flow
        maxFlow += pathFlow;
    }
    //Return the FlowResult
    result.flow = maxFlow;
    return result;
}

//This determines the optimal path to travel from point
std::vector<Route> CityNetwork::calculateOptimizedPath(std::string source, std::string dest, int timeWeight, int costWeight, int impactWeight){
    //Create an empty weights and routes vector. Weights starts out really big
    std::vector<double> weights(cities.size(), 1000000);
    std::vector<Route> source_route(cities.size(), Route());

    //Set the weight of the source to 0
    weights.at(getIndex(source)) = 0;

    //Check if we have an invalid input. This will break things.
    if( timeWeight == 0 && costWeight == 0 && impactWeight == 0 ){
        timeWeight = 1; costWeight = 1; impactWeight = 1;
    }

    //Go through each node(call destination node j)
    for( int j = 0; j < weights.size(); j++ ){
        //Go through each node (call source node i)
        for( int i = 0; i < weights.size(); i++ ){

            //Skip the node if it has not been visited yet
            if( weights.at(i) <= 100000  ){

                // This means we are going to process a node
                for (Route curr_route : routes ){

                    // Check if the route is relevant
                    std::string otherNode;
                    if( curr_route.sourceCity.compare( cities.at(i) ) == 0){otherNode = curr_route.destinationCity; }
                    else if( curr_route.destinationCity.compare( cities.at(i) ) == 0){otherNode = curr_route.sourceCity; }
                    else{ continue; }

                    // Now generate the node's score
                    double node_score = timeWeight*(curr_route.time/TIME_SCALE) + 
                        costWeight*(curr_route.cost/COST_SCALE) + 
                        impactWeight*(curr_route.environmentalImpact/CARBON_SCALE) + weights.at(i);
                    
                    // Check if the other Node's score is less than the current score
                    if( weights.at(getIndex(otherNode)) > node_score ){

                        //If so, assign the other Node's score & weight
                        source_route.at(getIndex(otherNode)) = curr_route;
                        weights.at(getIndex(otherNode)) = node_score;
                    }
                }
            }
        }
    }

    //This is the route we will be returning
    std::vector<Route> final_route;
    //Start at the destination
    std::string curr_node = dest;

    //Go through each node, add the route, and go to the other node in the route
    do {
        Route curr_route = source_route.at(getIndex(curr_node));
        final_route.push_back(curr_route);
        if( curr_route.sourceCity.compare(curr_node) == 0 ){
            curr_node = curr_route.destinationCity;
        }else{
            curr_node = curr_route.sourceCity;
        }
    
    //Continue doing this until we hit the source
    }while( curr_node.compare(source) != 0);

    //Return the route
    return final_route;
}

//This gets whether we are in the network
bool CityNetwork::isInNetwork( std::string city ){
    //Go through each element and check if it's equivilant
    for( std::string element : cities){
        if( element.compare( city ) == 0 ){
            return true;
        }
    }
    return false;
}


/*
--------------PRIVATE METHODS---------------
 */
//Get the index of a node
int CityNetwork::getIndex(std::string city){
    int i = 0;
    //Go through and check if the index is right
    for( std::string element : cities){
        if( element.compare( city ) == 0 ){
            return i;
        }
        i++;
    }
    return -1;
}

//Find the augmenting path (BFS algorithm with Ford-Fulkerson)
bool CityNetwork::findAugmentingPath(int source, int sink, std::vector<int>& parent) {
    //Create visited vector and queue
    std::vector<bool> visited(cities.size(), false);
    std::queue<int> q;
    
    //Set the source
    q.push(source);
    visited[source] = true;
    parent[source] = -1;

    //Go until queue empty
    while (!q.empty()) {
        int curr = q.front();
        q.pop();
        
        //Go through each city
        for (int next = 0; next < cities.size(); next++) {
            //Check if city is visited and it has a nonzero capacity
            if (!visited[next] && capacity[curr][next] > 0) {
                //If so, push the next element
                q.push(next);
                
                //The city has been visited
                visited[next] = true;
                parent[next] = curr;

                //If we have it the sink, return
                if (next == sink) {
                    return true;
                }
            }
        }
    }
    
    //No augmenting path found
    return false;
}



    

    