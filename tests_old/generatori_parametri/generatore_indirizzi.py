from faker import Faker
import random

class Indirizzo():
    def receive_random_value(self):
        fake = Faker('it_IT')  # Utilizza l'italiano per i dati specifici del contesto

        via = fake.street_name()
        numero_civico = str(random.randint(1, 200))
        cap = fake.postcode()
        citta = fake.city()
        provincia = fake.state()

        indirizzo = via + ", " + numero_civico + ", " + cap  + ", " + citta + ", " + provincia
        indirizzo = indirizzo.replace("'", "")
        return indirizzo