import random

RANDOM_SEED = 8
random.seed(RANDOM_SEED)

class Specializzazione() :

    specializzazioni = ["Cardiologia", "Dermatologia", "Ortopedia", "Pediatria", "Oftalmologia", "Pneumatologia"]

    def receive_random_value(self):
        return random.choice(self.specializzazioni)
