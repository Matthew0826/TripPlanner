#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "CityNetwork.h"
#include "WindowsInterface.h"

/*Read from the CSV File*/
CityNetwork getNetworkFromFile( std::string filename ){
    std::ifstream file("routes.csv");  // Open the CSV file
    std::string line;                   //Create an empty line

    if( !file.is_open() ){ return CityNetwork(); }            // Check if File is Empty

    CityNetwork network = CityNetwork();                    //Generate blank city network

    //Initialize cellNum to 0 (to skip first cell)
    int cellNum = 0;

    //Go through each line in the file
    while(std::getline(file, line)){
        std::stringstream ss(line);
        std::string cell;

        //Go through each element seperated by a comma
        while( std::getline(ss, cell, ',')){
            size_t pos;                 //Store the position
            int elementNumber = 0;      //Store the current Element
            Route route = Route();      //Create a blank route

            //Go through and add the element to the proper part of the route
            while (cellNum >= 1 && (pos = cell.find('\t')) != std::string::npos) {
                if( elementNumber == 0 ){ route.sourceCity = cell.substr(0, pos); }
                else if(elementNumber == 1 ){ route.destinationCity = cell.substr(0, pos); }
                else if(elementNumber == 2 ){ route.mode = cell.substr(0, pos); }
                else if(elementNumber == 3 ){ route.capacity = stoi(cell.substr(0, pos)); }
                else if(elementNumber == 4 ){ route.cost = stod(cell.substr(0, pos)); }
                else if(elementNumber == 5 ){ route.time = stod(cell.substr(0, pos)); }
                cell = cell.substr(pos + 1);  // Get the part after the tab
                elementNumber++;
            }

            //If it is not the header, add to the route
            if( cellNum >= 1 ){
                //Add the environmental impact
                route.environmentalImpact = stod(cell.substr(0, pos));
                network.addRoute(route);
            }
            //Increment the current cell number
            cellNum++;
        }
    }

    //Close the file and return the generated network
    file.close();
    return network;
}

