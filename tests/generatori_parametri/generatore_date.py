import random
import datetime

RANDOM_SEED = 8
random.seed(RANDOM_SEED)

current_date = datetime.datetime.now().date()
start_date = datetime.datetime(1900, 1, 1).date()

class Data():
    def receive_random_value(self):
        random_days = random.randint(0, 365)

        return current_date + datetime.timedelta(days=random_days)


class DataNascita():
    def receive_random_value(self):
        # Genera un numero casuale di giorni tra la data di inizio e la data corrente
        random_days = random.randint(0, (current_date - start_date).days)

        # Ritorna una data valida aggiungendo i giorni casuali alla data di inizio
        return start_date + datetime.timedelta(days=random_days)
    
class DataInizio():

    data = ["2025-04-10 03:51", "2025-04-11 10:24", "2025-04-24 11:38", "2025-04-25 11:29", "2025-04-12 07:57", "2025-04-15 08:12", "2025-04-19 10:57", "2025-04-27 17:38",
            "2025-04-30 01:34", "2025-04-20 05:23", "2025-04-26 18:19", "2025-04-03 05:01", "2025-04-16 05:56", "2025-04-07 15:00", "2025-04-09 14:11"]
    
    # data = ['1745533321', '1745308314', '1743863652', '1745520849', '1743970815', '1745480344', '1745518841', '1745114001', '1745909754', '1744525964', '1744048372',
    #         '1745528491', '1745307968', '1745348000', '1744588546', '1744650313', '1744030604', '1744359874', '1745762615', '1743935005']


    def receive_random_value(self):
        return random.choice(self.data)
        
class DataFine():

    data = ["2025-05-10 03:51", "2025-05-11 10:24", "2025-05-24 11:38", "2025-05-25 11:29", "2025-05-12 07:57", "2025-05-15 08:12", "2025-05-19 10:57", "2025-05-27 17:38",
            "2025-05-30 01:34", "2025-05-20 05:23", "2025-05-26 18:19", "2025-05-03 05:01", "2025-05-16 05:56", "2025-05-07 15:00", "2025-05-09 14:11"]
    
    # data = ["1746120000", "1746206400", "1746292800", "1746379200", "1746465600", "1746552000", "1746638400", "1746724800", "1746811200", "1746897600", "1746984000", 
    #         "1747070400", "1747156800", "1747243200", "1747329600", "1747416000", "1747502400", "1747588800", "1747675200", "1747761600"]

    def receive_random_value(self):
        return random.choice(self.data)
    


