import mysql.connector

mydb = mysql.connector.connect(
  host="localhost",
  user="battleship",
  passwd="qwerty12345678",
  database="BATTLESHIP"
)

mycursor = mydb.cursor()

#mycursor.execute("CREATE TABLE PSHAND ( \
#	id INT(6) UNSIGNED AUTO_INCREMENT PRIMARY KEY ,\
#	STATUS VARCHAR(2) \
#	)")

#mycursor.execute("SHOW TABLES")
#for x in mycursor:
#  print(x)
  
#sql = "INSERT INTO PSHAND (STATUS) VALUES (%s)"
#val = [('0'),
#       ('0')
#]
  
#mycursor.executemany(sql, val)

#mydb.commit()

mycursor.execute("SELECT * FROM PSHAND")

myresult = mycursor.fetchall()

for x in myresult:
  print(x)
  
mycursor.execute("UPDATE PSHAND SET STATUS = '0'")

mydb.commit()