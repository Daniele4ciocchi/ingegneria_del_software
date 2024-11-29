\c :log_dbname 

create domain string as varchar(50);

create domain realgez as real check (value >= 0);

create type sessionstatistictype as enum ('Session', 'Response');

create table if not exists client (
        server_name     string      not null,
        file_descriptor integer     not null,
        iconnection     timestamp   not null,
        idisconnection  timestamp,

        primary key (server_name, file_descriptor, iconnection),
        check (idisconnection is null or idisconnection >= iconnection)
);

create table if not exists communication (
        request                 varchar(1000)   not null,
        irequest                timestamp       not null,
        response_status         string,
        iresponse               timestamp,
        client_server_name      string          not null,
        client_file_descriptor  integer         not null,
        client_iconnection      timestamp       not null,

        primary key (client_server_name, client_file_descriptor, client_iconnection, irequest),
        foreign key (client_server_name, client_file_descriptor, client_iconnection) references client(server_name, file_descriptor, iconnection),
        check (iresponse is null or iresponse >= irequest),
        ccheck (irequest >= client_iconnection)
);

create table if not exists sessionstatistic (
        id              serial                  not null primary key,
        type            sessionstatistictype    not null,
        iend            timestamp               not null,
        value           realgez                 not null,
        response_status string                  not null,

        unique (type, iend)
);