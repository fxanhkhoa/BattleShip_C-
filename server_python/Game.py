import Database

horizontal = 1
vertical = 2

class GameControl:
    # Init 2 team
    team1 = []
    team2 = []

    def __init__(self):
        print('initializing')
        self.shipPicking_player1 = 0
        self.shipPicking_player2 = 0
        self.donePutting_player1 = 0
        self.donePutting_player2 = 0
        self.currentPlayer = 1
        
        self.timeoutPlayer = 1
        
        self.time = 0
        
        self.player1_hit = 0
        self.player2_hit = 0
        
        self.step = 1
        
        self.database = Database.database()
        self.database.set_turn(1)
        self.database.update_step(1)
        self.database.set_time(10)
        for i in range(0,8):
            self.team1.append([0,0,0,0,0,0,0,0])
            self.team2.append([0,0,0,0,0,0,0,0])
        print(self.team1[3][3])

    
    def fire(self, id, x, y):
        print('coordinate: ',id,x,y)
        print('team1_hit', self.player2_hit)
        print('team2_hit', self.player1_hit)
        if id == 1:
            if self.team2[x][y] == 0: #x
                self.team2[x][y] = 1
                self.database.update_status(str(2) + str(x) + str(y), 1)
                return 1 #return to put result in dim
            elif self.team2[x][y] == 3 or self.team2[x][y] == 4 or self.team2[x][y] == 5: # fire
                self.team2[x][y] = 2
                self.database.update_status(str(2) + str(x) + str(y), 2)
                self.player2_hit += 1
                return 2 #return to put result in dim
        elif id == 2:
            if self.team1[x][y] == 0: #x
                self.team1[x][y] = 1
                self.database.update_status(str(1) + str(x) + str(y), 1)
                return 1 #return to put result in dim
            elif self.team1[x][y] == 3 or self.team1[x][y] == 4 or self.team1[x][y] == 5: # fire
                self.team2[x][y] = 2
                self.database.update_status(str(1) + str(x) + str(y), 2)
                self.player1_hit += 1
                return 2 #return to put result in dim
                
        if self.player1_hit == 6:
          self.database.set_turn(4)
        elif (self.player2_hit == 6):
          self.database.set_turn(3)
        return 0

    def put(self, id, x, y, dim, shipType):
        print('go here', id, x, y, dim, shipType)
        if not (self.checkPutting(id, x, y, dim, shipType)):
          if (id == 1):
            self.shipPicking_player1 = self.shipPicking_player1 - 1
          elif (id == 2):
            self.shipPicking_player2 = self.shipPicking_player2 - 1
          return 0
        if id == 1:
            if shipType == 3:
                #print('setting')
                self.team1[x][y] = 3
                self.database.update_status(str(id) + str(x) + str(y), shipType)
                #print('setted')
            elif shipType == 4:
                if dim == vertical:
                    self.team1[x][y] = 4
                    self.team1[x + 1][y] = 4
                    self.database.update_status(str(id) + str(x) + str(y), shipType)
                    self.database.update_status(str(id) + str(x + 1) + str(y), shipType)
                elif dim == horizontal:
                    self.team1[x][y] = 4
                    self.team1[x][y + 1] = 4
                    self.database.update_status(str(id) + str(x) + str(y), shipType)
                    self.database.update_status(str(id) + str(x) + str(y + 1), shipType)
            elif shipType == 5:
                if dim == vertical:
                    self.team1[x][y] = 5
                    self.team1[x + 1][y] = 5
                    self.team1[x + 2][y] = 5
                    self.database.update_status(str(id) + str(x) + str(y), shipType)
                    self.database.update_status(str(id) + str(x + 1) + str(y), shipType)
                    self.database.update_status(str(id) + str(x + 2) + str(y), shipType)
                elif dim == horizontal:
                    self.team1[x][y] = 5
                    self.team1[x][y + 1] = 5
                    self.team1[x][y + 2] = 5
                    self.database.update_status(str(id) + str(x) + str(y), shipType)
                    self.database.update_status(str(id) + str(x) + str(y + 1), shipType)
                    self.database.update_status(str(id) + str(x) + str(y + 2), shipType)

        if id == 2:
            if shipType == 3:
                self.team2[x][y] = 3
                self.database.update_status(str(id) + str(x) + str(y), shipType)
            elif shipType == 4:
                if dim == vertical:
                    self.team2[x][y] = 4
                    self.team2[x + 1][y] = 4
                    self.database.update_status(str(id) + str(x) + str(y), shipType)
                    self.database.update_status(str(id) + str(x + 1) + str(y), shipType)
                elif dim == horizontal:
                    self.team2[x][y] = 4
                    self.team2[x][y + 1] = 4
                    self.database.update_status(str(id) + str(x) + str(y), shipType)
                    self.database.update_status(str(id) + str(x) + str(y + 1), shipType)
            elif shipType == 5:
                if dim == vertical:
                    self.team2[x][y] = 5
                    self.team2[x + 1][y] = 5
                    self.team2[x + 2][y] = 5
                    self.database.update_status(str(id) + str(x) + str(y), shipType)
                    self.database.update_status(str(id) + str(x + 1) + str(y), shipType)
                    self.database.update_status(str(id) + str(x + 2) + str(y), shipType)
                elif dim == horizontal:
                    self.team2[x][y] = 5
                    self.team2[x][y + 1] = 5
                    self.team2[x][y + 2] = 5
                    self.database.update_status(str(id) + str(x) + str(y), shipType)
                    self.database.update_status(str(id) + str(x) + str(y + 1), shipType)
                    self.database.update_status(str(id) + str(x) + str(y + 2), shipType)

        print(self.team1)
        print(self.team2)
        return 1
        
    def reset(self, room = 1):
      self.database.reset(room)
      
    def checkPutting(self, id, x, y, dim, shipType):
      if id == 1:
            if shipType == 3:
                if self.team1[x][y] != 0:
                  return 0
            elif shipType == 4:
                if dim == vertical:
                    if self.team1[x][y] != 0:
                      return 0
                    if self.team1[x + 1][y] != 0:
                      return 0
                elif dim == horizontal:
                    if self.team1[x][y] != 0:
                      return 0
                    if self.team1[x][y + 1] != 0:
                      return 0
            elif shipType == 5:
                if dim == vertical:
                    if self.team1[x][y] != 0:
                      return 0
                    if self.team1[x + 1][y] != 0:
                      return 0
                    if self.team1[x + 2][y] != 0:
                      return 0
                elif dim == horizontal:
                    if self.team1[x][y] != 0:
                      return 0
                    if self.team1[x][y + 1] != 0:
                      return 0
                    if self.team1[x][y + 2] != 0:
                      return 0
          

      if id == 2:
            if shipType == 3:
                if self.team2[x][y] != 0:
                  return 0
            elif shipType == 4:
                if dim == vertical:
                    if self.team2[x][y] != 0:
                      return 0
                    if self.team2[x + 1][y] != 0:
                      return 0
                elif dim == horizontal:
                    if self.team2[x][y] != 0:
                      return 0
                    if self.team2[x][y + 1] != 0:
                      return 0
                    
            elif shipType == 5:
                if dim == vertical:
                    if self.team2[x][y] != 0:
                      return 0
                    if self.team2[x + 1][y] != 0:
                      return 0
                    if self.team2[x + 2][y] != 0:
                      return 0
                    
                elif dim == horizontal:
                    if self.team2[x][y] != 0:
                      return 0
                    if self.team2[x][y + 1] != 0:
                      return 0
                    if self.team2[x][y + 2] != 0:
                      return 0
      return 1

