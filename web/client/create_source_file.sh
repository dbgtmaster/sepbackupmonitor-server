#!/bin/bash

cd /var/www/intranet/customer_backup_syncer

# Konfiguration wird NIE uebertragen
rm source/config.php -f
rm source/cache -Rf

## Tar erstellen:
tar cvf siedl_backupsyncer.tar source/* 
cp source/VERSION siedl_backupsyncer.VERSION

echo "Finsihed..."
