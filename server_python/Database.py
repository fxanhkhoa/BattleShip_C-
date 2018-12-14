import mysql.connector
from termcolor import colored


class database:
    

    def __init__(self):
        self.mydb = mysql.connector.connect(
            host="localhost",
            user="battleship",
            passwd="qwerty12345678",
            database="BATTLESHIP"
            )
        self.mycursor = self.mydb.cursor()

    def signUp(self, buf):
        username = buf[2:7]
        password = buf[8:13]
        sql = "INSERT INTO USER (USERNAME, PASSWORD) VALUES (%s, %s)"
        val = (str(username), str(password))

        try:
            self.mycursor.execute(sql, val)
            self.mydb.commit()

            print(self.mycursor.rowcount)
            print(colored("Sign up successful.", 'green'))
        except Exception as e:
            print(e)

    def signIn(self, buf):
        username = buf[2:7]
        password = buf[8:13]
        sql = "SELECT * FROM USER Where USERNAME = %s and PASSWORD = %s"
        val = (str(username), str(password))

        try:
            self.mycursor.execute(sql, val)
            myresult = self.mycursor.fetchall()
            print(self.mycursor)
            print(myresult)
            if myresult:
                print(self.mycursor.rowcount)
                print(colored("Sign in successful.", 'green'))
            else:
                print(colored('wrong username or password', 'red'))
        except Exception as e:
            print(e)
          
    def update_status(self, id_square, value, room = 1):
        print(id_square, " ", value)
        
        sql = "SELECT SHIPVALUE FROM GAMESTATUS WHERE id_square = %s AND ROOM = %s"
        val = (id_square, room)
        self.mycursor.execute(sql, val)
        myresult = self.mycursor.fetchone()
        
        print('ship value= ', myresult[0])
        # 6 = select signal in web
        #if (myresult[0] == '3'):
        #  value = 3 + 6
        #if (myresult[0] == '4'):
        #  value = 4 + 6
        #if (myresult[0] == '5'):
        #  value = 5 + 6
        #print('value to update = ', value)
        
        sql = "UPDATE GAMESTATUS SET SHIPVALUE = %s WHERE id_square = %s AND ROOM = %s"
        val = (value, id_square, room)
        
        self.mycursor.execute(sql, val)

        self.mydb.commit()
        
        print(self.mycursor.rowcount, "record(s) affected")
        return 1
     
    def reset(self, room = 1):
      sql = "UPDATE GAMESTATUS SET SHIPVALUE = %s WHERE ROOM = %s"
      val = ('0', room)
        
      self.mycursor.execute(sql, val)

      self.mydb.commit()
      
      print(self.mycursor.rowcount, "record(s) affected")
      
    def set_time(self, time, room = 1):
      sql = "UPDATE TIME SET TIME_NOW = %s WHERE ROOM = %s"
      val = (str(time), room)
        
      self.mycursor.execute(sql, val)

      self.mydb.commit()
      
      print(self.mycursor.rowcount, "record(s) affected")
      
    def set_turn(self, turn, room = 1):
      sql = "UPDATE TURN SET CURRENT_TURN = %s WHERE ROOM = %s"
      
      val = (str(turn), room)
        
      self.mycursor.execute(sql, val)

      self.mydb.commit()
      
      print(self.mycursor.rowcount, "record(s) affected")
      
      return 1
      
    def update_step(self, step, room = 1):
      sql = "UPDATE STEP SET CURRENT_STEP = %s WHERE ROOM = %s"
      
      val = (str(step), room)
        
      self.mycursor.execute(sql, val)

      self.mydb.commit()
      
      print(self.mycursor.rowcount, "record(s) affected")
      
      return 1
      
    def select(self, buf, step, room = 1):
      print('step = ', step)
      id = buf[0]
      if step == 3:
        #print(step, id)
        if id == 1:
          #print('in here')
          id = 2
          print(id)
        else:
          id = 1
      x = buf[2]
      y = buf[3]
      id_square = str(id) + str(x) + str(y)
      print(id_square)
      ################
      if id == 1:
      
        sql = "UPDATE GAMESTATUS SET SHIPVALUE = %s WHERE ROOM = %s AND SHIPVALUE = '6' AND id_square < 200"
        val = ('0', room)
          
        self.mycursor.execute(sql, val)

        self.mydb.commit()
        
        sql = "UPDATE GAMESTATUS SET SHIPVALUE = %s WHERE ROOM = %s AND SHIPVALUE = '7' AND id_square < 200"
        val = ('1', room)
          
        self.mycursor.execute(sql, val)

        self.mydb.commit()
        
        sql = "UPDATE GAMESTATUS SET SHIPVALUE = %s WHERE ROOM = %s AND SHIPVALUE = '8' AND id_square < 200"
        val = ('2', room)
          
        self.mycursor.execute(sql, val)

        self.mydb.commit()
        
        sql = "UPDATE GAMESTATUS SET SHIPVALUE = %s WHERE ROOM = %s AND SHIPVALUE = '9' AND id_square < 200"
        val = ('3', room)
          
        self.mycursor.execute(sql, val)

        self.mydb.commit()
        
        sql = "UPDATE GAMESTATUS SET SHIPVALUE = %s WHERE ROOM = %s AND SHIPVALUE = '10' AND id_square < 200"
        val = ('4', room)
          
        self.mycursor.execute(sql, val)

        self.mydb.commit()
        
        sql = "UPDATE GAMESTATUS SET SHIPVALUE = %s WHERE ROOM = %s AND SHIPVALUE = '11' AND id_square < 200"
        val = ('5', room)
          
        self.mycursor.execute(sql, val)

        self.mydb.commit()
      
      elif id == 2:
        
        sql = "UPDATE GAMESTATUS SET SHIPVALUE = %s WHERE ROOM = %s AND SHIPVALUE = '6' AND id_square > 199"
        val = ('0', room)
          
        self.mycursor.execute(sql, val)

        self.mydb.commit()
        
        sql = "UPDATE GAMESTATUS SET SHIPVALUE = %s WHERE ROOM = %s AND SHIPVALUE = '7' AND id_square > 199"
        val = ('1', room)
          
        self.mycursor.execute(sql, val)

        self.mydb.commit()
        
        sql = "UPDATE GAMESTATUS SET SHIPVALUE = %s WHERE ROOM = %s AND SHIPVALUE = '8' AND id_square > 199"
        val = ('2', room)
          
        self.mycursor.execute(sql, val)

        self.mydb.commit()
        
        sql = "UPDATE GAMESTATUS SET SHIPVALUE = %s WHERE ROOM = %s AND SHIPVALUE = '9' AND id_square > 199"
        val = ('3', room)
          
        self.mycursor.execute(sql, val)

        self.mydb.commit()
        
        sql = "UPDATE GAMESTATUS SET SHIPVALUE = %s WHERE ROOM = %s AND SHIPVALUE = '10' AND id_square > 199"
        val = ('4', room)
          
        self.mycursor.execute(sql, val)

        self.mydb.commit()
        
        sql = "UPDATE GAMESTATUS SET SHIPVALUE = %s WHERE ROOM = %s AND SHIPVALUE = '11' AND id_square > 199"
        val = ('5', room)
          
        self.mycursor.execute(sql, val)

        self.mydb.commit()
      ################
      
      sql = "SELECT SHIPVALUE FROM GAMESTATUS WHERE id_square = %s AND ROOM = %s"
      val = (id_square, room)
      self.mycursor.execute(sql, val)
      myresult = self.mycursor.fetchone()
      
      print('ship value= ', myresult[0])
      input_value = 0
      # 6 = select signal in web
      if (myresult[0] == '1'):
        input_value = 1 + 6
      elif (myresult[0] == '2'):
        input_value = 2 + 6
      elif (myresult[0] == '3'):
        input_value = 3 + 6
      elif (myresult[0] == '4'):
        input_value = 4 + 6
      elif (myresult[0] == '5'):
        input_value = 5 + 6
      else:
        input_value = 6
      print('value to update = ', input_value)
      
      sql = "UPDATE GAMESTATUS SET SHIPVALUE = %s WHERE ROOM = %s AND id_square = %s"
      val = (str(input_value), room, id_square)
        
      self.mycursor.execute(sql, val)

      self.mydb.commit()
      
      print(self.mycursor.rowcount, "record(s) affected")

