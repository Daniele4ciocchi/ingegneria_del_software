import socket
import random
import string
from inizializzazione_funzioni import requests, apis, ports


errate = []
HOST = "127.0.0.1"  # L'indirizzo IP del server
PORT = 42072  # La porta del server

random.seed()
# Richiesta esempio : nome-funzione key1 value1 ... keyN valueN
request_string = "check-order order_id 2" 
#request_string = "take-delivery orderId 2 courier 1 status shipped"

def generate_random_argument(parameter_class):
    return parameter_class().receive_random_value()

def generate_random_request(client):
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
            if type(arg_value) == string:
                arg_value.replace(" ", "##")              # è giusto così lo spacing ?
            request_string += f" {arg_name} {arg_value}"
    
    return request_string.strip()


if __name__ == "__main__":
    totale = 2
    richieste = 2
    succesful = 0
    failed = 0
    
    for _ in range(totale):
        
        client = random.choice(list(apis.keys()))
        print(client, type(client))
        #client = "paziente_non_registrato"
        port = ports[client]
        # due righe successive poi da togliere
        port = 42072
        client = "paziente_non_registrato"
        
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
            s.connect((HOST, port))
            s.settimeout(5)  
  
            for _ in range(richieste):
                request_string = generate_random_request(client)

                print(f"Inviando la richiesta: {request_string}")
    
                s.send(request_string.encode()) # Invia la richiesta al server
                print("post invio")
                
                # ABBIAMO UN PROBLEMA QUI SI BLOCCA PERCHE' IL SERVERNON DA RISPOSTA
                try:
                    response = s.recv(4096).decode() # Ricevi la risposta dal server
                    print(f"Risposta ricevuta: {response}")
                    if response.startswith("BAD_REQUEST") or response.startswith("DB_ERROR") or response.startswith("Timeout"):
                        failed += 1
                        errate.append(request_string)
                    else:
                        succesful += 1
                except socket.timeout:
                    print("Timeout: il server non ha risposto.\n")
                    failed += 1
                except ConnectionResetError:
                    print("Errore:connessione chiusa") 
                    
    
                
                            
    print(f"\n succesful requests: {succesful}/{totale*richieste} \n failed requests: {failed}/{totale*richieste} \n\n")
    