/* This is the main method. It contains all the goods as well as the Headless UI*/
int main() {
    //Read the file and display the initial text
    CityNetwork network = getNetworkFromFile("routes.csv");
    std::cout << "Welcome to the Trip Planner Backend" << std::endl;
    std::cout << "Enter 0 to connect to independent frontend and 1 to run headless: ";

    //This chooses between headed and headless
    int input;
    std::cin >> input;

    /*
    --------- HEADLESS MODE ------------
    */
    if( input == 0 ){

        //Create a windows interface
        WindowsInterface w = WindowsInterface(8001);

        //Display that we are looking for a frontend & wait for the frontend
        std::cout << "Looking for frontend on port 8001..." << std::endl;
        w.wait_for_client();

        //We found the frontend :)
        std::cout << "Frontend Found!" << std::endl;

        //Reacurring loop
        while( true ){
            //Get the message
            std::string message = w.rcv_msg();

            //Display all message
            if( message.compare("Display All") == 0){
                std::string msg;
                //Return all routes
                for( Route route : network.getAllRoutes() ){
                    msg += route.sourceCity + "," + route.destinationCity + "," + route.mode + ";";
                }
                w.send_msg(msg);
            
            //Display message
            }else if( message.substr(0, 7).compare("Display") == 0){
                std::string msg;
                //Get routes from city and send it to the frontend
                for( Route route : network.getCityRoutes(message.substr(8, 3)) ){
                    msg += route.sourceCity + "," + route.destinationCity + "," + route.mode + ";";
                }
                w.send_msg(msg);

            //Flow message
            }else if( message.substr(0, 4).compare("Flow") == 0 ){
                std::string msg = "";

                //Calculate the flow based on the parameters
                FlowResult flow = network.calculateMaxFlow(message.substr(5, 3), message.substr(9, 3));
                msg = "";

                //Go through each connection in the result
                for (int i = 0; i < flow.connections.size(); i++ ){
                    //Add the capacity
                    msg += std::to_string(flow.capacities[i]);
                    msg += ":";

                    //Add the cities
                    for( int j = 0; j < flow.connections[i].size(); j++ ){
                        msg += flow.connections[i].at(j) + ",";
                    }
                    msg += ";";
                }
                //Send the message
                w.send_msg(msg);

            //Route message
            }else if( message.substr(0, 5).compare("Route") == 0 ){
                std::string msg = "";
                //Generate optmized path and send it. Subtract by int('1') to convert char to int - 1
                //This makes it so 1 is actually 0 and has no effect
                for( Route route : network.calculateOptimizedPath(message.substr(6, 3), message.substr(10, 3),
                    int(message.at(16)) - int('1'), int(message.at(14)) - int('1'), int(message.at(18)) - int('1') )){
                    msg += route.sourceCity + "," + route.destinationCity + "," + route.mode + ";";
                }
                w.send_msg(msg);

            //Exit message
            }else if( message.compare("Bye!")){
                //Terminate the program
                std::cout << "Backend closed by frontend. Goodbye." << std::endl;
                return 0;
            }
        }

    /*
    ------------- HEADLESS UI ---------------------
     */
    }else{
        //Start running the loop & display welcome message
        bool running = true;
        std::cout << std::endl << "Welcome to the Travel Headless Mode!";


        while( running ){
            //Display option list
            std::cout << std::endl << "---Option List---" << std::endl;
            std::cout << "1. View Routes" << std::endl << "2. Display City Routes" << std::endl;
            std::cout << "3. Calculate Maxmimum Flow" << std::endl << "4. Find Optimal Route" << std::endl;
            std::cout << "5. Exit" << std::endl << "Enter option: ";

            //Get selection
            int selection;
            try{
                std::cin >> selection;
            }catch(...){std::cout << "Invalid Selection. Please try again." << std::endl;}

            //OPTION 1
            if( selection == 1 ){
                //Display all routes
                for(Route r : network.getAllRoutes() ){
                    std::cout << r.sourceCity << " -> " << r.destinationCity << " (" << r.mode <<
                    "): cost- " << r.cost << ", time- " << r.time << ", tons of carbon- " << r.environmentalImpact << std::endl;
                }

            //OPTION 2: Display routes from source
            }else if( selection == 2 ){
                std::string city;
                //Ask for city
                std::cout << std::endl << "Enter the source city: ";
                std::cin >> city;

                //Check if we actually have the city
                if( !network.isInNetwork(city)){
                    std::cout << "ERROR: " << city << " is not a valid city code. Please remember to use all caps." << std::endl;
                    continue;
                }

                //Display the routes
                for(Route r : network.getCityRoutes(city) ){
                    std::cout << r.sourceCity << " -> " << r.destinationCity << " (" << r.mode <<
                    "): cost- " << r.cost << ", time- " << r.time << ", tons of carbon- " << r.environmentalImpact << std::endl;
                }

            }else if( selection == 3 ){
                //Ask for the cities
                std::string src, dst;
                std::cout << std::endl << "Enter the source city: ";
                std::cin >> src;
                std::cout << "Enter the destination city: ";
                std::cin >> dst;

                //Check if they are valid
                if( !network.isInNetwork(src)){
                    std::cout << "ERROR: " << src << " is not a valid city code. Please remember to use all caps." << std::endl;
                    continue;
                }
                if( !network.isInNetwork(dst)){
                    std::cout << "ERROR: " << dst << " is not a valid city code. Please remember to use all caps." << std::endl;
                    continue;
                }
                
                //Get the FlowResult
                FlowResult f = network.calculateMaxFlow(src, dst);

                //Display the flow
                std::cout << "Maximum flow: " << f.flow << std::endl;

                //Display the paths used
                std::cout << "Paths used: " << std::endl;

                //Go through each path and display each of its cities
                for( int i = 0; i < f.capacities.size(); i++ ){
                    std::cout << f.capacities.at(i) << " sent via " << f.connections.at(i).at(0);
                    for( int j = 1; j < f.connections.at(i).size(); j++ ){
                        std::cout <<  " -> " <<f.connections.at(i).at(j);
                    }
                    std::cout << std::endl;
                }

            //Get Optimized Route
            }else if( selection == 4 ){
                std::string src, dst;
                int cost, time, carbon;
                std::cout << std::endl << "Enter the source city: ";
                std::cin >> src;
                std::cout << "Enter the destination city: ";
                std::cin >> dst;
                try{
                    std::cout << "Enter the time weight (1-5): ";
                    std::cin >> time;
                    std::cout << "Enter the cost weight (1-5): ";
                    std::cin >> cost;
                    std::cout << "Enter the environmental weight (1-5): ";
                    std::cin >> carbon;
                }catch( ... ){std::cout << "That is not an integer. Please try again." << std::endl; continue;}
                

                //Check if they are valid
                if( !network.isInNetwork(src)){
                    std::cout << "ERROR: " << src << " is not a valid city code. Please remember to use all caps." << std::endl;
                    continue;
                }
                if( !network.isInNetwork(dst)){
                    std::cout << "ERROR: " << dst << " is not a valid city code. Please remember to use all caps." << std::endl;
                    continue;
                }
                if( time > 5 || time <= 0 || cost > 5 || cost <= 0 || carbon > 5 || carbon <= 0 ){
                    std::cout << "ERROR: One or more of your inputs is not between 1-5. Please try again." << std::endl;
                    continue;
                }
                
                //Display the result
                std::cout << std::endl << "Result: " << std::endl; 

                //Display all the routes
                for(Route r : network.calculateOptimizedPath(src, dst, time, cost, carbon) ){
                    std::cout << r.sourceCity << " -> " << r.destinationCity << " (" << r.mode <<
                    "): cost- " << r.cost << ", time- " << r.time << ", tons of carbon- " << r.environmentalImpact << std::endl;
                }

            //This exits
            }else if( selection == 5 ){
                std::cout << "Goodbye";
                running = false;

            //Any other option
            }else{
                std::cout << "That is not a valid option" << std::endl;
            }
        }
    }

}
