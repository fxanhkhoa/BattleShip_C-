import Database

horizontal = 1
vertical = 2

class GameControl:
    # Init 2 team
    team1 = []
    team2 = []

    def __init__(self):
        print('initializing')
        self.database = Database.database()
        for i in range(0,8):
            self.team1.append([0,0,0,0,0,0,0,0])
            self.team2.append([0,0,0,0,0,0,0,0])
        print(self.team1[3][3])

    
    def fire(self, id, x, y):
        if id == 1:
            if self.team1[x][y] == 0: #x
                self.team1[x][y] = 1
                self.database.update_status(str(id) + str(x) + str(y), 1)
                return 1 #return to put result in dim
            elif self.team1[x][y] == 3 or self.team1[x][y] == 4 or self.team1[x][y] == 5: # fire
                self.team1[x][y] = 2
                self.database.update_status(str(id) + str(x) + str(y), 2)
                return 2 #return to put result in dim
        elif id == 2:
            if self.team2[x][y] == 0: #x
                self.team2[x][y] = 1
                self.database.update_status(str(id) + str(x) + str(y), 1)
                return 1 #return to put result in dim
            elif self.team2[x][y] == 3 or self.team2[x][y] == 4 or self.team2[x][y] == 5: # fire
                self.team2[x][y] = 2
                self.database.update_status(str(id) + str(x) + str(y), 2)
                return 2 #return to put result in dim
        return 0

    def put(self, id, x, y, dim, shipType):
        print('go here', id, x, y, dim, shipType)
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
        return
        
    def reset(self, room = 1):
      self.database.reset(room)

