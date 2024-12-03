import pygame
import math

#This is the parent class for all components
class Component:
    #Init function. NOTE: x_pos and y_pos are both the center
    def __init__( self, screen, x_pos, y_pos, width, height ):
        self.screen = screen
        self.x = x_pos
        self.y = y_pos
        self.width = width
        self.height = height
        self.min = min
    
    #This is the default draw function
    def draw(self):
        pass

    #These are the on click & on release functions
    def on_click(self, pos:tuple): pass
    def on_release(self): pass

#This is a slider
class Slider(Component):
    #Init function. NOTE: x_pos and y_pos are both the center
    def __init__( self, screen, x_pos, y_pos, width, height, min = 1, value = 1, max = 5, step = 1 ):
        Component.__init__(self, screen, x_pos, y_pos, width, height )
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

class Button(Component):
    def __init__( self, screen, x_pos, y_pos, width, height, on_press = lambda: None, text="", color=(150, 150, 150) ):
        Component.__init__(self, screen, x_pos, y_pos, width, height)
        self.on_press = on_press
        self.text = text
        self.color = color

    def draw(self):
        pygame.draw.rect( self.screen, self.color, (self.x - self.width/2, self.y - self.height/2, self.width, self.height ) )
        font = pygame.font.SysFont('Arial', 20)
        self.screen.blit(font.render(self.text, True, (0, 0, 0)), font.render(self.text, True, (0, 0, 0)).get_rect(center=(self.x, self.y)) )

    def on_click(self, pos:tuple):
            if pos[0] > self.x - self.width/2 and pos[0] < self.x + self.width/2 and pos[1] > self.y - self.height/2 and pos[1] < self.y + self.height/2:
                self.on_press()
    
