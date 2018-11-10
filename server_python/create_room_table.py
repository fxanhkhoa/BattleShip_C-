import mysql.connector

mydb = mysql.connector.connect(
  host="localhost",
  user="root",
  passwd="root",
  database="BATTLESHIP"
)

mycursor = mydb.cursor()

#mycursor.execute("CREATE TABLE ROOM ( \
#	id INT(6) UNSIGNED AUTO_INCREMENT PRIMARY KEY ,\
#	STATUS VARCHAR(2) \
#	)")

mycursor.execute("SHOW TABLES")
for x in mycursor:
  print(x)
  
sql = "INSERT INTO ROOM (STATUS) VALUES (%s)"
val = [('0')]
  
mycursor.executemany(sql, val)

mydb.commit()