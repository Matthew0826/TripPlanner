import pygame
import sys


def getPosFromCoords( coords ):
    return (coords[1] * 21.433 + 2686.177, coords[0] * -28.18455 + 1420.191)

# Initialize Pygame
pygame.init()

# Set up the display
screen = pygame.display.set_mode((1475, 700))  # Set screen size (width, height)
pygame.display.set_caption("Trip Planner")

# Load the image
image = pygame.image.load('map.jpg')  # Replace with your image path
# Set up the font
font = pygame.font.SysFont('Arial', 35)

# Text to display
text = "Trip Planner"

# Create a surface with the text
text_surface = font.render(text, True, (0, 0, 0))  # White color

# Get the position of the text
text_rect = text_surface.get_rect(center=(1375, 20))  

cities = [["NYC", (40.713, -74.006)], ["CHI", (41.878, -87.6298)], ["PHI", (39.953, -75.1652)], ["WDC", (38.9072, -77.0369)],
          ["BOS", (42.361, -71.057)], ["LAX", (34.059, -118.243)], ["HOU", (29.758, -95.361)]]

cities_clicked = []
# Game loop
running = True
while running:
    # Handle events
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False
        # Check if the mouse button is pressed
        if event.type == pygame.MOUSEBUTTONDOWN:
            # Get the position of the mouse click
            mouse_pos = pygame.mouse.get_pos()

            # Check if the click is inside the circle
            i = 0
            for city in cities:  
                coords = getPosFromCoords(city[1])
                distance = ((mouse_pos[0] - coords[0]) ** 2 + (mouse_pos[1] - coords[1]) ** 2) ** 0.5
                if( distance < 10 ):
                    print( city[0] )
                    cities_clicked.append(i)
                i += 1

            # Fill the screen with a color (optional, black in this case)
    screen.fill((200, 200, 200))  # RGB color
    pygame.time.delay(10)
    # Blit (draw) the image on the screen
    screen.blit(image, (0, 0))  # (100, 100) is the position where the image will appear
    pygame.draw.line(screen, (0, 0, 0), (1275, 0), (1275, 640), 3)
    pygame.draw.line(screen, (0, 0, 0), (1275, 50), (1475, 50), 3)
    screen.blit(text_surface, text_rect)
    if( len( cities_clicked ) >= 2):
        currCity = cities_clicked[len(cities_clicked) - 1]
        prevCity = cities_clicked[len(cities_clicked) - 2]
        pygame.draw.line(screen, (0, 0, 255), getPosFromCoords(cities[currCity][1]), getPosFromCoords(cities[prevCity][1]), 3)

    for city in cities:  
        coords = getPosFromCoords(city[1])
        pygame.draw.circle(screen, (255, 255, 255), coords, 8)
        font = pygame.font.SysFont('Arial', 20)
        screen.blit(font.render(city[0], True, (255, 255, 255)),text_surface.get_rect(center=(coords[0] + 30, coords[1] + 5)) )
    
    # Update the display
    pygame.display.flip()

# Quit Pygame
pygame.quit()
sys.exit()
