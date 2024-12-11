from .param_interface import ParamInterface

import random
import string

RANDOM_SEED = 7
random.seed(RANDOM_SEED)

class StringNC(ParamInterface):
    def __init__(self):
        self.chars = string.ascii_letters
        pass

    def right(self):
        return "".join(random.choices(self.chars, k = random.randint(4, 18)))

    def wrong(self):
        return "".join(random.choices(self.chars, k = random.randint(100, 256)))