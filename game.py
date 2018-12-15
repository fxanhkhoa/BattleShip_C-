import database_banthuyen

horizontal = 1
vertical = 2

start = 0
picking = 1
play = 2

## x-> 1
## fire -> 2
## ship -> 3

class game:
    # Init 2 team
    team1 = []
    team2 = []

    def __init__(self):
        print('initializing')
        self.current_step = 1
        self.player1_picking = 1
        self.player2_picking = 1
        self.currentPlayer = 1
        self.player1_x = 0
        self.player1_y = 0
        self.player1_dimension = horizontal
        self.player2_x = 0
        self.player2_y = 0
        self.player2_dimension = horizontal
        
        self.player1_hit = 0
        self.player2_hit = 0
        
        self.database = database_banthuyen.database()
        
        for i in range(0,9):
            self.team1.append([0,0,0,0,0,0,0,0])
            self.team2.append([0,0,0,0,0,0,0,0])
            
    def reset(self):
      self.current_step = 1
      self.player1_picking = 1
      self.player2_picking = 1
      self.currentPlayer = 1
      self.player1_x = 0
      self.player1_y = 0
      self.player1_dimension = horizontal
      self.player2_x = 0
      self.player2_y = 0
      self.player2_dimension = horizontal
      
      self.player1_hit = 0
      self.player2_hit = 0
      
      for i in range(0,9):
          self.team1.append([0,0,0,0,0,0,0,0])
          self.team2.append([0,0,0,0,0,0,0,0])
            
    def put_ship(self, id):
      print('putting ship')
      if not self.check_conflict(id):
        print('conflict ship!')
        return 0
      else:
        if id == '1':
          print(self.player1_x, self.player1_y)
          if (self.player1_picking == 1): #ship 1
            self.team1[self.player1_x][self.player1_y] = 3
            self.database.update_status(str(self.player1_x * 10 + self.player1_y), 'SHIP')
            self.player1_picking = 2
            print('ship 1 setted')
          elif (self.player1_picking == 2): #ship 2
            self.player1_picking = 3
            if self.player1_dimension == vertical:
              self.team1[self.player1_x][self.player1_y] = 3
              self.team1[self.player1_x + 1][self.player1_y] = 3
              self.database.update_status(str(self.player1_x * 10 + self.player1_y), 'SHIP')
              self.database.update_status(str((self.player1_x + 1) * 10 + self.player1_y), 'SHIP')
            else:
              self.team1[self.player1_x][self.player1_y] = 3
              self.team1[self.player1_x][self.player1_y + 1] = 3
              self.database.update_status(str(self.player1_x * 10 + self.player1_y), 'SHIP')
              self.database.update_status(str(self.player1_x * 10 + (self.player1_y + 1)), 'SHIP')
          elif (self.player1_picking == 3): #ship 3
            self.player1_picking = 4
            if self.player1_dimension == vertical:
              self.team1[self.player1_x][self.player1_y] = 3
              self.team1[self.player1_x + 1][self.player1_y] = 3
              self.team1[self.player1_x + 2][self.player1_y] = 3
              self.database.update_status(str(self.player1_x * 10 + self.player1_y), 'SHIP')
              self.database.update_status(str((self.player1_x + 1) * 10 + self.player1_y), 'SHIP')
              self.database.update_status(str((self.player1_x + 2) * 10 + self.player1_y), 'SHIP')
            else:
              self.team1[self.player1_x][self.player1_y] = 3
              self.team1[self.player1_x][self.player1_y + 1] = 3
              self.team1[self.player1_x][self.player1_y + 2] = 3
              self.database.update_status(str(self.player1_x * 10 + self.player1_y), 'SHIP')
              self.database.update_status(str(self.player1_x * 10 + (self.player1_y + 1)), 'SHIP')
              self.database.update_status(str(self.player1_x * 10 + (self.player1_y + 2)), 'SHIP')
            
        
        elif id == '2':
          if (self.player2_picking == 1): #ship 1
            self.player2_picking = 2
            self.team2[self.player2_x][self.player2_y] = 3
            self.database.update_status(str(self.player2_x * 10 + self.player2_y + 100), 'SHIP')
          elif (self.player2_picking == 2): #ship 2
            self.player2_picking = 3
            if self.player2_dimension == vertical:
              self.team2[self.player2_x][self.player2_y] = 3
              self.team2[self.player2_x + 1][self.player2_y] = 3
              self.database.update_status(str(self.player2_x * 10 + self.player2_y + 100), 'SHIP')
              self.database.update_status(str((self.player2_x + 1) * 10 + self.player2_y + 100), 'SHIP')
            else:
              self.team2[self.player2_x][self.player2_y] = 3
              self.team2[self.player2_x][self.player2_y + 1] = 3
              self.database.update_status(str(self.player2_x * 10 + self.player2_y + 100), 'SHIP')
              self.database.update_status(str(self.player2_x * 10 + (self.player2_y + 1) + 100), 'SHIP')
          elif (self.player2_picking == 3): #ship 3
            self.player2_picking = 4
            if self.player2_dimension == vertical:
              self.team2[self.player2_x][self.player2_y] = 3
              self.team2[self.player2_x + 1][self.player2_y] = 3
              self.team2[self.player2_x + 2][self.player2_y] = 3
              self.database.update_status(str(self.player2_x * 10 + self.player2_y + 100), 'SHIP')
              self.database.update_status(str((self.player2_x + 1) * 10 + self.player2_y + 100), 'SHIP')
              self.database.update_status(str((self.player2_x + 2) * 10 + self.player2_y + 100), 'SHIP')
            else:
              self.team2[self.player2_x][self.player2_y] = 3
              self.team2[self.player2_x][self.player2_y + 1] = 3
              self.team2[self.player2_x][self.player2_y + 2] = 3
              self.database.update_status(str(self.player2_x * 10 + self.player2_y + 100), 'SHIP')
              self.database.update_status(str(self.player2_x * 10 + (self.player2_y + 1) + 100), 'SHIP')
              self.database.update_status(str( self.player2_x * 10 + (self.player2_y + 2) + 100), 'SHIP')
      if (self.player1_picking == 4 and self.player2_picking == 4):
        self.current_step = play
      return 1
      
    def check_conflict(self, id):
      if id == '1':
          if (self.player1_picking == 1): #ship 1
              if self.team1[self.player1_x][self.player1_y] != 0:
                  return 0
          elif (self.player1_picking == 2): #ship 2
              if self.player1_dimension == vertical:
                  if self.team1[self.player1_x][self.player1_y] != 0:
                      return 0
                  if self.team1[self.player1_x + 1][self.player1_y] != 0:
                      return 0
              elif self.player1_dimension == horizontal:
                  if self.team1[self.player1_x][self.player1_y] != 0:
                      return 0
                  if self.team1[self.player1_x][self.player1_y + 1] != 0:
                      return 0
          elif self.player1_picking == 3: #ship 3
              if self.player1_dimension == vertical:
                  if self.team1[self.player1_x][self.player1_y] != 0:
                      return 0
                  if self.team1[self.player1_x + 1][self.player1_y] != 0:
                      return 0
                  if self.team1[self.player1_x + 2][self.player1_y] != 0:
                      return 0
              elif self.player1_dimension == horizontal:
                  if self.team1[self.player1_x][self.player1_y] != 0:
                      return 0
                  if self.team1[self.player1_x][self.player1_y + 1] != 0:
                      return 0
                  if self.team1[self.player1_x][self.player1_y + 2] != 0:
                      return 0
      
      
      elif id == '2':
          if self.player2_picking == 1:
              if self.team2[self.player2_x][self.player2_y] != 0:
                  return 0
          elif self.player2_picking == 2:
              if self.player2_dimension == vertical:
                  if self.team2[self.player2_x][self.player2_y] != 0:
                      return 0
                  if self.team2[self.player2_x + 1][self.player2_y] != 0:
                      return 0
              elif self.player2_dimension == horizontal:
                  if self.team2[self.player2_x][self.player2_y] != 0:
                      return 0
                  if self.team2[self.player2_x][self.player2_y + 1] != 0:
                      return 0
                      
          elif self.player2_picking == 3:
              if self.player2_dimension == vertical:
                  if self.team2[self.player2_x][self.player2_y] != 0:
                      return 0
                  if self.team2[self.player2_x + 1][self.player2_y] != 0:
                      return 0
                  if self.team2[self.player2_x + 2][self.player2_y] != 0:
                      return 0
                  
              elif self.player2_dimension == horizontal:
                  if self.team2[self.player2_x][self.player2_y] != 0:
                      return 0
                  if self.team2[self.player2_x][self.player2_y + 1] != 0:
                      return 0
                  if self.team2[self.player2_x][self.player2_y + 2] != 0:
                      return 0

      return 1

    def fire_ship(self, id):
      if id == '1': # xet doi thu la id 2
        if (self.team2[self.player1_x][self.player1_y] == 3):
          self.database.update_status(str(self.player1_x * 10 + self.player1_y + 100), 'FIRE')
          self.player2_hit += 1
        else:
          self.database.update_status(str(self.player1_x * 10 + self.player1_y + 100), 'X')
      elif id == '2': # xet doi thu la id 1
        if (self.team1[self.player2_x][self.player2_y] == 3):
          self.database.update_status(str(self.player2_x * 10 + self.player2_y), 'FIRE')
          self.player1_hit += 1
        else:
          self.database.update_status(str(self.player2_x * 10 + self.player2_y), 'X')
      
      if self.player1_hit == 6:
        print('team 2 WIN!')
      if self.player2_hit == 6:
        print('team 1 WIN!')