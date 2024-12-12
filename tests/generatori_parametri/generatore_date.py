import random
import datetime

RANDOM_SEED = 8
random.seed(RANDOM_SEED)

current_date = datetime.datetime.now().date()
start_date = datetime.datetime(1900, 1, 1).date()
end_date = datetime.datetime.now().date()

class Data():
    def receice_random_value(self):
        random_days = random.randint(0, 365)

        return current_date + datetime.timedelta(days=random_days)


class DataNascita():
    def receive_random_value(self):
        # Genera un numero casuale di giorni tra la data di inizio e la data corrente
        random_days = random.randint(0, (end_date - start_date).days)

        # Ritorna una data valida aggiungendo i giorni casuali alla data di inizio
        return start_date + datetime.timedelta(days=random_days)

