#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <set>
#include <algorithm>

using namespace std;

//This is a class that specifies a connection between two cities 
class Connection {
public:
    string city1;
    string city2;

    string mode;                //The mode of transport

    int capacity;               // The number of people the connection can hold
    int periodicity;            // Number of times the connection is run per day

    double cost;                // Cost of the connection in USD
    double time;                //Time of the connection in minutes
    double environmentalImpact; // Environmental impact in metric tons CO₂ per person

    Connection(string city1, string city2, string mode, int capacity, int periodicity, double cost, double time, double carbon ){
        this->city1 = city1;
        this->city2 = city2;
        this->mode = mode;
        this->capacity = capacity;
        this->periodicity = periodicity;
        this->cost = cost;
        this->time = time;
        this->environmentalImpact = carbon;
    }
};

class Route {
    public:
        vector<Connection> connections;
};

class TravelPlan {
    public:
        vector<Route> routes;
};


class CityNetwork {
public:
    vector<Connection> connections;
    set<string> cities = {"BOS", "NYC", "PHI", "WDC", "CHI", "HOU", "LAX"};

    //This adds a connection to the vector
    void addConnection( Connection c ){ connections.push_back(c); }

    //This prints all routes that are connected to the city
    void printCityConnections( string city ){
        for( int i = 0; i < connections.size(); i++ ){
            if( connections.at(i).city1.compare(city) == 0 || connections.at(i).city2.compare(city) == 0 ){
                cout << "Connection: " << connections.at(i).city1 << " to " << connections.at(i).city2 << " by " << connections.at(i).mode << endl;
            }
        }
    }

    vector<Connection> getCityConnections( string city ){
        vector<Connection> conns;
        for( int i = 0; i < connections.size(); i++ ){
            if( connections.at(i).city1.compare(city) == 0 || connections.at(i).city2.compare(city) == 0 ){
                conns.push_back( connections.at(i) );
            }
        }
        return conns;
    }

    //This calculates the most optimal route using BFS (Breadth First Search)
    Route getBestRoute( string source, string destination, TravelPlan currPlan = TravelPlan() ){
        //Start with the top node. Choose the smallest 
    }
};


CityNetwork read_from_table( string filename ){
    CityNetwork rtrnNetwork;
    ifstream file(filename);
    if( !file.is_open()){ cout << "File " << filename << " not found." << endl;}
    string line;
    bool firstLineSkipped = false;
    while( getline(file, line) ){
        stringstream currstream(line);
        if(!firstLineSkipped){ firstLineSkipped = true; continue;}
        string curr;
        int row = 0;
        string elements[8];
        while( getline(currstream, curr, '\t')){
            elements[row] = curr; 
            row++; 
        }
        rtrnNetwork.addConnection( Connection(elements[0], elements[1], elements[2], stoi(elements[3]), stoi(elements[7]), stod(elements[4]), stod(elements[5]), stod(elements[6])));
    }
    file.close();
    return rtrnNetwork;
}


int main(){
    CityNetwork network = read_from_table("routes.csv");
    network.printCityConnections("NYC");
}
