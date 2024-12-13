import socket
import random
from inizializzazione_funzioni import requests, apis, ports
import time

time.sleep(0.1) #100ms

HOST = "127.0.0.1"  # Indirizzo del server
random.seed()

# Lista per tracciare richieste errate
errate = []


def generate_random_argument(parameter_class):
    """Genera un valore casuale per un parametro dato."""
    return parameter_class().receive_random_value()


def generate_random_request(client):
    """Genera una richiesta casuale per un client dato."""
    apis_name = random.choice(apis[client])

    request_args = []
    for arg_set in requests[apis_name]:
        arg_values = {}
        for arg_name, arg_class in arg_set:
            arg_values[arg_name] = generate_random_argument(arg_class)
        request_args.append(arg_values)

    request_string = f"{apis_name}"
    for arg_values in request_args:
        for arg_name, arg_value in arg_values.items():
            if isinstance(arg_value, str):
                arg_value = arg_value.replace(" ", "##")
            request_string += f" {arg_name} {arg_value}"

    return request_string.strip()


def send_request(client, port, request_string):
    """Invia una richiesta al server e restituisce la risposta."""
    try:
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
            s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
            s.connect((HOST, port))
            s.settimeout(5)  # Timeout incrementato a 5 secondi

            print(f"Inviando la richiesta: {request_string}")
            s.send(request_string.encode())
            print('post invio')

            response = s.recv(4096).decode()
            print(f"Risposta ricevuta: {response}")
            return response
    except socket.timeout:
        print("Timeout: il server non ha risposto.")
        return "TIMEOUT"
    except Exception as e:
        print(f"Errore durante l'invio della richiesta: {e}")
        return "ERROR"
    finally:
        print("Socket chiuso correttamente.")

def main():
    totale = 2
    richieste = 5
    succesful = 0
    failed = 0

    for _ in range(totale):
        client = random.choice(list(apis.keys()))
        port = ports.get(client)  # Usa la porta configurata o quella di default

        for _ in range(richieste):
            request_string = generate_random_request(client)

            response = send_request(client, port, request_string)
            if response.startswith("REQUEST_SUCCESS"):
                succesful += 1
            else:
                failed += 1
                errate.append(request_string)
                

    print(f"\nSuccesful requests: {succesful}/{totale * richieste}")
    print(f"Failed requests: {failed}/{totale * richieste}")


if __name__ == "__main__":
    main()
