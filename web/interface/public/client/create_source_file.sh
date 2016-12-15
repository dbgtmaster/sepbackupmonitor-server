#!/bin/bash
# Create tar file for distribution

# delete config and cache folders
rm /usr/local/share/sepbackupmonitor-interface/client/source/config.php -f
rm /usr/local/share/sepbackupmonitor-interface/client/source/cache -Rf

## Tar erstellen:
cd /usr/local/share/sepbackupmonitor-interface/client/
tar cvf sepbackupmonitor-syncer.tar source/*

cp /usr/local/share/sepbackupmonitor-interface/client/source/VERSION /usr/local/share/sepbackupmonitor-interface/client/sepbackupmonitor-syncer.VERSION

echo "Finsihed..."
