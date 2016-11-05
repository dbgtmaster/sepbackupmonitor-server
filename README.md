Installation SEP- Backupmonitor servers

Es wird von einener Neuinstallation eines Debian 8 Servers ausgegangen (Minimal- Installation ohne grafischer Oberfläche). Als Systemsprache wurde Englisch gewählt. 
Desweiteren muss der Server mit einer statischen IP versehen werden.

Zum Bauen des sepbackupmonitor-server müssen folgende Pakete instaliert werden:
apt-get install git qt5-default qt5-qmake make g++ libldap2-dev

Wenn die postgresql Datenbank auf diesen Server installiert werden:
apt-get install postgresql

Wenn das HTTPs Interface zum synchroniseren der Clients auf diesem Server laufen soll, so auch Apache installieren:
apt-get install apache2

Jetzt checken wir den server aus und kompilieren diesen:
=======
sepbackupmonitor-server auschecken und kompilieren:

mkdir /root/software
cd /root/software
git clone https://github.com/dbgtmaster/sepbackupmonitor-server
cd sepbackupmonitor-server/
qmake && make

Server Binary und Konfigurations- Datei kopieren:
cp sepbackupmonitor-server /usr/local/bin/
mkdir /usr/local/etc/sepbackupmonitor/;
cp skel/server.ini /usr/local/etc/sepbackupmonitor/server.ini
cp skel/init-scripts-sh /lib/systemd/system/sepbackupmonitor-server.service

## Datenbank erstellen:
cp skel/import.sql /tmp/;
chown postgres /tmp/import.sql
su postgres
psql
CREATE DATABASE sepbackupmonitor;
\c sepbackupmonitor;
\i /tmp/import.sql

Die Konfigurationsdatei öffnen "/usr/local/etc/sepbackupmonitor/server.ini" und entsprechend konfigurieren.

systemctl enable sepbackupmonitor-server.service
systemctl start sepbackupmonitor-server.service