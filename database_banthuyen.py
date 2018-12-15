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
        
  
  def reset(self, room = 3):
      sql = "UPDATE GAMESTATUS SET SHIPVALUE = %s WHERE ROOM = %s"
      val = ('0', room)
        
      self.mycursor.execute(sql, val)

      self.mydb.commit()
      
      print(self.mycursor.rowcount, "record(s) affected")
      
  def select(self, id, x, y, step, room = 3):
      if step == 2:
        id_square = str(x * 10 + y + 100) #gop ID
        if (id == '2'):
          id_square = str(x * 10 + y)
      else:
        id_square = str(x * 10 + y) #gop ID
        if (id == '2'):
          id_square = str(x * 10 + y + 100)
          
      # clear all selected
      sql = "UPDATE GAMESTATUS SET SHIPVALUE = %s WHERE ROOM = %s AND SHIPVALUE = 'SHIP+SELECT'"
      val = ('SHIP', room)
        
      self.mycursor.execute(sql, val)

      self.mydb.commit()
      
      sql = "UPDATE GAMESTATUS SET SHIPVALUE = %s WHERE ROOM = %s AND SHIPVALUE = 'SELECT'"
      val = ('0', room)
        
      self.mycursor.execute(sql, val)

      self.mydb.commit()
      
      # Xem status hien tai
      sql = "SELECT SHIPVALUE FROM GAMESTATUS WHERE id_square = %s AND ROOM = %s"
      val = (id_square, room)
      self.mycursor.execute(sql, val)
      myresult = self.mycursor.fetchone()
      
      if (myresult[0] == 'SHIP'):
        input_value = 'SHIP+SELECT'
      elif (myresult[0] == 'FIRE'):
        input_value = 'FIRE+SELECT'
      elif (myresult[0] == 'X'):
        input_value = 'X+SELECT'
      else:
        input_value = 'SELECT'
      
      sql = "UPDATE GAMESTATUS SET SHIPVALUE = %s WHERE ROOM = %s AND id_square = %s"
      val = (input_value, room, id_square)
      
      self.mycursor.execute(sql, val)

      self.mydb.commit()
      
      print(self.mycursor.rowcount, "record(s) affected")
      
  def update_status(self, id_square, value, room = 3):
      print(id_square, " ", value)
      
      sql = "UPDATE GAMESTATUS SET SHIPVALUE = %s WHERE id_square = %s AND ROOM = %s"
      val = (value, id_square, room)
      
      self.mycursor.execute(sql, val)

      self.mydb.commit()
      
      print(self.mycursor.rowcount, "record(s) affected")
      return 1
