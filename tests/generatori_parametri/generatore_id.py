import random
import psycopg2

dbname="prenotazionimediche",
user="tester",
password="tester",
host="localhost",
port="5432"

class IdPaziente:
    def __init__(self):
        self.conn = psycopg2.connect(dbname, user, password, host, port)
        self.cursor = self.conn.cursor()

    def receive_random_value(self):
        self.cursor.execute("SELECT p.id FROM paziente p;")
        ids = self.cursor.fetchall()
        if not ids:
            raise ValueError("No IDs found in the 'paziente' table.")
        return random.choice(ids)[0]

    def __del__(self):
        self.cursor.close()
        self.conn.close()

class IdMedico:
    def __init__(self):
        self.conn = psycopg2.connect(dbname, user, password, host, port)
        self.cursor = self.conn.cursor()

    def receive_random_value(self):
        self.cursor.execute("SELECT m.id FROM medico m;")
        ids = self.cursor.fetchall()
        if not ids:
            raise ValueError("No IDs found in the 'medico' table.")
        return random.choice(ids)[0]

    def __del__(self):
        self.cursor.close()
        self.conn.close()


class IdAmministrativo:
    def __init__(self):
        self.conn = psycopg2.connect(dbname, user, password, host, port)
        self.cursor = self.conn.cursor()

    def receive_random_value(self):
        self.cursor.execute("SELECT a.id FROM amministrativo a;")
        ids = self.cursor.fetchall()
        if not ids:
            raise ValueError("No IDs found in the 'amministrativo' table.")
        return random.choice(ids)[0]

    def __del__(self):
        self.cursor.close()
        self.conn.close()

class IdPrenotazione_accettata:
    def __init__(self):
        self.conn = psycopg2.connect(dbname, user, password, host, port)
        self.cursor = self.conn.cursor()

    def receive_random_value(self):
        self.cursor.execute("SELECT a.id FROM prenotazione a;")
        ids = self.cursor.fetchall()
        if not ids:
            raise ValueError("No IDs found in the 'amministrativo' table.")
        return random.choice(ids)[0]

    def __del__(self):
        self.cursor.close()
        self.conn.close()

