@echo off 

SCHTASKS /DELETE /F /TN "SEP-Backupmonitor-Syncer"
SCHTASKS /CREATE /SC HOURLY /TN SEP-Backupmonitor-Syncer  /TR "C:\Program Files (x86)\SEP-BackupMonitor-Syncer\windows\bin\sync.bat"