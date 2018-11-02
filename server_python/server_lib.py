import socket
import threading
import socketserver
import Game

number = 65

Fire = 1
PutShip1 = 2
PutShip2 = 3
PutShip3 = 4
reset = 12
 
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

class ThreadedTCPRequestHandler(socketserver.BaseRequestHandler):

    def handle(self):
        print("{} connected".format(self.client_address))
        clients.append(self.request)
        close = 0
        while not close:
            try:
                buf = self.request.recv(1024)

                if (not buf):
                    print("{} closed".format(self.client_address))
                    clients.remove(self.request)
                    return
                    
                bufArray = bytearray(buf)
                data = str(buf, 'ascii')
                print(data)
                seperate(buf)
                result = process()
                cur_thread = threading.current_thread()
                response = bytes("{}: {}".format(cur_thread.name, data), 'ascii')
                #self.request.sendall(response)

                print('result = ', result)

                if result == 1:
                    bufArray[4] = 1
                elif result == 2:
                    bufArray[4] = 2

                if bufArray[1] == 69:
                    print("{} closed".format(self.client_address))
                    clients.remove(self.request)
                    close = 1
                send_to_all_clients(bufArray)
            except Exception as e:
                print(e)
                print("{} closed".format(self.client_address))
                clients.remove(self.request)
                #self.request.close()
                close = 1
                #break

class ThreadedTCPServer(socketserver.ThreadingMixIn, socketserver.TCPServer):
    pass

def client(ip, port, message):
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.connect((ip, port))
    try:
        sock.sendall(bytes(message, 'ascii'))
        response = str(sock.recv(1024), 'ascii')
        print("Received: {}".format(response))
    except:
        pass
    finally:
        pass
        #sock.close()

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
    elif command == reset:
        gameControl = Game.GameControl()
    else:
        print('command is not in list')
    return 0


def main():
    ownIp = socket.gethostbyname(socket.gethostname())
    #ownIp = '192.168.0.108'
    print('my ip is: ', ownIp)
    #print ownIp
    HOST, PORT = ownIp, 8500

    server = ThreadedTCPServer((HOST, PORT), ThreadedTCPRequestHandler)
    ip, port = server.server_address
    try:
        # Port 0 means to select an arbitrary unused port
        

        # Start a thread with the server -- that thread will then start one
        # more thread for each request
        server_thread = threading.Thread(target=server.serve_forever)
        # Exit the server thread when the main thread terminates
        server_thread.daemon = True
        server_thread.start()
        #print("Server loop running in thread:", server_thread.name)

        #client(ip, port, "Hello World 1")
        #client(ip, port, "Hello World 2")
        #client(ip, port, "Hello World 3")

        server.serve_forever()

    except:
        server.shutdown()
        server.server_close()

if __name__ == "__main__":
    try:
        main()
    except:
        pass