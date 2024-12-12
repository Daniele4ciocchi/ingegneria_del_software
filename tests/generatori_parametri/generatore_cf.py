import re
import random
import string

RANDOM_SEED = 8
random.seed(RANDOM_SEED)

class CF():
    def __init__(self):
        # Regex codice fiscale italiano
        self.regex = r'^[A-Z]{6}[0-9]{2}[A-Z]{1}[0-9]{2}[A-Z]{1}[0-9]{3}[A-Z]{1}$'
        pass

    def receive_random_value(self):
        chars_letters = string.ascii_uppercase  
        chars_digits = string.digits  

        while True:
            # Genera un codice fiscale valido secondo il formato
            s = (
                "".join(random.choices(chars_letters, k=6)) +  
                "".join(random.choices(chars_digits, k=2)) +    
                "".join(random.choices(chars_letters, k=1)) +  
                "".join(random.choices(chars_digits, k=2)) +   
                "".join(random.choices(chars_letters, k=1)) +  
                "".join(random.choices(chars_digits, k=3)) +   
                "".join(random.choices(chars_letters, k=1))
                )

            if re.match(self.regex, s):
                return s