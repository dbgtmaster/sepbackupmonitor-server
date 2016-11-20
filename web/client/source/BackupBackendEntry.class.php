<?php 
/**
 * 
 * ACHTUNG:
 * Wird eine Änderung an dieser Klasse vorgenommen, muss die Änderungen auf allen Kundenservern sowie am Intranet sync Interface aktualisiert 
 * werden, da diese Klasse beim Kunden serialisiert und am Intranet- Server wieder ent-serialisiert wird!!
 *
 */
class BackupJobState {
	
	public static $SUCCESS = 0;
	public static $WARNING = 1;
	public static $ERROR = 2;
	public static $ABORTED = 3;
	public static $MEDIA_INCOMPLETE = 5;	// Wenn eine Sicherung auf 2 Bänder gesichert wird, aber zB. 2te Band nicht verfügbar ist...
	public static $PENDING = 6;				// Wenn ein Job wartet, bis dieser gestartet wird...
	public static $DAILY_PROT = 50;			// Es handelt sich um ein Tagesprotokoll...
	public static $RUNNING = 80;
	public static $UNKNOWN = 99;	
}

class BackupJobType {
	
	public static $JOB = 0;					// Es handelt sich um einen Job
	public static $GROUP = 1;				// Es handelt sich um eine Gruppe
	public static $MIGRATION_JOB = 2;		// Es handelt sich um einen Migrationsjob	
	public static $MIGRATION_JOB_GROUP = 3;		// Es handelt sich um eine Gruppe mit Migrationsjobs..
	public static $PROTOCOL = 4;			// Tagesprotokoll und co....
}
	
class BackupBackendEntry {
	
	private $_softwareId;			// ID, welcher der Job intern in der Backup- Software besitzt.
	private $_taskName;				// Name des Jobs
	private $_startTime;			// Timestamp, wann Job gestartet wurde.
	private $_endTime;				// Timestamp, wann Job beendet wurde.
	private $_date;					// YYYYMMDD: Unter welchem Datum soll dieser Job angezeigt werden?
	private $_state;				// Backup erfolgreich, Warnungen,...
									// Siehe Klasse BackupJobState
	private $_message;				// Eine Kurznachricht bezüglich des Jobs (Sicherung erfolgreich gelaufen, Medium nicht eingelegt,...)
	private $_preProtocolPath;		// Das lange pre Protokoll des Backups (als Pfad)...
	private $_postProtocolPath;		// Das lange post Protokoll des Backups (als Pfad)...
	private $_mainProtocolPath;		// Das lange Protokoll des Backups (als Pfad)...
	private $_type;					// Handelt es sich hier um einen 'job', einer einer 'group',...	
									// Siehe: BackupJobType
	private $_groupSoftwareId = -1; // Interne Software ID der Gruppe, in welcher sich dieser Job befindet.
	private $_backupSoftware;		// Name der Backup- Software (zB. sesam)
	private $_sourceList;			// Array, welche Pfade gesichert werden sollen..
	private $_excludeList;			// Array, welche Pfade nicht gesichert werden sollen...
	private $_dataSize;				// Gesicherte Datenmenge
	private $_jobType;				// Job Type laut Backupsoftware
	private $_throughput;			// Datendurchsatz
	
	private $_clientName;			// Name des Clients, auf welchem dieser Job ausgeführt wurde...
	
	private $_driveName;			// Name des Laufwerkes, auf welchem gesichert wird
	private $_driveType;			// lto, disk,...
	private $_driveClientName;		// Name des Clients, auf welchem das Zielmedium liegt.
	
	public function setTaskName($value) {
		$this->_taskname = $value;
	}
	
	public function setClientName($value) {
		$this->_clientName = $value;
	}
	
	public function setDriveName($value) {
		$this->_driveName = $value;
	}
	
	public function setDriveType($value) {
		$this->_driveType = $value;
	}
	
	public function setDriveClientName($value) {
		$this->_driveClientName = $value;
	}
	
	public function setSoftwareId($value) {
		$this->_softwareId = $value;
	}
	
	public function setStartTime($value) {
		$this->_startTime = $value;
	}
	
