import random

RANDOM_SEED = 8
random.seed(RANDOM_SEED)

class Voto():

    def receive_random_value(self):
        return random.randint(1, 5)