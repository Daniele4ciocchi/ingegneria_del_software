import random
import string

RANDOM_SEED = 8
random.seed(RANDOM_SEED)

class Telefono():

    def receive_random_value(self):
        
        caratteri = string.digits
        res = "".join(random.choices(caratteri,k = 10))

        return res