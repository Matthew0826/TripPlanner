import pygame
import time
import sys

from components import *
from interface import *

## INTERFACE INITIALIZATION
try:
    #Create the interface
    w = Interface(8001)
except:
    print( "No backend detected. Please start the backend before running the frontend." )
    sys.exit()

### PYGAME INITIALIZATION

# Initialize Pygame
pygame.init()

# Set up the display
screen = pygame.display.set_mode((1475, 700))  # Set screen size (width, height)
pygame.display.set_caption("Trip Planner")

# Load the image
image = pygame.image.load('map.jpg')  # Replace with your image path
# Set up the font
font = pygame.font.SysFont('Arial', 35)

# Generic Text
text = "Trip Planner"

# Generic Text Surface & Position
text_surface = font.render(text, True, (0, 0, 0))  # White color
text_rect = text_surface.get_rect(center=(1375, 20))  


### GLOBAL DATA INITIALIZATION

#The currently displayed routes
route_list = []

# This is the list of cities
cities = [["NYC", (40.713, -74.006)], ["CHI", (41.878, -87.6298)], ["PHI", (39.953, -75.1652)], ["WDC", (38.9072, -77.0369)],
          ["BOS", (42.361, -71.057)], ["LAX", (34.059, -118.243)], ["HOU", (29.758, -95.361)]]

cities_clicked = []
### GLOBAL FUNCTION INITIALIZATION

#This translate coordinates into position. 
def getPosFromCoords( coords ):
    return (coords[1] * 21.433 + 2686.177, coords[0] * -28.18455 + 1420.191)

# Get the position of the city by finding its index and returning the position
def getCityPos( cityName ):
    for city in cities:
        if( city[0] == cityName ):
            return getPosFromCoords(city[1])
    return (0, 0)

#This process a route message from the backend
def process_route_message(message):
    #Define datatypes for modes of transport
    busses = []
    airplanes = []
    trains = []

    #Go through each route in the message
    for route in message:
        #Check if it is invalid
        if( route == "b',," or route == "'" ):
            continue
        route = route.split(",")
        if( route[2] == "Bus"):
            busses.append((route[0], route[1], route[2]))
        elif( route[2] == "Train"):
            trains.append((route[0], route[1], route[2]))
        elif( route[2] == "Airplane"):
            airplanes.append((route[0], route[1], route[2]))

    #Add the routes to the list
    route_list.extend(busses)
    route_list.extend(trains)
    route_list.extend(airplanes)

# This executes when a Display Route button is pressed
def get_routes(source):
    #Clear the displayed routes
    route_list.clear()
    if source == "all":
        w.send("Display All")
    else:
        #Check if we have enough cities        
        if len(cities_clicked) == 0:
            return
        
        w.send("Display " + cities[cities_clicked[len(cities_clicked) - 1]][0])
    
    #Wait for message to arrive & process message
    time.sleep(0.01)
    message = str(w.recv())[2:].split(";")
    process_route_message(message)

#This executes when a Max Flow button is pressed
def max_flow():
    #Reset the route list 
    route_list.clear()
    
    #Check if we have enough cities
    if( len(cities_clicked) <= 1):
        return
    
    #Send a flow request
    w.send("Flow " + cities[cities_clicked[len(cities_clicked)-1]][0] + " " + cities[cities_clicked[len(cities_clicked)-2]][0])
    
    #Wait for message to arrive
    time.sleep(0.01)

    #Parse message
    message = str(w.recv())[2:-1].split(";")[:-1]
    for route in message:

        #Split message and get capacity
        route = route.split(":")
        capacity = int(route[0])
        curr_cities = route[1].split(",")[:-1]

        #Go through each city in curr_cities
        for i in range(len(curr_cities)-1):

            #Check if route is already added
            addCity = True
            if len(route_list) != 0:

                #Go through each "subroute"
                for cty in route_list:

                    #If the route already exists, add to the capacity of it
                    if (curr_cities[i] == cty[0] and curr_cities[i+1] == cty[1] ) or(curr_cities[i] == cty[1] and curr_cities[i+1] == cty[0] ):
                        addCity = False 
                        cty[2] += capacity

            #If the route doesn't exist, add it
            if addCity:
                route_list.append([curr_cities[i], curr_cities[i+1], capacity])

#This executes when the Get Optimized Button is pressed
def get_optimized():
    #Clear the list
    route_list.clear()

    #Check if we have enough cities
    if( len(cities_clicked) <= 1):
        return
    
    #Send the message
    w.send("Route " + cities[cities_clicked[len(cities_clicked)-1]][0] + " " + cities[cities_clicked[len(cities_clicked)-2]][0] \
           + " " + str(int(components[0].value)) + " " + str(int(components[1].value)) + " " + str(int(components[2].value)))

    #Wait for the message
    time.sleep(0.01)
    message = str(w.recv())[2:].split(";")
    
    process_route_message(message)

