import pygame
import math
import time
import sys
import socket
from components import *
from interface import *

#This translate coordinates into position 
def getPosFromCoords( coords ):
    return (coords[1] * 21.433 + 2686.177, coords[0] * -28.18455 + 1420.191)

def getCityPos( cityName ):
    for city in cities:
        if( city[0] == cityName ):
            return getPosFromCoords(city[1])
    return (0, 0)
city_list = []

# Initialize Pygame
pygame.init()

#The status message
status = "Backend Disconnected"

# Set up the display
screen = pygame.display.set_mode((1475, 700))  # Set screen size (width, height)
pygame.display.set_caption("Trip Planner")

# Load the image
image = pygame.image.load('map.jpg')  # Replace with your image path
# Set up the font
font = pygame.font.SysFont('Arial', 35)

# Text to display
text = "Trip Planner"

#Create the interface
w = Interface(8001)
status = "Backend Found!"
# Create a surface with the text
text_surface = font.render(text, True, (0, 0, 0))  # White color

# Get the position of the text
text_rect = text_surface.get_rect(center=(1375, 20))  

# This is the list of cities
cities = [["NYC", (40.713, -74.006)], ["CHI", (41.878, -87.6298)], ["PHI", (39.953, -75.1652)], ["WDC", (38.9072, -77.0369)],
          ["BOS", (42.361, -71.057)], ["LAX", (34.059, -118.243)], ["HOU", (29.758, -95.361)]]

def get_routes(source):
    city_list.clear()
    if source == "all":
        status = "All Routes Displayed"
        w.send("Display All")
    else:
        status = cities[source][0] + " Routes Displayed"
        w.send("Display " + cities[source][0])
    
    time.sleep(0.01)
    message = str(w.recv())[2:].split(";")
    busses = []
    airplanes = []
    trains = []
    for route in message:
        if( route == "b',," or route == "'" ):
            continue
        route = route.split(",")
        if( route[2] == "Bus"):
            busses.append((route[0], route[1], route[2]))
        elif( route[2] == "Train"):
            trains.append((route[0], route[1], route[2]))
        elif( route[2] == "Airplane"):
            airplanes.append((route[0], route[1], route[2]))

    city_list.extend(busses)
    city_list.extend(trains)
    city_list.extend(airplanes)

components = [Slider(screen, 1375, 380, 175, 50 ), Slider(screen, 1375, 480, 175, 50 ),Slider(screen, 1375, 580, 175, 50 ), 
              Button(screen, 1375, 650, 175, 50, text="Get Best Route"), Button(screen, 1375, 85, 175, 50, text="Display All Routes", on_press=lambda: get_routes("all")),
              Button(screen, 1375, 225, 175, 50, text="Display Source Routes", on_press=lambda: get_routes(cities_clicked[len(cities_clicked) - 1] if len(cities_clicked) != 0 else "None")), Button(screen, 1375, 290, 175, 50, text="Get Max Flow")]

cities_clicked = []
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

            #Go through each city and check
            i = 0
            for city in cities:  
                coords = getPosFromCoords(city[1])
                distance = ((mouse_pos[0] - coords[0]) ** 2 + (mouse_pos[1] - coords[1]) ** 2) ** 0.5
                if( distance < 10 ):
                    cities_clicked.append(i)
                i += 1

        # Go through each component and update it on mouse up
        if event.type == pygame.MOUSEBUTTONUP:
            for component in components:
                component.on_release()

    screen.fill((200, 200, 200))  # RGB color
    pygame.time.delay(10)

    
    # Blit (draw) the image on the screen
    screen.blit(image, (0, 0))  # (100, 100) is the position where the image will appear
    pygame.draw.line(screen, (0, 0, 0), (1275, 0), (1275, 700), 3)
    pygame.draw.line(screen, (0, 0, 0), (1275, 50), (1475, 50), 3)
    screen.blit(text_surface, text_rect)

    if( len( cities_clicked ) >= 2):
        currCity = cities_clicked[len(cities_clicked) - 1]
        prevCity = cities_clicked[len(cities_clicked) - 2]
        # pygame.draw.line(screen, (0, 0, 255), getPosFromCoords(cities[currCity][1]), getPosFromCoords(cities[prevCity][1]), 18)
        
        # pygame.draw.line(screen, (0, 150, 0), getPosFromCoords(cities[currCity][1]), getPosFromCoords(cities[prevCity][1]), 12)
        
    for city in city_list:    
        if( getCityPos(city[0]) == (0, 0) or getCityPos(city[1]) == (0, 0) ):
            continue
        color = (200, 200, 200)
        width = 2
        if( city[2] == "Bus"):
            color = (0, 0, 255)
            width = 12
        elif( city[2] == "Train"):
            color = (0, 150, 0)
            width = 6
        pygame.draw.line(screen, color, getCityPos(city[0]), getCityPos(city[1]), width)


    for city in cities:  
        coords = getPosFromCoords(city[1])
        pygame.draw.circle(screen, (255, 255, 255), coords, 8)
        font = pygame.font.SysFont('Arial', 20)
        screen.blit(font.render(city[0], True, (255, 255, 255)),text_surface.get_rect(center=(coords[0] + 30, coords[1] + 5)) )
    
    #Draw people slider
    font = pygame.font.SysFont('Arial', 20)
    if len(cities_clicked) >= 2:
        city2 = cities[cities_clicked[len(cities_clicked)-1]][0]
        city1 = cities[cities_clicked[len(cities_clicked)-2]][0]
    elif len(cities_clicked) == 1:
        city1 = cities[cities_clicked[len(cities_clicked)-1]][0]
        city2 = "None"
    else:
        city2 = "None"
        city1 = "None"

    screen.blit(font.render(status, True, (0, 0, 0)),text_surface.get_rect(center=(1370, 150)) )
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
sys.exit()
