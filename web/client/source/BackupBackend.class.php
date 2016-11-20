<?php 
require_once(PATH . 'BackupBackendEntry.class.php');

abstract class BackupBackend {
	
	// Diese Funktion läd alle noch nicht abgearbeiteten Backup- Jobs...
	// ACHTUNG: Diese Methode muss auch alle geänderten Jobs holen (Protokoll verändert, Job Status geändert,...)
	// Diesekönnen dann über die Funktion getNextBackupEntry() geholt werden.
	abstract public function reloadNewBackupEntries();
	
	// Diese Funktion gibt das nächste noch nicht bearbeitende BackupBackendEnty Objekt zurück...
	// Falls keines mehr existiert, wird "false" zurückgegeben.
	abstract public function fetchBackupEntry();
	
	// Löscht einen Eintrag, so dass dieser als "erledigt" marktiert wird und beim nächsten Aufruf reloadNewBackupEntries() nicht
	// mehr geladen wird.
	// Als Parameter wird das BackupBackendEntry Object erwartet.
	abstract public function deleteEntry($entry);
	
	// Bereitet das Backend für die erste Verwendung vor (Verzeichnisse erstellen, Datenbanktabellen erstellen,...)
	abstract public function initBackend();
	
	// Wird aufgerufen, wenn das Backend erfolgreich ausgeführt wurde..
	abstract public function finishedBackend();
}
?>