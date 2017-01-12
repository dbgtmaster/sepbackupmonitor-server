Installation SEP- Backupmonitor Servers
==

Es wird von einener Neuinstallation eines Debian 8 Servers ausgegangen (Minimal- Installation ohne grafischer Oberfläche). Als Systemsprache wurde Englisch gewählt. 
Desweiteren muss der Server mit einer statischen IP versehen werden.

Zum Bauen des sepbackupmonitor-server müssen folgende Pakete installiert werden:
```
apt-get install git qt5-default qt5-qmake make g++ libldap2-dev php5 php5-pgsql apache2 postgresql libqt5sql5-psql
```

Nun wird der sepbackupmonitor-server vom github ausgecheckt und kompiliert:
```
mkdir /root/software
cd /root/software
git clone https://github.com/dbgtmaster/sepbackupmonitor-server
cd sepbackupmonitor-server
qmake && make
```
Server Binary und Konfigurations- Datei an den Ziel- Ort kopieren:
```
cp /root/software/sepbackupmonitor-server/sepbackupmonitor-server /usr/local/bin/
mkdir /usr/local/etc/sepbackupmonitor/;
cp /root/software/sepbackupmonitor-server/skel/server.ini /usr/local/etc/sepbackupmonitor/server.ini
cp /root/software/sepbackupmonitor-server/skel/init-scripts.sh /lib/systemd/system/sepbackupmonitor-server.service
```

Konfiguration des Web- Interfaces:
```
mkdir /usr/local/share/sepbackupmonitor-interface
cp -R /root/software/sepbackupmonitor-server/web/interface/public/* /usr/local/share/sepbackupmonitor-interface
cp /root/software/sepbackupmonitor-server/web/interface/sepbackupmonitor.conf /etc/apache2/conf-available/sepbackupmonitor.conf
ln -s /etc/apache2/conf-available/sepbackupmonitor.conf /etc/apache2/conf-enabled/sepbackupmonitor.conf

mkdir -p /var/lib/sepbackupmonitor-server/backup-protocols
mkdir -p /var/lib/sepbackupmonitor-server/backup-disasters
chown www-data.www-data /var/lib/sepbackupmonitor-server/ -R

# Create sepbackupmonitor-syncer client
sh /usr/local/share/sepbackupmonitor-interface/client/create_source_file.sh
```

Nun wird das Grund- Schema der Datenbank importiert:
```
cp /root/software/sepbackupmonitor-server/skel/import.sql /tmp/;
chown postgres /tmp/import.sql
su postgres
psql
CREATE DATABASE sepbackupmonitor;
\c sepbackupmonitor;
\i /tmp/import.sql
\q
```

Die Konfigurationsdatei öffnen "/usr/local/etc/sepbackupmonitor/server.ini" und entsprechend konfigurieren.
Der Datenbank- Benutzername lautet "postgres", dass Passwort entspricht jenes des root- Benutzers.

Backupmonitor-Server starten:
```
systemctl enable sepbackupmonitor-server.service
systemctl start sepbackupmonitor-server.service
```

Bitte im Log- File "/var/log/sepbackupmonitor-server.log" überprüfen, ob der Dienst erfolgreich gestartet wurde.

Nun kann der SEPBackupmonitor-Client auf einem PC gestartet werden. Dieser Client dient der Verwaltung der SEP- Server.
Download: https://github.com/dbgtmaster/sepbackupmonitor-client

Beim Start des Clients wird beim Login- Fenster ein Button "Server konfigurieren" angezeigt. Dort kann der SEPBackupmonitor-Server definiert werden. Per Default wird eine Verbindung zum Server mit dem Hostnamen "sepbackupmonitor" angelegt. Wird ein entsprechender DNS Eintrag zeigend zum SEPBackupmonitor-Server angelegt, muss nichts weiter konfiguriert werden.

Da derzeit kein Benutzer die Berechtigung hat, auf den BackupMonitor zuzugreifen, muss der BackupMonitor-Server mit einer speziellen Berechtigung gestartet werden, welche es einem Benutzer erlaubt, sich mit vollen Berechtigungen einzuloggen, damit die Grundkonfiguration vorgenommen werden kann.

