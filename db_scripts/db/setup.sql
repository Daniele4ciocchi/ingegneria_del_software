-- Creiamo il database :dbname
DROP DATABASE IF EXISTS :dbname;
CREATE DATABASE :dbname;

-- Creiamo l'utente :admin
\c :dbname postgres

REASSIGN OWNED BY :admin TO postgres;

-- Creiamo l'utente amministrativo
\c :dbname postgres

REASSIGN OWNED BY :amministrativo TO postgres;

REVOKE ALL PRIVILEGES ON ALL TABLES IN SCHEMA public FROM :amministrativo;
REVOKE ALL PRIVILEGES ON ALL SEQUENCES IN SCHEMA public FROM :amministrativo;
REVOKE ALL PRIVILEGES ON ALL FUNCTIONS IN SCHEMA public FROM :amministrativo;

DROP OWNED BY :amministrativo;
DROP USER IF EXISTS :amministrativo;

CREATE USER :amministrativo WITH ENCRYPTED PASSWORD 'amministrativo';

-- Creiamo l'utente medico
\c :dbname postgres

REASSIGN OWNED BY :medico TO postgres;

REVOKE ALL PRIVILEGES ON ALL TABLES IN SCHEMA public FROM :medico;
REVOKE ALL PRIVILEGES ON ALL SEQUENCES IN SCHEMA public FROM :medico;
REVOKE ALL PRIVILEGES ON ALL FUNCTIONS IN SCHEMA public FROM :medico;

DROP OWNED BY :medico;
DROP USER IF EXISTS :medico;

CREATE USER :medico WITH ENCRYPTED PASSWORD 'medico';

-- Creiamo l'utente paziente_non_registrato
\c :dbname postgres

REASSIGN OWNED BY :paziente_non_registrato TO postgres;

REVOKE ALL PRIVILEGES ON ALL TABLES IN SCHEMA public FROM :paziente_non_registrato;
REVOKE ALL PRIVILEGES ON ALL SEQUENCES IN SCHEMA public FROM :paziente_non_registrato;
REVOKE ALL PRIVILEGES ON ALL FUNCTIONS IN SCHEMA public FROM :paziente_non_registrato;

DROP OWNED BY :paziente_non_registrato;
DROP USER IF EXISTS :paziente_non_registrato;

CREATE USER :paziente_non_registrato WITH ENCRYPTED PASSWORD 'paziente_non_registrato';

-- Creiamo l'utente paziente
\c :dbname postgres

REASSIGN OWNED BY :paziente TO postgres;

REVOKE ALL PRIVILEGES ON ALL TABLES IN SCHEMA public FROM :paziente;
REVOKE ALL PRIVILEGES ON ALL SEQUENCES IN SCHEMA public FROM :paziente;
REVOKE ALL PRIVILEGES ON ALL FUNCTIONS IN SCHEMA public FROM :paziente;

DROP OWNED BY :paziente;
DROP USER IF EXISTS :paziente;

CREATE USER :paziente WITH ENCRYPTED PASSWORD 'paziente';