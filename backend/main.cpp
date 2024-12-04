#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "CityNetwork.h"
#include "WindowsInterface.h"


CityNetwork getNetworkFromFile( std::string filename ){
    std::ifstream file("routes.csv");  // Open the CSV file
    std::string line;

    if( !file.is_open() ){ return CityNetwork(); }            // Check if File is Empty

    CityNetwork network = CityNetwork();

    int cellNum = 0;
    while(std::getline(file, line)){
        std::stringstream ss(line);
        std::string cell;
        while( std::getline(ss, cell, ',')){
            size_t pos;                 //Store the position
            int elementNumber = 0;      //Store the current Element
            Route route = Route();
            while (cellNum >= 1 && (pos = cell.find('\t')) != std::string::npos) {
                if( elementNumber == 0 ){ route.sourceCity = cell.substr(0, pos); }
                else if(elementNumber == 1 ){ route.destinationCity = cell.substr(0, pos); }
                else if(elementNumber == 2 ){ route.mode = cell.substr(0, pos); }
                else if(elementNumber == 3 ){ route.capacity = stoi(cell.substr(0, pos)); }
                else if(elementNumber == 4 ){ route.cost = stod(cell.substr(0, pos)); }
                else if(elementNumber == 5 ){ route.time = stod(cell.substr(0, pos)); }
                else if(elementNumber == 6 ){ route.environmentalImpact = stod(cell.substr(0, pos)); }
                cell = cell.substr(pos + 1);  // Get the part after the tab
                elementNumber++;
            }
            if( cellNum >= 1 ){
                network.addRoute(route);
            }
            cellNum++;

        }
        
    }
    file.close();
    return network;
}

int main() {
    CityNetwork network = getNetworkFromFile("routes.csv");
    std::cout << "Welcome to the Trip Planner Backend" << std::endl;
    std::cout << "Enter 0 to connect to independent frontend and 1 to run headless: ";

    int input;
    std::cin >> input;
    if( input == 0 ){
        WindowsInterface w = WindowsInterface(8001);
        std::cout << "Looking for frontend on port 8001..." << std::endl;
        w.wait_for_client();
        std::cout << "Frontend Found!" << std::endl;
        while( true ){
            std::string message = w.rcv_msg();
            if( message.compare("Display All") == 0){
                std::string msg;
                for( Route route : network.getAllRoutes() ){
                    msg += route.sourceCity + "," + route.destinationCity + "," + route.mode + ";";
                }
                w.send_msg(msg);
            }else if( message.substr(0, 7).compare("Display") == 0){
                std::string msg;
                for( Route route : network.getCityRoutes(message.substr(8, 3)) ){
                    msg += route.sourceCity + "," + route.destinationCity + "," + route.mode + ";";
                }
                w.send_msg(msg);
            }else if( message.substr(0, 4).compare("Flow") == 0 ){
                std::string msg = "";
                FlowResult flow = network.calculateMaxFlow(message.substr(5, 3), message.substr(9, 3));
                msg = "";
                for (int i = 0; i < flow.connections.size(); i++ ){
                    msg += std::to_string(flow.capacities[i]);
                    msg += ":";
                    for( int j = 0; j < flow.connections[i].size(); j++ ){
                        msg += flow.connections[i].at(j) + ",";
                    }
                    msg += ";";
                }
                w.send_msg(msg);
            }else if( message.substr(0, 5).compare("Route") == 0 ){
                std::string msg = "";
                for( Route route : network.calculateOptimizedPath(message.substr(6, 3), message.substr(10, 3),
                    int(message.at(16)) - int('1'), int(message.at(14)) - int('1'), int(message.at(18)) - int('1') )){
                    msg += route.sourceCity + "," + route.destinationCity + "," + route.mode + ";";
                }
                w.send_msg(msg);
            }
        }
    }else{
        bool running = true;
        std::cout << std::endl << "Welcome to the Travel Headless Mode!";
        while( running ){
            std::cout << std::endl << "---Option List---" << std::endl;
            std::cout << "1. View Routes" << std::endl << "2. Display City Routes" << std::endl;
            std::cout << "3. Calculate Maxmimum Flow" << std::endl << "4. Find Optimal Route" << std::endl;
            std::cout << "5. Exit" << std::endl << "Enter option: ";
            int selection;
            std::cin >> selection;
            if( selection == 1 ){
                for(Route r : network.getAllRoutes() ){
                    std::cout << r.sourceCity << " -> " << r.destinationCity << " (" << r.mode <<
                    "): cost- " << r.cost << ", time- " << r.time << ", tons of carbon- " << r.environmentalImpact << std::endl;
                }
            }else if( selection == 2 ){
                std::string city;
                std::cout << std::endl << "Enter the source city: ";
                std::cin >> city;
                for(Route r : network.getCityRoutes(city) ){
                    std::cout << r.sourceCity << " -> " << r.destinationCity << " (" << r.mode <<
                    "): cost- " << r.cost << ", time- " << r.time << ", tons of carbon- " << r.environmentalImpact << std::endl;
                }
            }else if( selection == 3 ){
                std::string src, dst;
                std::cout << std::endl << "Enter the source city: ";
                std::cin >> src;
                std::cout << "Enter the destination city: ";
                std::cin >> dst;
                FlowResult f = network.calculateMaxFlow(src, dst);
                std::cout << "Maximum flow: " << f.flow << std::endl;
                std::cout << "Paths used: " << std::endl;
                for( int i = 0; i < f.capacities.size(); i++ ){
                    std::cout << f.capacities.at(i) << " sent via " << f.connections.at(i).at(0);
                    for( int j = 1; j < f.connections.at(i).size(); j++ ){
                        std::cout <<  " -> " <<f.connections.at(i).at(j);
                    }
                    std::cout << std::endl;
                }
            }else if( selection == 4 ){
                std::string src, dst;
                int cost, time, carbon;
                std::cout << std::endl << "Enter the source city: ";
                std::cin >> src;
                std::cout << "Enter the destination city: ";
                std::cin >> dst;
                std::cout << "Enter the time weight: ";
                std::cin >> time;
                std::cout << "Enter the cost weight: ";
                std::cin >> cost;
                std::cout << "Enter the environmental weight: ";
                std::cin >> carbon;
                std::cout << std::endl << "Result: " << std::endl;
                for(Route r : network.calculateOptimizedPath(src, dst, time, cost, carbon) ){
                    std::cout << r.sourceCity << " -> " << r.destinationCity << " (" << r.mode <<
                    "): cost- " << r.cost << ", time- " << r.time << ", tons of carbon- " << r.environmentalImpact << std::endl;
                }
            }else if( selection == 5 ){
                std::cout << "Goodbye";
                running = false;
            }else{
                std::cout << "That is not a valid option" << std::endl;
            }
        }
    }

}
