#!/bin/bash

ROOT=$(pwd)
CURRENT_ROOT=""

check_arguments() {
    if [ "$1" = "--kill" ] || [ "$1" = "-k" ]
    then
        pkill main
        exit
    fi
}

check_last_command() {
    if [ $? -ne 0 ]
    then
        printf "\n\n\n!!!!!!!!!! An error occurred while trying to run the script !!!!!!!!!!\n"
        exit
    fi
}

try_cd() {
    cd $1

    check_last_command
}

set_current_root() {
    CURRENT_ROOT=$1

    try_cd "$ROOT/$CURRENT_ROOT"
}

try_main() {
    ./main &

    check_last_command
}

run_main() {
    try_cd $1

    printf "INSIDE $1\n"

    try_main

    printf "DONE $1\n\n"

    try_cd "$ROOT/$CURRENT_ROOT"
}

check_arguments $1

printf "\n\n\n########## Started run script ##########\n\n\n\n"

pkill main

#printf "############## NON FUNCTIONAL REQ ##############\n\n"

#set_current_root src

#run_main non-functional-req/bin

#printf "############## AMMINISTRATIVO ##############\n\n"

#set_current_root src/amministrativo

#run_main handler/bin

#run_main funzioni/accetta_richpren/bin
#run_main funzioni/rifiuta_richpren/bin
#run_main funzioni/aggiungi_indisponibilita_medico/bin
#run_main funzioni/aggiungi_specializzazione_medico/bin
#run_main funzioni/cronologia_richpren/bin
#run_main funzioni/elimina_medico/bin
#run_main funzioni/elimina_paziente/bin

#printf "############## MEDICO ##############\n\n"

#set_current_root src/medico

#run_main handler/bin

#run_main funzioni/cronologia_prestazioni/bin
#run_main funzioni/prestazioni_future/bin
#run_main funzioni/statistiche/bin
#run_main funzioni/termina_prestazione/bin

printf "############## PAZIENTE ##############\n\n"

set_current_root src/paziente

run_main handler/bin

run_main funzioni/cronologia_prenotazioni/bin
run_main funzioni/effettua_prenotazione/bin
run_main funzioni/lascia_feedback/bin
run_main funzioni/ricerca_medico/bin

printf "############## PAZIENTE NON REGISTRATO ##############\n\n"

set_current_root src/paziente_non_registrato

run_main handler/bin

run_main funzioni/registrazione/bin
run_main funzioni/nr_ricerca_medico/bin

printf "\n\n\n########## Run script completed ###########\n\n\n\n"