	public function setEndTime($value) {
		
		// Falls "false" übergeben wurde... (wenn noch keine Endzeit existiert, da am lauen...)
		if ($value === false) $value = 0;
		
		$this->_endTime = $value;
	}
	
	public function setDate($value) {
		$this->_date = $value;
	}
	
	public function setMessage($value) {
		$this->_message = $value;
	}
	
	public function setGroupSoftwareId($value) {
		$this->_groupSoftwareId = $value;
	}
	
	public function setBackupSoftware($value) {
		$this->_backupSoftware = $value;
	}
	
	/**
	 * Job Status
	 *
	 * @param string $value: BackupJobState:: verwenden!
	 */
	public function setState($value) {
		$this->_state = $value;
	}
	
	// Existiert kein Protokoll, so wird false übergeben...
	public function setPreProtocolPath($value) {
		$this->_preProtocolPath = $value;
	}
	
	public function setPostProtocolPath($value) {
		$this->_postProtocolPath = $value;
	}
	
	public function setMainProtocolPath($value) {
		$this->_mainProtocolPath = $value;
	}
	
	/**
	 * Job Typ
	 *
	 * @param string $value: BackupJobType:: verwenden!
	 */
	public function setType($value) {
		$this->_type = $value;
	}
	
	// Pfade, welche laut Job gesichert werden sollen...
	public function setSource($value = array()) {
		$this->_sourceList = $value;
	}
	
	// Pfade, welche _NICHT_ laut Job gesichert werden sollen...
	public function setExclude($value = array()) {
		$this->_excludeList = $value;
	}
	
	// Gesicherte Datenmenge:
	public function setDataSize($v) {
		$this->_dataSize = $v;
	}
	
	// Job Typ laut Backup Software.
	public function setJobType($v) {
		$this->_jobType = $v;
	}
	
	// Datendurchsatz...
	public function setThroughput($v) {
		$this->_throughput = $v;
	}
	
	public function getTaskName() {
		return $this->_taskname;
	}
	
	// Pfade, welche laut Job gesichert werden sollen...
	public function getSource() {
		return $this->_sourceList;
	}
	
	// Pfade, welche _NICHT_ laut Job gesichert werden sollen...
	public function getExclude() {
		return $this->_excludeList;
	}
	
	// Gesicherte Datenmenge:
	public function getDataSize() {
		return $this->_dataSize;
	}
	
	// Job Typ laut Backup Software.
	public function getJobType() {
		return $this->_jobType;
	}
	
	// Datendurchsatz...
	public function getThroughput() {
		return $this->_throughput;
	}
	
	public function getSoftwareId() {
		return $this->_softwareId;
	}
	
	public function getStartTime() {
		return $this->_startTime;
	}
	
	public function getEndTime() {
		return $this->_endTime;
	}
	
	public function getDate() {
		return $this->_date;
	}
	
	public function getMessage() {
		return $this->_message;
	}
	
	// Gibt das Protocol als String von $byteStart bis $byteEnd zurück...
	public function getProtocolPart($byteStart, $byteEnd) {

		$f = fopen($this->_protocolPath, 'r');
		fseek($f, $byteStart);
		$data = fread($f, ($byteEnd - $byteStart) + 1);
		fclose($f);
		
		return $data;
	}
	
	public function getPreProtocolPath() {
		return $this->_preProtocolPath;
	}
	
	public function getMainProtocolPath() {
		return $this->_mainProtocolPath;
	}
	
	public function getPostProtocolPath() {
		return $this->_postProtocolPath;
	}
	
	public function getType() {
		return $this->_type;
	}
	
	public function getState() {
		return $this->_state;
	}
	
	public function getGroupSoftwareId() {
		return $this->_groupSoftwareId;
	}
	
	public function getSoftwareName() {
		return $this->_backupSoftware;
	}
	
	public function getClientName($value) {
		return $this->_clientName;
	}
	
	public function getDriveName($value) {
		return $this->_driveName;
	}
	
	public function getDriveType($value) {
		return $this->_driveType;
	}
	
	public function getDriveClientName($value) {
		return $this->_driveClientName;
	}
}
?>
