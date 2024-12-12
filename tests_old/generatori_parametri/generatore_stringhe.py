import random
import string

RANDOM_SEED = 8
random.seed(RANDOM_SEED)

class Stringa():

    def receive_random_value(self):
        
        caratteri = string.ascii_lowercase + string.digits
        i = random.randint(10,20)
        res = "".join(random.choices(caratteri,k = i))

        return res