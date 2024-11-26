\c :dbname 

-- Elimina tutti i triggers e tutte le funzioni
DO $$
DECLARE
    function_name TEXT;
BEGIN
    FOR function_name IN
        SELECT p.proname
        FROM pg_proc p
        INNER JOIN pg_namespace n ON p.pronamespace = n.oid
        WHERE n.nspname = 'public'
    LOOP
        EXECUTE 'DROP FUNCTION IF EXISTS ' || function_name || ' CASCADE';
    END LOOP;
END $$;


-- [V.persona.completezza]
CREATE OR REPLACE FUNCTION verifica_persona_completezza_deferred()
RETURNS TRIGGER AS $$
BEGIN
    PERFORM 1
    FROM persona p
    WHERE NOT EXISTS (
        SELECT 1 FROM paziente WHERE cf = p.cf
    ) AND NOT EXISTS (
        SELECT 1 FROM medico WHERE cf = p.cf
    ) AND NOT EXISTS (
        SELECT 1 FROM amministrativo WHERE cf = p.cf
    );

    IF FOUND THEN
        RAISE EXCEPTION 'Violazione del vincolo: Ogni persona deve essere paziente, medico o amministrativo';
    END IF;

    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE CONSTRAINT TRIGGER trg_verifica_persona_completezza
AFTER INSERT OR UPDATE ON persona
DEFERRABLE INITIALLY DEFERRED
FOR EACH ROW
EXECUTE FUNCTION verifica_persona_completezza_deferred();


-- [V.medico.almenounaspecializzazione]
CREATE OR REPLACE FUNCTION medico_specializzato()
RETURNS TRIGGER AS $$
BEGIN
    IF NOT EXISTS (
        SELECT 1 
        FROM medico_specializzazione 
        WHERE medico_id = NEW.id
    ) THEN
        RAISE EXCEPTION 'Violazione del vincolo: Ogni medico deve avere almeno una specializzazione.';
    END IF;
    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE CONSTRAINT TRIGGER medico_almeno_una_specializzazione 
AFTER INSERT OR UPDATE ON medico
DEFERRABLE INITIALLY DEFERRED
FOR EACH ROW
EXECUTE FUNCTION medico_specializzato();


-- [V.richiestaprenotazione.disgiunzione]
CREATE OR REPLACE FUNCTION verifica_disgiunzione_richiesta()
RETURNS TRIGGER AS $$
BEGIN
    IF TG_TABLE_NAME = 'prenotazioneaccettata' THEN
        -- Controllo se la richiesta è già presente in prenotazionerifiutata
        IF EXISTS (
            SELECT 1 FROM prenotazionerifiutata WHERE richiesta_id = NEW.richiesta_id
        ) THEN
            RAISE EXCEPTION 'Violazione del vincolo: La RichiestaPrenotazione è già stata rifiutata';
        END IF;
    ELSIF TG_TABLE_NAME = 'prenotazionerifiutata' THEN
        -- Controllo se la richiesta è già presente in prenotazioneaccettata
        IF EXISTS (
            SELECT 1 FROM prenotazioneaccettata WHERE richiesta_id = NEW.richiesta_id
        ) THEN
            RAISE EXCEPTION 'Violazione del vincolo: La RichiestaPrenotazione è già stata accettata';
        END IF;
    END IF;
    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER trg_verifica_disgiunzione_accettata
BEFORE INSERT OR UPDATE ON prenotazioneaccettata
FOR EACH ROW
EXECUTE FUNCTION verifica_disgiunzione_richiesta();

CREATE TRIGGER trg_verifica_disgiunzione_rifiutata
BEFORE INSERT OR UPDATE ON prenotazionerifiutata
FOR EACH ROW
EXECUTE FUNCTION verifica_disgiunzione_richiesta();


-- [V.richiestaprenotazione.specializzazionevalida]
CREATE OR REPLACE FUNCTION verifica_specializzazione_valida()
RETURNS TRIGGER AS $$
BEGIN
    IF NOT EXISTS (
        SELECT 1 
        FROM medico_specializzazione 
        WHERE medico_id = NEW.medico_id 
        AND specializzazione_nome = NEW.specializzazione_nome
    ) THEN
        RAISE EXCEPTION 'Violazione del vincolo: Il medico specificato non ha la specializzazione richiesta';
    END IF;
    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER trg_verifica_specializzazione_valida
BEFORE INSERT OR UPDATE ON richiestaprenotazione
FOR EACH ROW
EXECUTE FUNCTION verifica_specializzazione_valida();


-- [V.richiestaprenotazione.legale]
CREATE OR REPLACE FUNCTION verifica_non_durante_indisponibilita()
RETURNS TRIGGER AS $$
DECLARE
    fine_pren TIMESTAMP;
