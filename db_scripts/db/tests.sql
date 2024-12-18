BEGIN;

-- Inserimento nella tabella persona
INSERT INTO persona (cf, nome, cognome, nascita) VALUES 
('RSSMRA85M01H501Z', 'Mario', 'Rossi', '1985-03-01'),
('BNCLDN85M01H501Y', 'Luigi', 'Bianchi', '1985-07-15'),
('PRSMRA88M01H501X', 'Giulia', 'Pereira', '1988-06-22');

-- Inserimento nella tabella paziente
INSERT INTO paziente (cf, indirizzo, email, telefono) VALUES 
('RSSMRA85M01H501Z', 'Via Roma, 10, 00100, Roma, RM', 'mario.rossi@email.com', '3331234567'),
('BNCLDN85M01H501Y', 'Via Milano, 5, 20100, Milano, MI', 'luigi.bianchi@email.com', '3337654321');

-- Inserimento nella tabella medico
INSERT INTO medico (cf) VALUES 
('PRSMRA88M01H501X');

-- Inserimento nella tabella amministrativo
INSERT INTO amministrativo (cf) VALUES 
('RSSMRA85M01H501Z'),
('BNCLDN85M01H501Y');

-- Inserimento nella tabella specializzazione
INSERT INTO specializzazione (nome, duratavisitah) VALUES 
('Pneumatologia', 2);

-- Inserimento nella tabella medico_specializzazione
INSERT INTO medico_specializzazione (medico_id, specializzazione_nome) VALUES 
(5, 'Pneumatologia'),
(6, 'Pneumatologia'),
(6, 'Cardiologia'),
(6, 'Dermatologia'),
(6, 'Ortopedia'),
(6, 'Pediatria'),
(6, 'Oftalmologia');

-- Inserimento nella tabella indisponibilita
INSERT INTO indisponibilita (medico_id, inizioind, fineind) VALUES 
(6, '2020-12-01 09:00:00', '2020-12-01 12:00:00');

-- Inserimento nella tabella richiestaprenotazione
INSERT INTO richiestaprenotazione (paziente_id, medico_id, amministrativo_id, specializzazione_nome, irich, giornoorariopren) VALUES 
(11, 6, 4, 'Pneumatologia', '2020-11-28 10:00:00', '2020-12-02 10:30:00'),
(11, 6, 4, 'Pneumatologia', '2020-11-28 20:00:00', '2020-12-03 10:30:00'),
(12, 6, 4, 'Pneumatologia', '2000-11-25 20:00:00', '2000-12-03 10:30:00'),
(12, 6, 3, 'Cardiologia', '2020-11-28 11:00:00', '2020-12-25 10:30:00'),
(12, 6, 3, 'Pediatria', '2000-11-28 11:00:00', '2000-12-18 10:30:00');

-- Inserimento nella tabella prenotazioneaccettata

INSERT INTO prenotazioneaccettata (richiesta_id, iaccet, prestazioneavvenuta) VALUES 
(51, '2020-11-30 10:00:00', true), 
(52, '2020-11-29 21:00:00', false),
(53, '2000-11-26 01:00:00', true),
(55, '2000-11-29 01:00:00', true);

-- Inserimento nella tabella motivazione
INSERT INTO motivazione (motivo) VALUES 
('è NATALE!!!');

-- Inserimento nella tabella prenotazionerifiutata
INSERT INTO prenotazionerifiutata (richiesta_id, irif, motivazione_id) VALUES 
(54, '2020-11-29 09:00:00', 3);

-- Inserimento nella tabella feedback
INSERT INTO feedback (paziente_id, prenotazione_accettata_id, ifeed, votosodd, votopunt) VALUES 
(11, 51, '2020-12-02 11:00:00', 4, 5);

COMMIT;
