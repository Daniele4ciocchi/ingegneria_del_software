#!/bin/sh

sudo -u postgres psql postgres -f db/parameters.sql -f db/setup.sql -f db/schema.sql -f db/permissions.sql -f db/triggers.sql -f db/data.sql  -f db/tests.sql -f log_db/log_setup.sql -f log_db/log_schema.sql -f log_db/log_permissions.sql -f log_db/log_triggers.sql 