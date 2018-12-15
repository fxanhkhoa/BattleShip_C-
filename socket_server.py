import socket
import threading
import socketserver
import json
import game
import database_banthuyen as database

clients = [] #list of clients connected
game_control = game.game()
data = database.database()

######## Function to send data to all clients
def send_to_all_clients(data): 
    for request in clients:
        request.sendall(data)
    return

class ThreadedTCPRequestHandler(socketserver.BaseRequestHandler):

    def handle(self):
        #data = str(self.request.recv(1024), 'ascii')
        #cur_thread = threading.current_thread()
        #response = bytes("{}: {}".format(cur_thread.name, data), 'ascii')
        #self.request.sendall(response)
        ############################
        print("{} connected".format(self.client_address)) # show client's address
        clients.append(self.request) # add client to list
        close = 0
        while not close:
          try:
            buf = self.request.recv(1024)
            string_data = str(buf, 'ascii')
            #print(string_data)
            if (not string_data):
                print("{} closed".format(self.client_address))
                clients.remove(self.request)
                return
            
            ## Process here
            data_json = json.loads(string_data)
            print('command get: ',data_json["ID"], ' ', data_json["BUTTON"])
            process_input(data_json)
            
          except Exception as e:
                print(e)
                print("{} closed".format(self.client_address))
                clients.remove(self.request)
                #self.request.close()
                close = 1
                #break

class ThreadedTCPServer(socketserver.ThreadingMixIn, socketserver.TCPServer):
    pass

def process_input(data_json):
    print('processing')
    if data_json["ID"] == '1':
      print('toa do hien tai: ',game_control.player1_x, game_control.player1_y)
    else: 
      print('toa do hien tai: ',game_control.player2_x, game_control.player2_y)
      
    if data_json["BUTTON"] == 'RESET':
      data.reset()
      game_control.reset()
    if data_json["BUTTON"] == 'CANCEL':
      if game_control.current_step == game.picking:
        if data_json["ID"] == '1':
          game_control.player1_dimension = game.vertical
        if data_json["ID"] == '2':
          game_control.player2_dimension = game.vertical
        game_control.put_ship(data_json["ID"])
    if data_json["BUTTON"] == 'OK':
      print('step: ', game_control.current_step - game.picking)
      if game_control.current_step == game.picking:
        if data_json["ID"] == '1':
          game_control.player1_dimension = game.horizontal
        if data_json["ID"] == '2':
          game_control.player2_dimension = game.horizontal
        game_control.put_ship(data_json["ID"])
      elif game_control.current_step == game.play:
        game_control.fire_ship(data_json["ID"])
    
    elif data_json["BUTTON"] == 'UP':
      if data_json["ID"] == '1':
        game_control.player1_x -= 1
        if game_control.player1_x < 0:
            game_control.player1_x = 9
        data.select(data_json["ID"], game_control.player1_x, game_control.player1_y, game_control.current_step)
      else:
        game_control.player2_x -= 1
        if game_control.player2_x < 0:
            game_control.player2_x = 9
        data.select(data_json["ID"], game_control.player2_x, game_control.player2_y, game_control.current_step)
        
    elif data_json["BUTTON"] == "DOWN":
      if data_json["ID"] == '1':
        game_control.player1_x += 1
        if game_control.player1_x > 9:
            game_control.player1_x = 0
        data.select(data_json["ID"], game_control.player1_x, game_control.player1_y, game_control.current_step)
      else:
        game_control.player2_x += 1
        if game_control.player2_x > 9:
            game_control.player2_x = 0
        data.select(data_json["ID"], game_control.player2_x, game_control.player2_y, game_control.current_step)
        
    elif data_json["BUTTON"] == "RIGHT":
      if data_json["ID"] == '1':
        game_control.player1_y += 1
        if game_control.player1_y > 9:
          game_control.player1_y == 0
        data.select(data_json["ID"], game_control.player1_x, game_control.player1_y, game_control.current_step)
      else:
        game_control.player2_y += 1
        if game_control.player2_y > 9:
          game_control.player2_y == 0
        data.select(data_json["ID"], game_control.player2_x, game_control.player2_y, game_control.current_step)
        
    elif data_json["BUTTON"] == "LEFT":
      if data_json["ID"] == '1':
        game_control.player1_y -= 1
        if game_control.player1_y < 0:
          game_control.player1_y == 9
        data.select(data_json["ID"], game_control.player1_x, game_control.player1_y, game_control.current_step)
      else:
        game_control.player2_y -= 1
        if game_control.player2_y < 0:
          game_control.player2_y == 9
        data.select(data_json["ID"], game_control.player2_x, game_control.player2_y, game_control.current_step)
        
def get_ip_address():
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    s.connect(("8.8.8.8", 80))
    return s.getsockname()[0]

if __name__ == "__main__":
    # Port 0 means to select an arbitrary unused port
    ownIp = get_ip_address()
    print('my ip is: ', ownIp)
    HOST, PORT = ownIp, 3000

    server = ThreadedTCPServer((HOST, PORT), ThreadedTCPRequestHandler)
    ip, port = server.server_address

    # Start a thread with the server -- that thread will then start one
    # more thread for each request
    server_thread = threading.Thread(target=server.serve_forever)
    # Exit the server thread when the main thread terminates
    server_thread.daemon = True
    server_thread.start()
    print("Server loop running in thread:", server_thread.name)
    server.serve_forever()  
