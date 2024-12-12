import random
import string

class Telefono():

    def receive_random_value(self):
        
        caratteri = string.digits
        res = ""
        res.join(random.choices(caratteri,k = 10))

        return res