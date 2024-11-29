\c :log_dbname 

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

-- [V.client.filedescriptor]
CREATE OR REPLACE FUNCTION filedescriptor_valido() RETURNS TRIGGER AS $$
BEGIN
    IF EXISTS (
        SELECT 1 
        FROM client
        WHERE NEW.server_name = client.server_name
        AND NEW.file_descriptor = client.file_descriptor
        AND (client.idisconnection IS NULL OR NEW.iconnection <= client.idisconnection)
    ) IS TRUE THEN
        RAISE EXCEPTION 'Esiste già una connessione con questo file descriptor';
    END IF;

    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER filedescriptor_valido_trg
BEFORE INSERT ON client FOR EACH ROW EXECUTE FUNCTION filedescriptor_valido();