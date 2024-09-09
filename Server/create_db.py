import sqlite3


with sqlite3.connect("clients.db") as con:
    cur = con.cursor()
    cur.execute("""
    CREATE TABLE clients(
        client_name varchar(255)  PRIMARY KEY NOT NULL,
        client_id varchar(32) NOT NULL
    )
    """, ())
    con.commit()
