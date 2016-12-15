Installation SEP- Backupmonitor servers

Es wird von einener Neuinstallation eines Debian 8 Servers ausgegangen (Minimal- Installation ohne grafischer Oberfläche). Als Systemsprache wurde Englisch gewählt. 
Desweiteren muss der Server mit einer statischen IP versehen werden.

Zum Bauen des sepbackupmonitor-server müssen folgende Pakete instaliert werden:
apt-get install git qt5-default qt5-qmake make g++ libldap2-dev php5 php5-pgsql

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

Webinterface API:
mkdir /usr/share/sepbackupmonitor-interface
cp -R web/interface/public/* /usr/share/sepbackupmonitor-interface
cp web/interface/sepbackupmonitor.conf /etc/apache2/conf-available/sepbackupmonitor.conf
ln -s /etc/apache2/conf-available/sepbackupmonitor.conf /etc/apache2/conf-enabled/sepbackupmonitor.conf

mkdir -p /var/lib/sepbackupmonitor-server/backup-protocols
mkdir -p /var/lib/sepbackupmonitor-server/backup-disasters

# Create sepbackupmonitor-syncer client
sh /usr/local/share/sepbackupmonitor-interface/client/create_source_file.sh

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

Nun kann der SEPBackupmonitor-Client gestartet werden.
Beim Login Fenster gibt es einen Button "Server konfigurieren". Dort kann entsprechend der SEPBackupmonitor-Server definiert werden. Per Default wird zum Hostnamen "sepbackupmonitor" verbunden. Wird ein entsprechender DNS Eintrag zeigend zum SEPBackupmonitor-Server angelegt, muss nichts weiter konfiguriert werden.

Da derzeit kein Benutzer die Berechtigung hat, auf den Backupmonitor zuzugreifen, muss der BackupMonitor-Server mit einer speziellen Berechtigung gestartet werden, welche es einem Benutzer erlaubt, sich einzuloggen, damit die Grundkonfiguration vorgenommen werden kann.

Dazu den Backupmonitor stoppen:
systemctl stop sepbackupmonitor-server

Und den Benutzer "Administrator" volle Rechte erlauben.
/usr/local/bin/sepbackupmonitor-server --fullTcpPermissions Administrator

Nun kann man sich mit dem Benutzer "Administator" in den SEPBackupmonitor Client einloggen.

Nachdem man sich eingeloggt hat, wählt man in der Menüleiste "Administration -> Benutzergruppen" aus und klickt auf den Button "Neue Gruppe".

Bei Gruppenname kann zB. "Users" eingegeben werden (muss nicht mit LDAP ident sein!).
Bei "Zuordnung Systemgruppen" wird "UserAccount" und "BackupMonitor" ausgewählt.
Reiter "Zuordnung Benutzer" auswählen. Dort muss definiert werden, welche Benutzer zugreifen dürfen.
Als Beispiel:
Benutzer der Gruppe "Backupmonitor" dürfen zugreifen (getestet mit Univention):
LDAP Filter 1:
- Basis DN: cn=Backupmonitor,cn=groups,dc=test,dc=intranet
- Scope: Base
- Suchfilter: memberUid=%1

"Benutzer <-> Gruppen Zuordnung testen" klicken und einen Benutzer eingeben, welcher sich in dieser Gruppe befindet. Trifft kein Filter zu, bitte die LDAP Filter Konfiguration nochmals überprüfen.

Nun kann der "sepbackupmonitor-server" auf der Commandline wieder gestoppt werden und via systemctl wieder gestartet:
systemctl start sepbackupmonitor-server

Nun ist der Login mit allen erlaubten Benutzern möglich!