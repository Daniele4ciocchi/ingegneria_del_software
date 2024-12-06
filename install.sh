#!/bin/bash

# Pacchetti necessari per il progetto
sudo apt-get install make
sudo apt-get install g++
sudo apt-get install postgresql
sudo apt-get install redis
sudo apt-get install libhiredis-dev
sudo apt install libpq-dev

# così scritto aggiunge postgres al gruppo dell'utente che esegue il comando
sudo usermod -aG $USER postgres