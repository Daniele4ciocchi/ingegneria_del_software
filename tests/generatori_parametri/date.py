from .param_interface import ParamInterface
from .allowed_chars import ALLOWED_CHARS


import random
import datetime
import string
import re

RANDOM_SEED = 7
random.seed(RANDOM_SEED)

current_date = datetime.datetime.now().date()
start_date = datetime.datetime(1900, 1, 1).date()
end_date = datetime.datetime.now().date()

class Date(ParamInterface):
    def __init__(self):
        self.regex = r'\d{4}-\d{2}-\d{2}'
        pass

    def right(self):
        random_days = random.randint(0, 365)

        return current_date + datetime.timedelta(days=random_days)

    def wrong(self):
        chars = string.ascii_letters + string.digits + ALLOWED_CHARS
        while True:
            s = "".join(random.choices(chars, k=random.randint(1, 50)))
            if not re.match(self.regex, s):
                return s
            

class BornDate(ParamInterface):
    def __init__(self):
        # Espressione regolare per validare una data nel formato YYYY-MM-DD
        self.regex = r'\d{4}-\d{2}-\d{2}'
        pass

    def right(self):
        # Genera un numero casuale di giorni tra la data di inizio e la data corrente
        random_days = random.randint(0, (end_date - start_date).days)

        # Ritorna una data valida aggiungendo i giorni casuali alla data di inizio
        return start_date + datetime.timedelta(days=random_days)

    def wrong(self):
        # Genera una stringa casuale di lunghezza variabile (1-50 caratteri) che non corrisponde a una data
        chars = string.ascii_letters + string.digits + ALLOWED_CHARS
        while True:
            s = "".join(random.choices(chars, k=random.randint(1, 50)))
            if not re.match(self.regex, s):
                return s