### GLOBAL GRAPHICAL ELEMENT INITIALIZATION
components = [Slider(screen, 1375, 380, 175, 50 ), Slider(screen, 1375, 480, 175, 50 ),Slider(screen, 1375, 580, 175, 50 ), 
              Button(screen, 1375, 650, 175, 50, text="Get Best Route", on_press=get_optimized), Button(screen, 1375, 85, 175, 50, text="Display All Routes", on_press=lambda: get_routes("all")),
              Button(screen, 1375, 225, 175, 50, text="Display Source Routes", on_press=lambda: get_routes("")), Button(screen, 1375, 290, 175, 50, text="Get Max Flow", on_press=max_flow)]


# Game loop
running = True
while running:

    # Handle events
    for event in pygame.event.get():

        # This exits the loop when the user exits
        if event.type == pygame.QUIT:
            running = False

        # Check if the mouse button is pressed
        if event.type == pygame.MOUSEBUTTONDOWN:

            # Get the position of the mouse click
            mouse_pos = pygame.mouse.get_pos()

            #Update all the components
            for component in components:
                component.on_click(mouse_pos)

            #Go through each city and check if it was pressed
            i = 0
            for city in cities:  
                #Get the current city's position
                coords = getPosFromCoords(city[1])

                #Check if we are within a 10 pixel radius. If so, add the city to cities_clicked
                distance = ((mouse_pos[0] - coords[0]) ** 2 + (mouse_pos[1] - coords[1]) ** 2) ** 0.5
                if( distance < 10 ):
                    cities_clicked.append(i)
                i += 1

        # Go through each component and update it on mouse up
        if event.type == pygame.MOUSEBUTTONUP:
            for component in components:
                component.on_release()

    #Fill screen with white background
    screen.fill((200, 200, 200)) 

    #Set 10 ms delay
    pygame.time.delay(10)

    
    # Draw the image on the screen
    screen.blit(image, (0, 0))  

    #Draw the top and side line
    pygame.draw.line(screen, (0, 0, 0), (1275, 0), (1275, 700), 3)
    pygame.draw.line(screen, (0, 0, 0), (1275, 50), (1475, 50), 3)

    #Draw the title text
    screen.blit(text_surface, text_rect)

    #Go through each route in route list
    for route in route_list:
        #Determine proper weight and color    
        color = (200, 200, 200)
        width = 2
        if( route[2] == "Bus"):
            color = (0, 0, 255)
            width = 12
        elif( route[2] == "Train"):
            color = (0, 150, 0)
            width = 6
        elif( type(route[2]) == int ):
            width = int(route[2]/10)
            color = (200, 200, 200)
        
        #Draw the line
        pygame.draw.line(screen, color, getCityPos(route[0]), getCityPos(route[1]), width)

    #Go through each city
    for city in cities: 
        #Get the coords and draw the dot
        coords = getPosFromCoords(city[1])
        pygame.draw.circle(screen, (255, 255, 255), coords, 8)

        #Draw the city name off to the side
        font = pygame.font.SysFont('Arial', 20)
        screen.blit(font.render(city[0], True, (255, 255, 255)),text_surface.get_rect(center=(coords[0] + 30, coords[1] + 5)) )
    
    #Set the font
    font = pygame.font.SysFont('Arial', 20)

    #Display the two most recently selected cities
    if len(cities_clicked) >= 2:
        city2 = cities[cities_clicked[len(cities_clicked)-1]][0]
        city1 = cities[cities_clicked[len(cities_clicked)-2]][0]
    elif len(cities_clicked) == 1:
        city1 = cities[cities_clicked[len(cities_clicked)-1]][0]
        city2 = "None"
    else:
        city2 = "None"
        city1 = "None"

    #Draw all the labels
    screen.blit(font.render("Cities: " + city1 + " → " + city2, True, (0, 0, 0)),text_surface.get_rect(center=(1370, 190)) )
    screen.blit(font.render("Cost Weight (1-5): " + str(int(components[0].value)), True, (0, 0, 0)),text_surface.get_rect(center=(1390, 350)) )
    screen.blit(font.render("Time Weight (1-5): " + str(int(components[1].value)), True, (0, 0, 0)),text_surface.get_rect(center=(1390, 450)) )
    screen.blit(font.render("Carbon Weight (1-5): " + str(int(components[2].value)), True, (0, 0, 0)),text_surface.get_rect(center=(1380, 550)) )
   
    # Draw the components
    for component in components:
        component.draw()

    # Update the display
    pygame.display.flip()

# Quit Pygame
pygame.quit()
#Close backend
w.send("Bye!")
sys.exit()