Dazu den Backupmonitor stoppen:
```
systemctl stop sepbackupmonitor-server
```

Und den Benutzer "Administrator" volle Rechte erlauben.
```
/usr/local/bin/sepbackupmonitor-server --fullTcpPermissions Administrator
```
ACHTUNG: Der oben genannte Befehl läuft endlos! Der Befehl wird erst im späteren Verlauf mit STRG+C wieder abgebrochen.

Nun kann man sich mit dem Benutzer "Administator" in den SEPBackupmonitor Client einloggen.
Sollte ein Login nicht möglich sein, bitte am sepbackupserver die Konfigration überprüfen!

Nachdem man sich eingeloggt hat, wählt man oben in der Menüleiste "Administration -> Benutzergruppen" aus und klickt auf die Auswahl "Neue Gruppe".

Bei Gruppenname kann zB. "Users" eingegeben werden (muss nicht mit LDAP ident sein!).
Bei "Zuordnung Systemgruppen" wird "UserAccount" und "BackupMonitor" ausgewählt.
Reiter "Zuordnung Benutzer" auswählen. Dort muss definiert werden, welche Benutzer den SEPBackupMonitor verwenden dürfen.
Als Beispiel:
Benutzer der Gruppe "Backupmonitor" dürfen zugreifen (getestet mit Univention):
LDAP Filter 1:
- Basis DN: cn=Backupmonitor,cn=groups,dc=test,dc=intranet
- Scope: Base
- Suchfilter: memberUid=%1

"Benutzer <-> Gruppen Zuordnung testen" klicken und einen Benutzer eingeben, welcher sich in dieser Gruppe befindet. Trifft kein Filter zu, bitte die LDAP Filter Konfiguration nochmals überprüfen.

Nun kann der "sepbackupmonitor-server" auf der Commandline mit STRG+C wieder gestoppt werden und via systemctl wieder gestartet:
```
systemctl start sepbackupmonitor-server
```

Nun ist der Login mit allen erlaubten Benutzern möglich!

Am Backupmonitor-client einloggen, auf "Kundenverwaltung" klicken und einen Kunden anlegen (in der server.ini muss bei der Sektion "customer" die Option "editable" aktiviert sein). Nun den eben angelegten Kunden öffnen, auf "Backupmonitor" klicken und dort auf den Button "Generiere neuen CUSTOMER_INTERFACE_KEY". Jetzt werden Zugangsdaten für die Synchronisierung des Kunden erzeugt.

Am unteren Ende des Fensters wird ein Bash Befehl angezeigt, welcher auf den SEP- Server ausgeführt werden muss. Dieser Befehl installiert den Client zur Synchronisierung.
Bevor der Befehl ausgeführt wird, muss überprüft werden, ob folgende Pakete am sepserver installiert sind:
- php5
- php5-pgsql (only for 64bit system)
- php5-zlib
- php5-bz2
- php5-openssl
- php5-curl

Vor der Installation muss verifiziert werden, ob der sepbackupmonitor-server von extern über Port 443 erreichbar ist. Sollte diese nicht der Fall sein, muss der Router / die Firewall entsprechend konfiguriert werden.

Der Installer des Syncer- Clients fragt nach einer CustomerID sowie nach dem erzeugten CUSTOMER_INTERFACE_KEY. Nach erfolgter Installation wird automatisch ein Cron angelegt: "/etc/cron.hourly/sepbackupmonitor-syncer"
Der sepbackupmontitor-syncer wird nach "/usr/local/lib/sepbackupmonitor-syncer" installiert.
Nun erfolgt stündlich eine automatsche synchronisierung der Backup- Jobs.

Im Backupmonitor-client kann unter "Backupmonitor" nun die synchronisierten Jobs betrachtet werden.
Jetzt muss noch konfiguriert werden, wie viele Jobs täglich erwartet werden. Dazu einen Rechtsklick auf den Kunden, den Tab "Backupmonitor" auswählen. Bei den Einstellungen "Anzahl der zu erwartenden Jobs" werden die Anzahl der Jobs eintragen, welche mindestens pro Wochentag erwartet werden.


Hilft & Support: http://www.siedl.net
