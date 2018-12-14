import socket
import threading
import socketserver
#import getip
import Game
import Database
import time

number = 65

dump = 0
Fire = 1
PutShip1 = 2
PutShip2 = 3
PutShip3 = 4
SignUp = 5
SignIn = 6
Select = 7
reset = 12
 
id = 0
x = 0
y = 0
dim = 0
shiptype = 0
command = 0

step = 0

gameControl = Game.GameControl()
database = Database.database()



clients = []

def send_to_all_clients(data):
    for request in clients:
        request.sendall(data)
    return

class play_time(threading.Thread):
  def __init__(self, threadID, name, counter):
      threading.Thread.__init__(self)
      self.threadID = threadID
      self.name = name
      self.counter = counter
  def run(self):
      while (True):
        if (gameControl.step == 3):
          database.set_time(round(time.time() - gameControl.time, 4))
          if (time.time() - gameControl.time >= 10):
            if (gameControl.currentPlayer == 1):
              gameControl.currentPlayer = 2
              database.set_turn(2)
              gameControl.time = time.time()
              database.set_time(10)
            elif (gameControl.currentPlayer == 2):
              gameControl.currentPlayer = 1
              database.set_turn(1)
              gameControl.time = time.time()
              database.set_time(10)    
    
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
                result = process(bufArray)
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
    try:
      id = buf[0]
      command = buf[1]
      if command == 2:
        print(gameControl.donePutting_player1, gameControl.donePutting_player1)
        if gameControl.donePutting_player1 == 1 and gameControl.donePutting_player2 == 1:
          print('----------now fire-----------')
          command = Fire
          gameControl.step = 3
          database.update_step(3)
        else:
          if (id == 1):
            command = command + gameControl.shipPicking_player1
            gameControl.shipPicking_player1 = gameControl.shipPicking_player1 + 1
          elif (id == 2):
            command = command + gameControl.shipPicking_player2
            gameControl.shipPicking_player2 = gameControl.shipPicking_player2 + 1
      x = buf[2]
      y = buf[3]
      dim = buf[4]
      print(id,' ', command, ' ', x, ' ', y, ' ', dim)
    except Exception as e:
      print('Error', e)
    return

def process(buf):
    global id
    global x
    global y
    global dim
    global shiptype
    global command
    global gameControl
    print('processing ', command, ' ', PutShip1)
    if command == Fire:
        ## Check turn
        print('command: FIRE with id = ', id)
        print('current player is ', gameControl.currentPlayer)
        
        if (id == gameControl.currentPlayer):
          ## reset timeout
          gameControl.time = time.time()
          database.set_time(10)
          if (id == 1):
            gameControl.currentPlayer = 2
            database.set_turn(2)
          elif (id == 2):
            gameControl.currentPlayer = 1
            database.set_turn(1)
          result = gameControl.fire(id, x, y)
          return result
    elif command == PutShip1:
        gameControl.put(id, x, y, dim, 3)
    elif command == PutShip2:
        gameControl.put(id, x, y, dim, 4)
    elif command == PutShip3:
        done = gameControl.put(id, x, y, dim, 5)
        if done == 1 and id == 1:
          gameControl.donePutting_player1 = 1
          print('player 1 finished putting')
        elif done == 1 and id == 2:
          gameControl.donePutting_player2 = 1
          print('player 2 finished putting')
          
        if gameControl.donePutting_player1 == 1 and gameControl.donePutting_player2 == 1:
          print('----------now fire-----------')
          gameControl.step = 3
          database.update_step(3)
          gameControl.timeoutPlayer = gameControl.currentPlayer
          gameControl.time = time.time()
          database.set_time(10)
          play_thread.start()
    elif command == reset:
        gameControl = Game.GameControl()
        gameControl.reset(room = 1)
    elif command == SignUp:
        database.signUp(buf)
    elif command == SignIn:
        database.signIn(buf)
    elif command == Select:
        database.select(buf, gameControl.step, room = 1)
    else:
        print('command is not in list')
    return 0
########################    
    
    

play_thread = play_time(1, 'play_thread', 1)
 
 
######################## 

def get_ip_address():
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    s.connect(("8.8.8.8", 80))
    return s.getsockname()[0]

def main():
    ownIp = get_ip_address()
    #ownIp = socket.gethostbyname(socket.gethostname())
    #ownIp = '192.168.0.108'
    print('my ip is: ', ownIp)
    #print ownIp
    HOST, PORT = ownIp, 8000

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