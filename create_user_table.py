import mysql.connector

mydb = mysql.connector.connect(
  host="localhost",
  user="battleship",
  passwd="qwerty12345678",
  database="BATTLESHIP"
)

mycursor = mydb.cursor()
#mycursor.execute("CREATE DATABASE BATTLESHIP")

mycursor.execute("SHOW DATABASES")

for x in mycursor:
  print(x)
  
#mycursor.execute("CREATE TABLE USER ( \
#	id INT(6) UNSIGNED AUTO_INCREMENT PRIMARY KEY ,\
#	USERNAME VARCHAR(255), \
#	PASSWORD VARCHAR(255) \
#)")

sql = "INSERT INTO USER (USERNAME, PASSWORD) VALUES (%s, %s)"
val = [('player1','player1'),
       ('player2','player2')
]

mycursor.executemany(sql, val)

mydb.commit()