import pygame
import math
import sys

class Slider:
    #Init function. NOTE: x_pos and y_pos are both the center
    def __init__( self, screen, x_pos, y_pos, width, height, min = 1, value = 1, max = 5, step = 1 ):
        self.screen = screen
        self.x = x_pos
        self.y = y_pos
        self.width = width
        self.height = height
        self.min = min
        self.value = value
        self.max = max
        self.step = step
        self.dragging = False

    def draw(self):
        if( self.dragging ):
            pos = pygame.mouse.get_pos()
            percentage = (pos[0] - (self.x - self.width/2 + self.height/2)) / (self.width - self.height)
            percentage = max( min( percentage, 1), 0)
            self.value = int( self.min + (self.max - self.min) * percentage * self.step ) / self.step

        #Get the top corner
        corner = (self.x - self.width/2, self.y - self.height/2)

        #Draw the fill (we'll do light grey for now)
        pygame.draw.circle(self.screen, (230, 230, 230), (corner[0] + self.height/2, corner[1] + self.height/2), self.height/2 )
        pygame.draw.circle(self.screen, (230, 230, 230), (corner[0] + self.width - self.height/2, corner[1] + self.height/2), self.height/2)
        pygame.draw.rect(self.screen, (230, 230, 230), (corner[0] + self.height/2, corner[1], self.width-self.height, self.height) )
        
        #Draw the slider circle
        displacemenet = (self.width - self.height) * (self.value - self.min) / (self.max-self.min)
        pygame.draw.circle(self.screen, (150, 150, 150),(corner[0] + self.height/2 + displacemenet, corner[1] + self.height/2), self.height/2 )
        pygame.draw.circle(self.screen, (100, 100, 100),(corner[0] + self.height/2 + displacemenet, corner[1] + self.height/2), self.height/2, 2 )

    def on_click(self, pos:tuple):
        if pos[0] > self.x - self.width/2 and pos[0] < self.x + self.width/2 and pos[1] > self.y - self.height/2 and pos[1] < self.y + self.height/2:
            self.dragging = True
    
    def on_release(self):
        self.dragging = False

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

pplSlider = Slider(screen, 1375, 250, 150, 50, max=30 )
costSlider = Slider(screen, 1375, 350, 150, 50 )
timeSlider = Slider(screen, 1375, 450, 150, 50 )
carbonSlider = Slider(screen, 1375, 550, 150, 50 )

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
            pplSlider.on_click(mouse_pos)
            costSlider.on_click(mouse_pos)
            timeSlider.on_click(mouse_pos)
            carbonSlider.on_click(mouse_pos)
            i = 0
            for city in cities:  
                coords = getPosFromCoords(city[1])
                distance = ((mouse_pos[0] - coords[0]) ** 2 + (mouse_pos[1] - coords[1]) ** 2) ** 0.5
                if( distance < 10 ):
                    cities_clicked.append(i)
                i += 1

        if event.type == pygame.MOUSEBUTTONUP:
            pplSlider.on_release()
            costSlider.on_release()
            timeSlider.on_release()
            carbonSlider.on_release()
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
        pygame.draw.line(screen, (0, 0, 255), getPosFromCoords(cities[currCity][1]), getPosFromCoords(cities[prevCity][1]), 3)

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

    screen.blit(font.render("City 1: " + city1, True, (0, 0, 0)),text_surface.get_rect(center=(1360, 100)) )
    screen.blit(font.render("City 2: " + city2, True, (0, 0, 0)),text_surface.get_rect(center=(1360, 150)) )
    screen.blit(font.render("People: " + str(int(pplSlider.value)), True, (0, 0, 0)),text_surface.get_rect(center=(1420, 220)) )
    screen.blit(font.render("Cost Weight (1-5): " + str(int(costSlider.value)), True, (0, 0, 0)),text_surface.get_rect(center=(1390, 320)) )
    screen.blit(font.render("Time Weight (1-5): " + str(int(timeSlider.value)), True, (0, 0, 0)),text_surface.get_rect(center=(1390, 420)) )
    screen.blit(font.render("Carbon Weight (1-5): " + str(int(carbonSlider.value)), True, (0, 0, 0)),text_surface.get_rect(center=(1380, 520)) )
    pplSlider.draw()
    costSlider.draw()
    timeSlider.draw()
    carbonSlider.draw()

    pygame.draw.rect( screen, (150, 150, 150), (1300, 600, 150, 75 ) )
    screen.blit(font.render("Generate Routes", True, (0, 0, 0)),text_surface.get_rect(center=(1390, 642)) )

    # Update the display
    pygame.display.flip()

# Quit Pygame
pygame.quit()
sys.exit()
