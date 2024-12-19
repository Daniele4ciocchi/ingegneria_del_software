import socket
import random
from inizializzazione_funzioni import funzioni, apis, porte


HOST = "127.0.0.1"  # Indirizzo del server
random.seed()

errate = []

def generate_random_argument(parameter_class):
    result =  parameter_class().receive_random_value()
    if result == "STOP":
        return "STOP"
    else:
        return result
        
def generate_random_request(funzione):
    request_args = []
    for arg_set in funzioni[funzione]:
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


def send_request(port, request_string):
    try:
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
            s.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
            s.connect((HOST, port))
            s.settimeout(2)  
            
            print(f"Inviando la richiesta: {request_string}")
            s.send(request_string.encode())
            response = s.recv(4096).decode()
            print(f"Risposta ricevuta: {response}")
            return response
    except socket.timeout:
        print("Timeout: il server non ha risposto.")
        return "TIMEOUT"
    except Exception as e:
        print(f"Errore durante l'invio della richiesta: {e}")
        return "ERROR"

def main():
    client_list = list(apis.keys())
    richieste = 0
    # MODIFICARE SOLO QUESTO VALORE PER AUMENTARE IL NUMERO DI TEST PER OGNI FUNZIONE
    test = 2
    succesful = 0
    failed = 0

    # Entriamo una volta in ogni client
    for client in client_list:
        port = porte.get(client)  
        funzioni = apis[client]
        
        print(f'\n------------- Funzioni {client} -------------\n')
        # Entriamo una volta il ogni funzione
        for funzione in funzioni:

            for _ in range(test): 
                request_string = generate_random_request(funzione)
                if request_string.endswith("STOP"):
                    request_string = request_string.split(' ')[0]
                    print(f"Non è  più possibile eseguire la funzione {request_string}, allo stato attuale del database \n")
                    continue
                
                richieste += 1
                response = send_request(port, request_string)
                
                if response.startswith("REQUEST_SUCCESS"):
                    succesful += 1
                else:
                    failed += 1
                    errate.append((request_string, response))

    print(f"\nRichieste avvenute con successo: {succesful}/{richieste}")
    print(f"Richieste fallite: {failed}/{richieste}")
    print("Elenco richieste errate:")
    for funzione in errate:
        print(f'    - {funzione}')

if __name__ == "__main__":
    main()