BEGIN
    -- Calcola l'orario di fine della prenotazione
    fine_pren := NEW.giornoorariopren + INTERVAL '1 hour' * (
        SELECT duratavisitah 
        FROM specializzazione 
        WHERE nome = NEW.specializzazione_nome
    );

    -- Controlla sovrapposizione con le indisponibilità del medico
    IF EXISTS (
        SELECT 1
        FROM indisponibilita
        WHERE medico_id = NEW.medico_id
        AND (
            NEW.giornoorariopren BETWEEN inizioind AND fineind
            OR fine_pren BETWEEN inizioind AND fineind
        )
    ) THEN
        RAISE EXCEPTION 'Violazione del vincolo: La prenotazione coincide con un periodo di indisponibilità del medico';
    END IF;

    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER trg_verifica_non_durante_indisponibilita
BEFORE INSERT OR UPDATE ON richiestaprenotazione
FOR EACH ROW
EXECUTE FUNCTION verifica_non_durante_indisponibilita();


-- [V.prenotazioneaccettata/rifiutata.legale]
CREATE OR REPLACE FUNCTION controllo_richiesta_valida()
RETURNS TRIGGER AS $$
DECLARE
    richiesta RECORD;
BEGIN
    -- Recupera i dati della richiesta corrispondente
    SELECT irich, giornoorariopren
    INTO richiesta
    FROM richiestaprenotazione
    WHERE id = NEW.richiesta_id;

    -- Verifica che la richiesta esista
    IF NOT FOUND THEN
        RAISE EXCEPTION 'Violazione del vincolo: La prenotazione deve riferirsi a una richiesta valida';
    END IF;

    -- Logica per prenotazioneaccettata
    IF TG_TABLE_NAME = 'prenotazioneaccettata' THEN
        IF NOT (NEW.iaccet > richiesta.irich AND NEW.iaccet < richiesta.giornoorariopren) THEN
            RAISE EXCEPTION 'Violazione del vincolo: iaccett deve essere maggiore di irich e minore di giornoorariopren';
        END IF;

    -- Logica per prenotazionerifiutata
    ELSIF TG_TABLE_NAME = 'prenotazionerifiutata' THEN
        IF NOT (NEW.irif > richiesta.irich AND NEW.irif < richiesta.giornoorariopren) THEN
            RAISE EXCEPTION 'Violazione del vincolo: irif deve essere maggiore di irich e minore di giornoorariopren';
        END IF;
    END IF;

    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER controllo_accettazione_valida
BEFORE INSERT OR UPDATE ON prenotazioneaccettata
FOR EACH ROW
EXECUTE FUNCTION controllo_richiesta_valida();

CREATE TRIGGER controllo_rifiuto_valido
BEFORE INSERT OR UPDATE ON prenotazionerifiutata
FOR EACH ROW
EXECUTE FUNCTION controllo_richiesta_valida();


-- [V.feedback.legale]
CREATE OR REPLACE FUNCTION controllo_feedback_valido()
RETURNS TRIGGER AS $$
DECLARE
    prenotazione RECORD;
    paziente_richiesta INT;
BEGIN
    -- Recupera i dati della prenotazione accettata
    SELECT rp.giornoorariopren, pa.prestazioneavvenuta, rp.paziente_id
    INTO prenotazione
    FROM prenotazioneaccettata pa, richiestaprenotazione rp
    WHERE pa.richiesta_id = rp.id and pa.richiesta_id = NEW.prenotazione_accettata_id;

    -- Verifica che la prenotazione esista
    IF NOT FOUND THEN
        RAISE EXCEPTION 'Violazione del vincolo: Il feedback deve riferirsi a una prenotazione accettata valida.';
    END IF;

    -- Verifica che il paziente che sta lasciando il feedback sia lo stesso che ha fatto la prenotazione
    IF prenotazione.paziente_id != NEW.paziente_id THEN
        RAISE EXCEPTION 'Violazione del vincolo: Il paziente non corrisponde a quello che ha effettuato la prenotazione.';
    END IF;

    -- Verifica che la prestazione sia avvenuta
    IF prenotazione.prestazioneavvenuta = false THEN
        RAISE EXCEPTION 'Violazione del vincolo: La prestazione deve essere avvenuta prima di lasciare un feedback.';
    END IF;

    -- Verifica che ifeed sia maggiore di giornoorariopren
    IF NOT (NEW.ifeed > prenotazione.giornoorariopren) THEN
        RAISE EXCEPTION 'Violazione del vincolo: Il timestamp del feedback deve essere maggiore di giornoorariopren.';
    END IF;

    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER controllo_feedback_valido
BEFORE INSERT OR UPDATE ON feedback
FOR EACH ROW
EXECUTE FUNCTION controllo_feedback_valido();