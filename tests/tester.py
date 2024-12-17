import socket
import random
from inizializzazione_funzioni import requests, apis, ports


HOST = "127.0.0.1"  # Indirizzo del server
random.seed()

# Lista per tracciare richieste errate
errate = []


def generate_random_argument(parameter_class):
    """Genera un valore casuale per un parametro dato."""
    return parameter_class().receive_random_value()


def generate_random_request(funzione):

    request_args = []
    for arg_set in requests[funzione]:
        arg_values = {}
        for arg_name, arg_class in arg_set:
            arg_values[arg_name] = generate_random_argument(arg_class)
        request_args.append(arg_values)

    request_string = f"{funzione}"
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
            s.settimeout(2)  # Timeout incrementato a 5 secondi

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
    client_list = list(apis.keys())
    richieste = 0
    # MODIFICARE SOLO QUESTO VALORE PER AUMENTARE IL NUMERO DI TEST PER OGNI FUNZIONE
    test = 5
    succesful = 0
    failed = 0

    # Entriamo una volta in ogni client
    for client in client_list:
        port = ports.get(client)  # Usa la porta configurata o quella di default

        funzioni = apis[client]

        # Entriamo una volta il ogni funzione
        for funzione in funzioni:

            for _ in range(test): 

                request_string = generate_random_request(funzione)
                richieste += 1

                response = send_request(client_list, port, request_string)
                if response.startswith("REQUEST_SUCCESS"):
                    succesful += 1
                else:
                    failed += 1
                    errate.append(request_string)
                    errate.append("\n")
                

    print(f"\nSuccesful requests: {succesful}/{richieste}")
    print(f"Failed requests: {failed}/{richieste}")
    print(errate)


if __name__ == "__main__":
    main()
