from generatori_parametri.generatore_id import IdPaziente, IdMedico

# Create an instance of IdPaziente
id_paziente = IdMedico()

# Get the random value
random_value = id_paziente.receive_random_value()

# Print the random value
print(f"Random Value: {random_value}")