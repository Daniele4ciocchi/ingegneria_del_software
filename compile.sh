#!/bin/bash

# Controllo risultato ultimo comando eseguito
check_last_command() {
    if [ $? -ne 0 ]
    then
        printf "\n\n\n!!!!!!!!!! An error occurred while trying to run the script !!!!!!!!!!\n"
        exit
    fi
}

# Funzione per escuzione script shell (con argomento opzionale)
try_sh() {
    sh $1 $2

    check_last_command
}

# Funzione per cambio directory
try_cd() {
    cd $1

    check_last_command
}

# Script principale
try_cd db-scripts
try_sh create.sh
try_cd ..
try_sh makes.sh --clean
try_sh makes.sh