import random

RANDOM_SEED = 8
random.seed(RANDOM_SEED)

class NomePersona() :

    nomi = ["Daniele", "Gabriele", "Luca", "Mario", "Luigi", "Giulia", "Sofia", "Andrea", 
            "Marco", "Chiara", "Francesca", "Alessandro", "Federico", "Valentina", "Elena", 
            "Simone", "Matteo", "Giorgia", "Martina", "Serena", "Davide", "Alice"]

    def receive_random_value(self):
        return random.choice(self.nomi)
