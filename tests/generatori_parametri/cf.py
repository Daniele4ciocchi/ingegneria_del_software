from .param_interface import ParamInterface
from .allowed_chars import ALLOWED_CHARS

import re
import random
import string

RANDOM_SEED = 7
random.seed(RANDOM_SEED)

class AlphaNumeric(ParamInterface):
    def __init__(self):
        # Regex codice fiscale italiano
        self.regex = r'^[A-Z]{6}[0-9]{2}[A-Z]{1}[0-9]{2}[A-Z]{1}[0-9]{3}[A-Z]{1}$'
        pass

    def right(self):
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

    def wrong(self):
        chars = string.ascii_letters + string.digits + ALLOWED_CHARS

        while True:
            s = "".join(random.choices(chars, k=random.randint(1, 50)))
            if not re.match(self.regex, s):
                return s
