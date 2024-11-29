\c :dbname

create domain codice_fiscale as varchar(16)
    check (value ~ '^[A-Z0-9]{16}$');

create type indirizzo as (
    via varchar(255),
    numero_civico varchar(10),
    cap varchar(5),
    citta varchar(100),
    provincia varchar(100)
);

create type telefono as (
    prefisso_internazionale varchar(5), 
    numero varchar(15)
);

create domain valutazione as integer
check (value between 1 and 5);


create table if not exists persona (
    cf      codice_fiscale  primary key, 
    nome    varchar(100)    not null,
    cognome varchar(100)    not null,
    nascita date            not null
);

create table if not exists paziente (
    id          serial          primary key,
    cf          codice_fiscale  not null, 
    indirizzo   indirizzo       not null,
    email       varchar(100)    not null,
    telefono    telefono        not null,
    foreign key (cf) references persona(cf)
);

create table if not exists medico (
    id      serial          primary key,
    cf      codice_fiscale  not null,
    foreign key (cf) references persona(cf)
);

create table if not exists amministrativo (
    id      serial          primary key,
    cf      codice_fiscale  not null,
    foreign key (cf) references persona(cf)
);

create table if not exists specializzazione (
    nome            varchar(100)    primary key,
    duratavisitah   integer         not null
);

create table if not exists medico_specializzazione (
    medico_id               integer         not null,
    specializzazione_nome   varchar(100)    not null,
    primary key (medico_id, specializzazione_nome),
    foreign key (medico_id) references medico(id),
    foreign key (specializzazione_nome) references specializzazione(nome)
);

create table if not exists indisponibilita (
    id          serial          primary key,
    medico_id   integer         not null,
    inizioind   timestamp       not null,
    fineind     timestamp       not null,
    foreign key (medico_id) references medico(id),
    check (inizioind < fineind)
);

create table if not exists richiestaprenotazione (
    id                      serial          primary key,
    paziente_id             integer         not null,
    medico_id               integer         not null,
    amministrativo_id       integer         not null, 
    specializzazione_nome   varchar(100)    not null,
    irich                   timestamp       not null, 
    giornoorariopren        timestamp       not null, 
    foreign key (paziente_id) references paziente(id),
    foreign key (medico_id) references medico(id),
    foreign key (amministrativo_id) references amministrativo(id),
    foreign key (specializzazione_nome) references specializzazione(nome),
    check (irich < giornoorariopren)
);

create table if not exists prenotazioneaccettata (
    richiesta_id        integer     primary key,
    iaccet              timestamp   not null, 
    prestazioneavvenuta boolean     not null default false,
    foreign key (richiesta_id) references richiestaprenotazione(id)
);

create table if not exists motivazione (
    id      serial          primary key,
    motivo  varchar(100)    not null
);

create table if not exists prenotazionerifiutata (
    richiesta_id        integer     primary key,
    irif                timestamp   not null, 
    motivazione_id      integer     not null,
    foreign key (richiesta_id) references richiestaprenotazione(id),
    foreign key (motivazione_id) references motivazione(id)
);

create table if not exists feedback (
    paziente_id                 integer     not null,
    prenotazione_accettata_id   integer     not null,
    ifeed                       timestamp   not null,
    votosodd                    valutazione not null,
    votopunt                    valutazione not null,
    primary key (paziente_id,prenotazione_accettata_id),
    foreign key (paziente_id) references paziente(id),
    foreign key (prenotazione_accettata_id) references prenotazioneaccettata(richiesta_id)
);