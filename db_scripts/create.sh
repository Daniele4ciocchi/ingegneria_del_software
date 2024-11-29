#!/bin/sh

sudo -u postgres psql postgres -f db/parameters.sql -f db/setup.sql -f db/schema.sql -f db/permissions.sql -f db/triggers.sql -f db/data.sql  -f db/tests.sql -f log_sb/setup_log.sql -f log_sb/schemas_log.sql -f log_sb/permissions_log.sql -f log_sb/triggers_log.sql 