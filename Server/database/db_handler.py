import sqlite3


class DBHandler:
    dbname = "clients.db"

    def _execute_select_query(self, query: str, params: tuple) -> list:
        with sqlite3.connect(self.dbname) as con:
            cur = con.cursor()
            cur.execute(query, params)
            return cur.fetchall()

    def get_client_id(self, name) -> str:
        matching = self._execute_select_query("SELECT client_id FROM clients WHERE client_name=?", (name,))
        if len(matching) == 0:
            raise ValueError(f"Client name '{name}' doesn't exist in the clients table")
        return matching[0][0]
