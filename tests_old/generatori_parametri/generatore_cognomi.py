import random

RANDOM_SEED = 8
random.seed(RANDOM_SEED)

class CognomePersona() :

    cognomi = ["Quattrociocchi", "Sepe", "Mastromattei", "Rossi", "Bianchi", "Esposito", "Russo",
                "Romano", "Ferrari", "Marino", "Ricci", "Bruno", "Gallo", "Conti", "De Luca", 
                "Mancini", "Costa", "Greco", "Giordano", "Rizzo", "Lombardi", "Moretti", "Barbieri"]

    def receive_random_value(self):
        return random.choice(self.cognomi)