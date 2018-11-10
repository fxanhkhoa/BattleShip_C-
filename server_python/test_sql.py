import mysql.connector

mydb = mysql.connector.connect(
  host="localhost",
  user="root",
  passwd="root",
  database="BATTLESHIP"
)

mycursor = mydb.cursor()

#mycursor.execute("CREATE DATABASE BATTLESHIP")
mycursor.execute("SHOW DATABASES")

for x in mycursor:
  print(x)
  
mycursor.execute("SELECT * FROM GAMESTATUS")

#sql = "SELECT * FROM GAMESTATUS WHERE id_square = '133' "
#id_square = '133'
#val = (id_square)
#mycursor.execute(sql)
myresult = mycursor.fetchall()

for x in myresult:
  print(x)
 
id_square = str(1) + str(3) + str(3) 
value = str(3)
room = '1'
  
sql = "UPDATE GAMESTATUS SET SHIPVALUE = %s WHERE (id_square = %s) and (ROOM = %s) "
val = (value, id_square, room)

mycursor.execute(sql, val)

mydb.commit()  
print(sql)
print(mycursor.rowcount, "record(s) affected")