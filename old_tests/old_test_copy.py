from requests_init import requests, apis, ports
from generatori_parametri.wrong_arg import WrongArg

import socket
import random

HOST = "127.0.0.1"  # The server's hostname or IP address
RANDOM_SEED = 8

random.seed(RANDOM_SEED)

#set a seed for random
"""
Test generator
    base structure of a request: Type-of-request key1-arg value1-arg ..... keyn-arg valuen-arg

        Type-of-request should be in an enum/set/list
        key, value are separated by spaces, if exists a space in value it cointain the next key, same but different for key
        spaces in key,value are ##
"""

if __name__ == "__main__":
    # Lista di tuple (tipoRichiesta,numRichiesteDaEseguire)
    counter = [(k, 20) for k in requests.keys()]
    # Numero richieste totali da eseguire
    num_requests = sum([v for _, v in counter])
    test = 1
    correct = unknow = wrong = 0
    to_save = []
    wrong_arg_key = WrongArg()

    while counter:
        # Prendiamo una elemento di counter casuale e decrementiamo subito il numero di richieste di quel tipo da fare.
        i = random.randint(0, len(counter) -1)
        k, v = counter[i]
        counter[i] = (k, v-1)
        # Se abbiamo finito le richieste di quel tipo togliamo la tupla dalla lista
        if v == 1:
            counter.pop(i)
        
        #  Numero totale di coppie chiave-valore generate casualmente, queste sono gli argomenti per la richiesta. Sono generati più argomenti del necessario (con duplicati o errati).
        args = (len(requests[k]) * 2) + random.randint(0, 2) * 2

        # Suddivisione in numero di argomenti validi e numero di argomenti non validi
        true_args =  random.randint(0, min(len(requests[k]), args))
        false_args = args - true_args

        ta_cpy = true_args
        fa_cpy = false_args

        # True solamente se tutti gli argomenti sono validi
        true_response = True if false_args <= 0 else False

        # Tipo k della richiesta iniziale
        req = k
        # Indici degli argomenti validi che possono essere inseriti nella richiesta k
        to_insert = len(requests[k])

    
        
        for j in range(len(requests[k])):
            # Se il numero casuale è maggiore di false_args, viene aggiunto un argomento valido
            if random.randint(1, 1) == 1:
                # Chiave valida presa direttamente dalla richiesta k
                req += " " + requests[k][j][0][0]
                # Valore valido preso dalla funzione che genera valori corretti all'interno della classe del tipo
                req += " " + str(requests[k][j][0][1]().right()).replace(" ", "##")
                true_args -= 1
            else:
                # Chiave generata errata
                req += " " + wrong_arg_key.right().replace(" ", "##")
                # Valore errato preso dalla funzione che genera valori corretti all'interno della classe del tipo
                req += " " + str(requests[k][j][0][1]().wrong())
                false_args -= 1
        
        req = k
        req += " "+"cf"+" "+ "SPEGRL03D08E472M"+" "+"nome"+" "+"gabriele"+" "+"cognome"+" "+"sepe"+" "+" nascita"+" "+"2003/04/08"


        #handler, port = ports[random.randint(0, 3)]
        handler, port = ports[3]

        response = ""
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
            s.connect((HOST, port))
            print(f"TEST[{test}/{num_requests}]")
            print(f"Sending to {handler} a {k} request")

            s.send(req.encode())

            response = s.recv(2048).decode()

        if k not in apis[handler]:
            if not response.startswith("BAD_REQUEST"):
                print(f"WRONG:\n\tExpected: BAD_REQUEST\n\tRecived: {response}")
                wrong += 1
                to_save.append(("1", k, handler, req, response))
            else:
                print("CORRECT")
                correct += 1
        else:
            if true_response:
                print(f"Expected unknow\n\tResponse: {response}")
                unknow += 1
            else:
                if not response.startswith("BAD_REQUEST") and not response.startswith("DB_ERROR"):
                    print(f"WRONG:\n\tExpected: BAD_REQUEST | DB_ERROR\n\tRecived: {response}")
                    wrong += 1
                    to_save.append(("2", k, handler, req, response, ta_cpy, fa_cpy))
                else:
                    print("CORRECT")
                    correct += 1
        print()
        test += 1
    
    print(f"Runned {num_requests} tests:")
    print(f"\tCorrect {correct} / {num_requests} tests")
    print(f"\tUnknow {unknow} / {num_requests} tests")
    print(f"\tWrong {wrong} / {num_requests} tests")

    for elem in to_save:
        print(elem)