import socket

class Interface:

    def __init__( self, port ):

        # Create a client socket
        self.client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.client.connect(('localhost', port))
        self.client.setblocking(False)

    def send(self, message):
        self.client.sendall(message.encode())

    def recv(self):
        try:
            return self.client.recv(1024)
        except BlockingIOError:
            return ""

    def __del__(self):
        self.client.close()