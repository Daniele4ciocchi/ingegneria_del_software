#!/bin/bash

# Directory di esecuzione iniziale
ROOT=$(pwd)
# Directory corrente (viene aggiornata dinaicamente)
CURRENT_ROOT=""

CLEAN=false

# Funzione che controlla se viene passato l'argomento --clean
check_arguments() {
    if [ "$1" = "--clean" ] || [ "$1" = "-c" ]
    then
        CLEAN=true
    fi
}

# Funzione per exit code ultimo comando, se !=0 printa un errore
check_last_command() {
    if [ $? -ne 0 ]
    then
        printf "\n\n\n!!!!!!!!!! An error occurred while trying to run the script !!!!!!!!!!\n"
        exit 
    fi
}

# Funzione per cambio directory
try_cd() {
    cd $1

    check_last_command
}

# Funzione per aggiornare la directory corrente
set_current_root() {
    CURRENT_ROOT=$1

    try_cd "$ROOT/$CURRENT_ROOT"
}

# Funzione che esegue make clean o make in base al valore di CLEAN
try_make() {
    if $CLEAN = true
    then
        make clean
    else
        make
    fi

    check_last_command
}

# Funzione che con parametro di una directory vi entra, esegue try_make() ed esce
make_folder() {
    try_cd $1

    printf "INSIDE $1\n"

    try_make

    printf "DONE $1\n\n"

    try_cd "$ROOT/$CURRENT_ROOT"
}

# Controlla se è stata passata l'opzione --clean o -c
check_arguments $1

# INIZIO SCRIPT PRINCIPALE
printf "\n\n\n########## Started make script ##########\n\n\n\n"

printf "############## LIB ##############\n\n"

set_current_root src/lib

make_folder con2db
make_folder con2redis/src

printf "############## UTILS ##############\n\n"

set_current_root src

make_folder utils/src

printf "############## CLASSES ##############\n\n"

set_current_root src

make_folder classes/src

printf "############## SERVER ##############\n\n"

set_current_root src

make_folder server/src

#printf "############## NON FUNCTIONAL REQ ##############\n\n"

#set_current_root src

#make_folder non-functional-req/src

#printf "############## AMMINISTRATIVO ##############\n\n"

#set_current_root src/amministrativo

#make_folder handler/src

#make_folder funzioni/accetta_richpren/src
#make_folder funzioni/rifiuta_richpren/src
#make_folder funzioni/aggiungi_indisponibilita_medico/src
#make_folder funzioni/aggiungi_specializzazione_medico/src
#make_folder funzioni/cronologia_richpren/src
#make_folder funzioni/elimina_medico/src
#make_folder funzioni/elimina_paziente/src

#printf "############## MEDICO ##############\n\n"

#set_current_root src/medico

#make_folder handler/src

#make_folder funzioni/cronologia_prestazioni/src
#make_folder funzioni/prestazioni_future/src
#make_folder funzioni/statistiche/src
#make_folder funzioni/termina_prestazione/src

printf "############## PAZIENTE ##############\n\n"

set_current_root src/paziente

make_folder handler/src

make_folder funzioni/cronologia_prenotazioni/src
make_folder funzioni/effettua_prenotazione/src
make_folder funzioni/lascia_feedback/src
make_folder funzioni/ricerca_medico/src

printf "############## PAZIENTE NON REGISTRATO ##############\n\n"

set_current_root src/paziente_non_registrato

make_folder handler/src

make_folder funzioni/registrazione/src
#make_folder funzioni/ricerca_non_registrata/src

printf "\n\n\n########## Make script completed ###########\n\n\n\n"