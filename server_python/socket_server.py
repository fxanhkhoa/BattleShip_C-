import socket
import threading
import socketserver
import Game

Fire = 1
PutShip1 = 2
PutShip2 = 3
PutShip3 = 4
 
id = 0
x = 0
y = 0
dim = 0
shiptype = 0
command = 0

gameControl = Game.GameControl()

clients = []

def send_to_all_clients(data):
    for request in clients:
        request.sendall(data)
    return

class MyTCPHandler(socketserver.BaseRequestHandler):
    """
    The RequestHandler class for our server.

    It is instantiated once per connection to the server, and must
    override the handle() method to implement communication to the
    client.
    """

    def handle(self):
        print("{} connected".format(self.client_address))
        clients.append(self.request)
        while True:
            # self.request is the TCP socket connected to the client
            self.data = self.request.recv(1024).strip()

            # is disconnected
            if not self.data:
                print("{} closed".format(self.client_address))
                clients.remove(self.request)
                return

            bufArray = bytearray(self.data)
            
            # Print data out
            print("{} wrote:".format(self.client_address[0]))
            print(self.data)
            
            #seperate(bufArray)
            #result = process()

            #print('result = ', result)

            #if result == 1:
                #bufArray[4] = 1 + number
            #elif result == 2:
                #bufArray[4] = 2 + number

            if bufArray[1] == 69:
                print("{} closed".format(self.client_address))
                return
            send_to_all_clients(bufArray)

            
            # just send back the same data, but upper-cased
            #self.request.sendall(self.data.upper())


def seperate(buf):
    global id
    global x
    global y
    global dim
    global shiptype
    global command
    print('seperating')
    id = buf[0]
    command = buf[1]
    x = buf[2]
    y = buf[3]
    dim = buf[4]
    print(id,' ', command, ' ', x, ' ', y, ' ', dim)
    return

def process():
    global id
    global x
    global y
    global dim
    global shiptype
    global command
    global gameControl
    print('processing ', command, ' ', PutShip1)
    if command == Fire:
        result = gameControl.fire(id, x, y)
        return result
    elif command == PutShip1:
        gameControl.put(id, x, y, dim, 3)
    elif command == PutShip2:
        gameControl.put(id, x, y, dim, 4)
    elif command == PutShip3:
        gameControl.put(id, x, y, dim, 5)
    else:
        print('command is not in list')
    return 0


if __name__ == "__main__":
    ownIp = socket.gethostbyname(socket.gethostname())
    #ownIp = '192.168.0.108'
    print('my ip is: ', ownIp)
    #print ownIp
    HOST, PORT = ownIp, 8500
    #HOST, PORT = "localhost", 8500

    # Create the server, binding to localhost on port 9999
    server = socketserver.TCPServer((HOST, PORT), MyTCPHandler)

    # Activate the server; this will keep running until you
    # interrupt the program with Ctrl-C
    server.serve_forever()