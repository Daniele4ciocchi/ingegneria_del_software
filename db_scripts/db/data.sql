BEGIN;

-- Inserimento persone (anagrafiche comuni a pazienti, medici, amministrativi)
INSERT INTO persona (cf, nome, cognome, nascita) VALUES
('ABCDEF12G34H567I', 'Mario', 'Rossi', '1975-03-15'),
('BCDEFG23H45I678J', 'Luca', 'Verdi', '1980-07-22'),
('CDEFGH34I56J789K', 'Giulia', 'Bianchi', '1988-11-05'),
('DEFGHI45J67K890L', 'Francesca', 'Neri', '1990-02-17'),
('EFGHIJ56K78L901M', 'Giovanni', 'Esposito', '1983-09-11'),
('FGHIJK67L89M012N', 'Maria', 'Conte', '1978-05-10'),
('GHIJKL78M90N123O', 'Sofia', 'Ricci', '1992-06-29'),
('HIJKLM89N01O234P', 'Antonio', 'De Luca', '1987-01-14'),
('IJKLMN90O12P345Q', 'Chiara', 'Ferrari', '1995-04-18'),
('JKLMNO01P23Q456R', 'Paolo', 'Gallo', '1976-12-03'),
('KLMNOP12Q34R567S', 'Stefano', 'Romano', '1982-08-20'),
('LMNOPQ23R45S678T', 'Alessia', 'Marino', '1990-10-11'),
('MNOPQR34S56T789U', 'Elena', 'Greco', '1985-03-25'),
('NOPQRS45T67U890V', 'Luigi', 'Moretti', '1979-09-13'),
('CFADMIN001ADMIN0', 'Mario', 'Rossi', '1980-05-12'),
('CFADMIN002ADMIN0', 'Luisa', 'Bianchi', '1985-07-24'),
('CFADMIN003ADMIN0', 'Giulia', 'Verdi', '1990-11-03'),
('OPQRST56U78V901W', 'Silvia', 'Gatti', '1993-07-19');

-- Inserimento specializzazioni
INSERT INTO specializzazione (nome, duratavisitah) VALUES
('Cardiologia', 1),
('Dermatologia', 1),
('Ortopedia', 1),
('Pediatria', 1),
('Oftalmologia', 1);

-- Popolamento pazienti
INSERT INTO paziente (cf, indirizzo, email, telefono) VALUES
('ABCDEF12G34H567I', 'Via Roma, 10, 00100, Roma, RM', 'mario.rossi@example.com', '3401234567'),
('BCDEFG23H45I678J', 'Corso Italia, 15, 10100, Torino, TO', 'luca.verdi@example.com', '3456789012'),
('CDEFGH34I56J789K', 'Piazza Garibaldi, 5, 80100, Napoli, NA', 'giulia.bianchi@example.com', '3312345678'),
('DEFGHI45J67K890L', 'Via Milano, 20, 20100, Milano, MI', 'francesca.neri@example.com', '3209876543'),
('EFGHIJ56K78L901M', 'Viale Venezia, 8, 30100, Venezia, VE', 'giovanni.esposito@example.com', '3276543210'),
('FGHIJK67L89M012N', 'Via Firenze, 12, 50100, Firenze, FI', 'maria.conte@example.com', '3298765432'),
('GHIJKL78M90N123O', 'Via Napoli, 3, 70100, Bari, BA', 'sofia.ricci@example.com', '3345678901'),
('HIJKLM89N01O234P', 'Via Bologna, 9, 40100, Bologna, BO', 'antonio.deluca@example.com', '3309876543'),
('IJKLMN90O12P345Q', 'Via Palermo, 6, 90100, Palermo, PA', 'chiara.ferrari@example.com', '3391234567'),
('JKLMNO01P23Q456R', 'Via Cagliari, 7, 09100, Cagliari, CA', 'paolo.gallo@example.com', '3323456789');

-- Popolamento medici
INSERT INTO medico (cf) VALUES
('KLMNOP12Q34R567S'),
('LMNOPQ23R45S678T'),
('MNOPQR34S56T789U'),
('NOPQRS45T67U890V'),
('OPQRST56U78V901W');

-- Inserimento degli amministrativi
INSERT INTO amministrativo (cf) VALUES
('CFADMIN001ADMIN0'),
('CFADMIN002ADMIN0'),
('CFADMIN003ADMIN0');

-- Associazione medici a specializzazioni
INSERT INTO medico_specializzazione (medico_id, specializzazione_nome) VALUES
(1, 'Cardiologia'),
(1, 'Dermatologia'),
(2, 'Dermatologia'),
(3, 'Ortopedia'),
(4, 'Pediatria'),
(5, 'Oftalmologia');

COMMIT;

-- Popolamento indisponibilità
INSERT INTO indisponibilita (medico_id, inizioind, fineind) VALUES
(1, '2024-11-20 08:00', '2024-11-20 09:00'),
(2, '2024-11-21 09:00', '2024-11-21 10:00'),
(3, '2024-11-22 10:00', '2024-11-22 11:00'),
(4, '2024-11-23 11:00', '2024-11-23 12:00'),
(5, '2024-11-24 12:00', '2024-11-24 13:00');

-- Popolamento richieste prenotazione
INSERT INTO richiestaprenotazione (paziente_id, medico_id, amministrativo_id, specializzazione_nome, irich, giornoorariopren) VALUES
(1, 1, 1, 'Cardiologia', '2024-11-20 09:00', '2024-11-25 09:00'),
(2, 2, 1, 'Dermatologia', '2024-11-20 10:00', '2024-11-25 10:00'),
(3, 3, 1, 'Ortopedia', '2024-11-20 11:00', '2024-11-25 11:00'),
(4, 4, 1, 'Pediatria', '2024-11-20 12:00', '2024-11-25 12:00'),
(5, 5, 1, 'Oftalmologia', '2024-11-20 13:00', '2024-11-25 13:00'),
(6, 1, 1, 'Cardiologia', '2024-11-21 09:00', '2024-11-26 09:00'),
(7, 2, 1, 'Dermatologia', '2024-11-21 10:00', '2024-11-26 10:00'),
(8, 3, 1, 'Ortopedia', '2024-11-21 11:00', '2024-11-26 11:00'),
(9, 4, 1, 'Pediatria', '2024-11-21 12:00', '2024-11-26 12:00'),
(10, 5, 1, 'Oftalmologia', '2024-11-21 13:00', '2024-11-26 13:00');

-- Popolamento prenotazioni accettate
INSERT INTO prenotazioneaccettata (richiesta_id, iaccet, prestazioneavvenuta) VALUES
(1, '2024-11-20 09:30', false),
(2, '2024-11-20 10:30', true),
(3, '2024-11-20 11:30', false),
(4, '2024-11-20 12:30', true),
(5, '2024-11-20 13:30', false);

-- Popolamento motivazioni
INSERT INTO motivazione (motivo) VALUES
('Medico non disponibile'),
('Specializzazione non disponibile');

-- Popolamento prenotazioni rifiutate
INSERT INTO prenotazionerifiutata (richiesta_id, irif, motivazione_id) VALUES
(6, '2024-11-21 09:30', 1),
(7, '2024-11-21 10:30', 2),
(8, '2024-11-21 11:30', 1),
(9, '2024-11-21 12:30', 2),
(10, '2024-11-21 13:30', 1);

-- Popolazione feedback
INSERT INTO feedback (paziente_id, prenotazione_accettata_id, ifeed, votosodd, votopunt) VALUES
(2, 2, '2024-12-20 10:30', 5, 4),
(4, 4, '2024-12-20 12:30', 5, 5);