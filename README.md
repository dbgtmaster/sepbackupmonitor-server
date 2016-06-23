Installation SEP- Backupmonitor servers

Es wird von einener Neuinstallation eines Debian 8 Servers ausgegangen (Minimal- Installation ohne grafischer Oberfläche). Als Systemsprache wurde Englisch gewählt. 
Desweiteren muss der Server mit einer statischen IP versehen werden.

Zum bauen des sepbackupmonitor-server müssen folgende Pakete instaliert werden:
apt-get install git qt5-default qt5-qmake make g++ libldap2-dev

Jetzt checken wir den server aus und kompilieren diesen:
mkdir /root/software
cd /root/software
git clone https://github.com/dbgtmaster/sepbackupmonitor-server
cd sepbackupmonitor-server/
qmake && make