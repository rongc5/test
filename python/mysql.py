'''
#=============================================================================
#     FileName: mysql.py
#         Desc:
#       Author: zhangming
#        Email:
#     HomePage:
#      Version: 1.0.0
#   LastChange: 2013-11-22 01:35:54
#      History:
#=============================================================================
'''

import MySQLdb


class Database:

    host = 'localhost'
    user = 'root'
    password = '123456'
    db = 'test'

    def __init__(self):
        self.connection = MySQLdb.connect(self.host, self.user, self.password, self.db)
        self.cursor = self.connection.cursor()

    def insert(self, query):
        try:
            self.cursor.execute(query)
            self.connection.commit()
        except:
            self.connection.rollback()



    def query(self, query):
        cursor = self.connection.cursor( MySQLdb.cursors.DictCursor )
        cursor.execute(query)

        return cursor.fetchall()

    def __del__(self):
        self.connection.close()


if __name__ == "__main__":

    db = Database()

    #CleanUp Operation
    #del_query = "DELETE FROM basic_python_database"
    #db.insert(del_query)

    # Data Insert into the table
    #query = """
        #INSERT INTO basic_python_database
        #(`name`, `age`)
        #VALUES
        #('Mike', 21),
        #('Michael', 21),
        #('Imran', 21)
        #"""

    # db.query(query)
    #db.insert(query)

    # Data retrieved from the table
    #select_query = """
        #SELECT * FROM basic_python_database
        #WHERE age = 21
        #"""

    str = "select * from epg_channle;"
    people = db.query(str)

    for person in people:
        print "Found %s " % person
