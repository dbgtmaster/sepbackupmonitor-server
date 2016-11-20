@echo off

REM Das Programm muss so gestartet werden, dass wir uns im Hauptverzeichnis des siedl-backupsyncer befinden:
cd /d %0\..\..\..

windows\php\php.exe -c windows\php\php.ini sync.php