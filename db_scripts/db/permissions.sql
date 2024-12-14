\c :dbname postgres

GRANT ALL PRIVILEGES ON DATABASE :dbname to :admin;
GRANT CONNECT ON DATABASE :dbname TO :amministrativo;
GRANT CONNECT ON DATABASE :dbname TO :medico;
GRANT CONNECT ON DATABASE :dbname TO :paziente;
GRANT CONNECT ON DATABASE :dbname TO :paziente_non_registrato;

ALTER TABLE persona OWNER TO :admin;
ALTER TABLE paziente OWNER TO :admin;
ALTER TABLE medico OWNER TO :admin;
ALTER TABLE amministrativo OWNER TO :admin;
ALTER TABLE specializzazione OWNER TO :admin;
ALTER TABLE medico_specializzazione OWNER TO :admin;
ALTER TABLE indisponibilita OWNER TO :admin;
ALTER TABLE richiestaprenotazione OWNER TO :admin;
ALTER TABLE prenotazioneaccettata OWNER TO :admin;
ALTER TABLE prenotazionerifiutata OWNER TO :admin;
ALTER TABLE motivazione OWNER TO :admin;
ALTER TABLE feedback OWNER TO :admin;

GRANT ALL ON SCHEMA public TO :admin;
GRANT ALL PRIVILEGES ON ALL TABLES IN SCHEMA public TO :admin;

GRANT USAGE, SELECT ON ALL SEQUENCES IN SCHEMA public TO :admin;
GRANT USAGE, SELECT ON ALL SEQUENCES IN SCHEMA public TO :amministrativo;
GRANT USAGE, SELECT ON ALL SEQUENCES IN SCHEMA public TO :medico;
GRANT USAGE, SELECT ON ALL SEQUENCES IN SCHEMA public TO :paziente;
GRANT USAGE, SELECT ON ALL SEQUENCES IN SCHEMA public TO :paziente_non_registrato;

-- Permessi amministrativo
GRANT INSERT, UPDATE, DELETE ON  persona TO :amministrativo;
GRANT INSERT, UPDATE, DELETE ON  medico TO :amministrativo;
GRANT INSERT, UPDATE, DELETE ON  paziente TO :amministrativo;
GRANT INSERT, UPDATE, DELETE ON  indisponibilita TO :amministrativo;
GRANT INSERT, SELECT ON prenotazioneaccettata TO :amministrativo;
GRANT INSERT, SELECT ON prenotazionerifiutata TO :amministrativo;
GRANT SELECT ON  richiestaprenotazione TO :amministrativo;

-- Permessi medico
GRANT SELECT ON richiestaprenotazione TO :medico;
GRANT SELECT, UPDATE ON prenotazioneaccettata TO :amministrativo;
GRANT SELECT ON prenotazionerifiutata TO :amministrativo;
GRANT SELECT ON feedback TO :medico;

-- Permessi paziente_non_registrato
GRANT INSERT, UPDATE, SELECT ON persona TO :paziente_non_registrato;
GRANT SELECT ON medico_specializzazione TO :paziente_non_registrato;
GRANT INSERT, UPDATE ON paziente TO :paziente_non_registrato;
GRANT SELECT ON medico TO :paziente_non_registrato;

-- Permessi paziente
GRANT SELECT ON medico TO :paziente;
GRANT SELECT ON persona TO :paziente;
GRANT SELECT ON medico_specializzazione TO :paziente;
GRANT INSERT, SELECT ON richiestaprenotazione TO :paziente;
GRANT SELECT ON prenotazioneaccettata TO :paziente;
GRANT SELECT ON prenotazionerifiutata TO :paziente;
GRANT INSERT ON feedback TO :paziente;

-- Permessi tester
GRANT SELECT ON medico TO :tester;
GRANT SELECT ON paziente TO :tester;
GRANT SELECT ON amministrativo TO :tester;