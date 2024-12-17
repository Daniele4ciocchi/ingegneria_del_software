import random
import psycopg2

import random
import psycopg2

class IdPaziente:
    def __init__(self):
        self.conn = psycopg2.connect(
            dbname="prenotazionimediche",
            user="tester",
            password="tester",
            host="localhost",
            port="5432"
        )
        self.cursor = self.conn.cursor()

    def receive_random_value(self):
        self.cursor.execute("SELECT p.id FROM paziente p;")
        ids = self.cursor.fetchall()
        if not ids:
            return "STOP"
        return random.choice(ids)[0]
        

    def __del__(self):
        if hasattr(self, 'cursor') and self.cursor:
            self.cursor.close()
        if hasattr(self, 'conn') and self.conn:
            self.conn.close()

class IdPazienteVisitato:
    def __init__(self):
        self.conn = psycopg2.connect(
            dbname="prenotazionimediche",
            user="tester",
            password="tester",
            host="localhost",
            port="5432"
        )
        self.cursor = self.conn.cursor()

    def receive_random_value(self):
        self.cursor.execute("SELECT p.id AS persona, rp.id AS prenotazione FROM paziente p, richiestaprenotazione AS rp, prenotazioneaccettata AS pa WHERE p.id = rp.paziente_id AND rp.id = pa.richiesta_id AND pa.prestazioneavvenuta = 'true';")
        ids = self.cursor.fetchall()
        if not ids:
            return "STOP"
        return random.choice(ids)[0]
        

    def __del__(self):
        if hasattr(self, 'cursor') and self.cursor:
            self.cursor.close()
        if hasattr(self, 'conn') and self.conn:
            self.conn.close()


class IdMedico:
    def __init__(self):
        self.conn = psycopg2.connect(
            dbname="prenotazionimediche",
            user="tester",
            password="tester",
            host="localhost",
            port="5432"
        )
        self.cursor = self.conn.cursor()

    def receive_random_value(self):
        self.cursor.execute("SELECT m.id FROM medico m;")
        ids = self.cursor.fetchall()
        if not ids:
            return "STOP"
        #return random.choice(ids)[0]
        return 1

    def __del__(self):
        if hasattr(self, 'cursor') and self.cursor:
            self.cursor.close()
        if hasattr(self, 'conn') and self.conn:
            self.conn.close()


class IdAmministrativo:
    def __init__(self):
        self.conn = psycopg2.connect(
            dbname="prenotazionimediche",
            user="tester",
            password="tester",
            host="localhost",
            port="5432"
        )
        self.cursor = self.conn.cursor()

    def receive_random_value(self):
        self.cursor.execute("SELECT a.id FROM amministrativo a;")
        ids = self.cursor.fetchall()
        if not ids:
            return "STOP"
        return random.choice(ids)[0]

    def __del__(self):
        if hasattr(self, 'cursor') and self.cursor:
            self.cursor.close()
        if hasattr(self, 'conn') and self.conn:
            self.conn.close()

class IdPrenotazioneAccettataConclusa():
    def __init__(self):
        self.conn = psycopg2.connect(
            dbname="prenotazionimediche",
            user="tester",
            password="tester",
            host="localhost",
            port="5432"
        )
        
        self.cursor = self.conn.cursor()

    def receive_random_value(self):
        self.cursor.execute("SELECT p.richiesta_id FROM prenotazioneaccettata p WHERE p.prestazioneavvenuta = true;")
        ids = self.cursor.fetchall()
        if not ids:
            return "STOP"
        return random.choice(ids)[0]

    def __del__(self):
        if hasattr(self, 'cursor') and self.cursor:
            self.cursor.close()
        if hasattr(self, 'conn') and self.conn:
            self.conn.close()

class IdPrenotazioneAccettataNonConclusa():
    def __init__(self):
        self.conn = psycopg2.connect(
            dbname="prenotazionimediche",
            user="tester",
            password="tester",
            host="localhost",
            port="5432"
        )
        
        self.cursor = self.conn.cursor()

    def receive_random_value(self):
        self.cursor.execute("SELECT p.richiesta_id FROM prenotazioneaccettata p WHERE p.prestazioneavvenuta = false;")
        ids = self.cursor.fetchall()
        if not ids:
            return "STOP"
        return random.choice(ids)[0]

    def __del__(self):
        if hasattr(self, 'cursor') and self.cursor:
            self.cursor.close()
        if hasattr(self, 'conn') and self.conn:
            self.conn.close()