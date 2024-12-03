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
            while (cellNum != 0 && (pos = cell.find('\t')) != std::string::npos) {
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
            network.addRoute(route);
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
                for( Route route : network.getCityRoutes(message.substr(8, 10)) ){
                    msg += route.sourceCity + "," + route.destinationCity + "," + route.mode + ";";
                }
                w.send_msg(msg);
            }
        }
    }

}
