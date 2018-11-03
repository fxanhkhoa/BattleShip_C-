import mysql.connector
from termcolor import colored


class database:
    

    def __init__(self):
        self.mydb = mysql.connector.connect(
            host="localhost",
            user="root",
            passwd="root",
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

