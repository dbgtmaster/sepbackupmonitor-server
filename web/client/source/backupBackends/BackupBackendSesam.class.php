<?php
require_once(PATH . 'BackupBackend.class.php'); 

class BackupBackendSesam extends BackupBackend {
	
	private $_pdo;
	
	// Ergebnis der Jobs bzw. Migrationsaufträge
	private $_resultJobs;
	private $_resultJobsFinshed = false;
	private $_resultMigrations;
	
	// Daten im Cache:
	private $_cacheData = array();

	// Pfad zum SEP Dir
	private $_sepPath;	

	// Ob das Tagesprotokoll bereits übertragen wurde...
	private $_dailyProtSend = false;
	private $_dailyProtCount = 0;
	
	public function __construct() {

		if (defined("SESAM_DIR")) {
			if (substr(SESAM_DIR, 0, 3) == '???') {
				die('Config variable SESAM_DIR have not a correct value. Please check config.php');
			}
			$this->_sepPath = SESAM_DIR . '/';
		}

		if (empty($this->_sepPath)) {
			if (isWindows()) {
				die("Config variable SESAM_DIR not defined! Check config.php!\n\n");
			}
			else {
				$this->_sepPath = "/var/opt/sesam/";
			}
		}

		// Im SEP- Pfad muss ein Unterverzeichnis Namens "ini", und "lis" befinden, ansonsten befinden wir uns an einem falschen Ort...
		if (!is_dir($this->_sepPath)) {
			throw new Exception("SEP Dir is not a directory! Path: {$this->_sepPath}");
		}
		if (!is_dir($this->_sepPath . '/var')) {
			throw new Exception("var in SEP Dir is not a directory! Path: {$this->_sepPath}");
		}
		if (!is_dir($this->_sepPath . '/var/ini')) {
			throw new Exception("var/ini in SEP Dir is not a directory! Path: {$this->_sepPath}");
		}
		if (!is_dir($this->_sepPath . '/var/lis')) {
			throw new Exception("var/lis in SEP Dir is not a directory! Path: {$this->_sepPath}");
		}


		/**
		 * Ein 32- Bit System verwendet eine SQLite, ein 64- Bit System wiederrum eine Postgresql Datenbank..
		 */
		try {
		
			if (isWindows()) {
				// Windows benutzt immer SQLITE
				$db = "sqlite";
			}
			elseif (ARCHITECTURE == 'i686') {
				$db = "sqlite";
			}
			elseif (ARCHITECTURE == 'x86_64') {
				$db = "pgsql";
			}

			if ($db == "sqlite") {
				echo "Use sqlite as database.\n";
				$this->_pdo = new PDO("sqlite:{$this->_sepPath}var/db/sesam_db.sdb");
			}
			elseif ($db == "pgsql") {
				
				echo "Use postgresql as database.\n";
				$this->_pdo = new PDO("pgsql:host=localhost;port=11201;user=root;dbname=sesam_db;password=");
			}
			else {
				throw new Exception("ERROR: I dont know which database engine (pgsql or sqlite) should i use... archticture '" . ARCHITECTURE . "'\n");
			}
			
			// Exception werfen, wenn ein SQL Fehler auftritt:
			$this->_pdo->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
			
			echo "Connection successful!\n";
		}
		catch (Exception $e) {
			echo "EXCEPTION in BackupBackendSesam::__construct(): " . $e->getMessage();
			exit(1);
		}
	}
	
	public function reloadNewBackupEntries() {
		
		$this->_dailyProtSend = false;
		$this->_dailyProtCount = 0;
		
		/**
		 * Alle gelaufenen JOBS bis vor SYNC_MAX_DAYS Tage auslesen und schauen, was sich alles geändert hat...
		 */
		$this->_resultJobs = $this->_pdo->query("SELECT r.saveset, r.task, r.start_time, r.stop_time, r.state, r.msg, r.sesam_date, 
													r.cnt, r.fdi_type, r.session_id, r.client, r.drive_num,
													r.source, r.exclude, r.data_size, r.backup_type, r.throughput,
													d.name AS drive_name, d.drive_type,
													c.name AS drive_client_name FROM results AS r
												LEFT JOIN hw_drives d ON r.drive_num = d.drive_num
												LEFT JOIN clients c ON d.client_id = c.client_id
												WHERE start_time > '" . date("Y-m-d", time() - 86400*SYNC_MAX_DAYS+7200 ) . "'" );
		
		/**
		 * Alle gelaufenen JOBS bis vor SYNC_MAX_DAYS Tage auslesen und schauen, was sich alles geändert hat...
		 */
		$this->_resultMigrations = $this->_pdo->query("SELECT m.migration_id, m.parent_task, m.migration_id AS saveset, m.migration_task, m.task, m.start_time, m.stop_time, m.state, m.msg, m.sesam_date,
													m.client, m.target_drive AS drive_num,
													m.size AS data_size, m.throughput,
													d.name AS drive_name, d.drive_type,
													c.name AS drive_client_name FROM migration_results AS m
												LEFT JOIN hw_drives d ON m.target_drive = d.drive_num
												LEFT JOIN clients c ON d.client_id = c.client_id 
												WHERE start_time > '" . date("Y-m-d", time() - 86400*SYNC_MAX_DAYS+7200 ) . "' ORDER BY start_time" );
	}
	
	public function fetchBackupEntry() {

		/**
		 * Wir geben in folgender Reihenfolge die Einträge zurück:
		 * 1.) Die Tagesprotokolle
		 * 2.) Die Sicherungsjobs
		 * 3.) Die Migrationsjobs
		 */
		
		// Prüfen, ob die Tagesprotokolle der letzten Tage schon übertagen wurde...
		if ($this->_dailyProtCount >= SYNC_MAX_DAYS) $this->_dailyProtSend = true;
	
                $isJob = false;
                $isMigrationJob = false;
		$isDailyProtocol = false;
	
		if (!$this->_dailyProtSend) {
			
			/**
			 * Im ersten Schnitt übertragen wir die Tagesprotokolle der letzten CONST SYNC_MAX_DAYS Tage...
			 */
			$dateToday = date("Ymd", time() - ($this->_dailyProtCount * 86400) );
			$dateTodayLong = date("Y-m-d 00:00:00", time() - ($this->_dailyProtCount * 86400) );
			$dateTodayLong2 = date("Y-m-d 23:59:59", time() - ($this->_dailyProtCount * 86400) );
			$this->_dailyProtCount++;
			
			$row = array();
			$row['saveset'] = 'DP' . $dateToday . '@dailyProtocol';					// DP -> DailyProtocol (DP wurde von mir definiert,
																					// um unterscheiden zu können, ob es ein DP ist oder nicht...
																					// Wird daher von SEP selber nicht verwendet, da ein Tagesprotokoll
																					// kein Job ist...
			$row['task'] = 'Tagesprotokoll';
			$row['start_time'] = $dateTodayLong;
			$row['stop_time'] = $dateTodayLong2;
			$row['state'] = 548;
			$row['msg'] = 'Tagesprotokoll';
			$row['sesam_date'] = $dateToday;
			$row['cnt'] = 0;
			$row['fdi_type'] = 'F';
			$row['session_id'] = 'single';
			$row['client'] = 'server';
			$row['drive_num'] = '';
			$row['source'] = '';
			$row['exclude'] = '';
			$row['data_size'] = '0';
			$row['backup_type'] = '';
			$row['throughput'] = '0';
			$row['drive_client_name'] = '';
			$row['drive_name'] = '';
			$row['drive_type'] = '';

			$isDailyProtocol = true;
		}
		else {
		
			if (!$this->_resultJobsFinshed) {
			
				$isJob = true;
				$row = $this->_resultJobs->fetch();
				if ($row === false) {	
					// Nun sollen die Migrationsaufträge abgearbeitet werden...
					$this->_resultJobsFinshed = true;
					return $this->fetchBackupEntry();
				}
			}
			else {
				$isMigrationJob = true;
				$row  = $this->_resultMigrations->fetch();

				// WORKAROUND:
				// In bestimmte SESAM Versionen ist bei der Migrationsgruppe der Spalte "task" leer, und der task-name steht in "migration-task"
				if ($row["task"] == "NULL" OR $row['task'] == '') {
					$row["task"] = $row["migration_task"];
				}

				if ($row === false  || count($row) < 2) {
					return false;
				}
			}
		}
		
		// Ist kein Datum & co angegeben, so existiert dieser Job vl. schon gar nicht mehr, da zu alt...
		if (empty($row['start_time'])) {
			return $this->fetchBackupEntry();
		}

		// Backup- Job erstellen:
		$entry = new BackupBackendEntry;
		$entry->setTaskName($row['task']);
		$entry->setSoftwareId($row['saveset']);
		$entry->setStartTime($row['start_time']);
		$entry->setEndTime($row['stop_time']);
		$entry->setDate($row['sesam_date']);
		$entry->setMessage($row['msg']);
		$entry->setBackupSoftware('sesam');
		$entry->setSource( explode(',', $row['source']) );
		$entry->setExclude( explode(',', $row['exclude']) );
		$entry->setDataSize($row['data_size']);
		$entry->setJobType($row['backup_type']);
		$entry->setThroughput($row['throughput']);
		$entry->setClientName($row['client']);
		$entry->setDriveName($row['drive_name']);
		$entry->setDriveType($row['drive_type']);
		$entry->setDriveClientName($row['drive_client_name']);
		
		if ($isDailyProtocol) {
			$entry->setType( BackupJobType::$PROTOCOL );
		}
		elseif ($isJob) {
			if (substr($row["fdi_type"], 0, 1) == 'G') {
				$entry->setType( BackupJobType::$GROUP );
				if (empty($row["session_id"]) || $row["session_id"] == $row["saveset"]) {
					$entry->setGroupSoftwareId(-1);
				}
				else {
					$entry->setGroupSoftwareId($row["session_id"]);
				}
			}
			else {
				$entry->setType( BackupJobType::$JOB );
				if ($row["session_id"] == "single" || empty($row["session_id"]) ) {
					$entry->setGroupSoftwareId(-1);
				}
				else {
					$entry->setGroupSoftwareId($row["session_id"]);
				}
			}
		}
		elseif ($isMigrationJob) {
			
			if ($row['parent_task'] == '') {
					$entry->setType( BackupJobType::$MIGRATION_JOB_GROUP );
			}
			else {
				$entry->setType( BackupJobType::$MIGRATION_JOB );
				$entry->setGroupSoftwareId($row['parent_task']);
			}
		}
		else {
			/**
			 * Darunter fällt das Tagesprotokoll:
			 */
			$entry->setType( BackupJobType::$JOB );
		}
		
		/**
		 * Backupstatus:
		 * 
		 * State vom laufenden Job???
		 */
		switch ($row['state']) {
			case '0':		$state = BackupJobState::$SUCCESS;			break;
			case '1':		$state = BackupJobState::$WARNING;			break;
			case 'X':		$state = BackupJobState::$ERROR;			break;
			case 'c':		$state = BackupJobState::$ABORTED;			break;
			case 'd':		$state = BackupJobState::$MEDIA_INCOMPLETE;	break;
			case 'q':		$state = BackupJobState::$PENDING;			break;
			case 'a':		$state = BackupJobState::$RUNNING; 			break;
			case '3':		$state = BackupJobState::$ABORTED;			break;
			case '2':		$state = BackupJobState::$UNKNOWN;			break;
			case '548':		$state = BackupJobState::$DAILY_PROT;		break;
			default:		$state = BackupJobState::$UNKNOWN;			break;
		}
		$entry->setState($state);

		$this->_cacheData[$row['saveset']]['data'] = $row;
		
		/**
		 * Pfad des MAIN-, PRE-, POST- Protokolls ermitteln:
		 */

		$type = substr($row['saveset'], 0, 2);

		if ($isDailyProtocol) {
			// Tagesprotokoll...
			$entry->setMainProtocolPath($this->_sepPath .  'var/prot/' . $dateToday . '.prt');
		}
		elseif ($isJob) {
		
			// Liste mit möglichen Pfaden zum Protokoll....
			// SEP hat den Pfad leider von Version X zu Y geändert, daher müssen wir 2 Pfade probieren...
			$protocolPathMain1 = $this->_sepPath . 'var/lis/' . $row['task'] . '-' . substr($row['sesam_date'], 0, 4) . substr($row['sesam_date'], 5, 2) . substr($row['sesam_date'], 8, 2) . '_' . 
							str_pad($row['cnt'], 3 ,'0', STR_PAD_LEFT) . '_' . $row['saveset'] . '.not';
							
			$protocolPathMain2 = $this->_sepPath . 'var/lis/' . $row['task'] . '-' . substr($row['sesam_date'], 0, 4) . substr($row['sesam_date'], 5, 2) . substr($row['sesam_date'], 8, 2) . '_' . 
							str_pad($row['cnt'], 3 ,'0', STR_PAD_LEFT) . '_' . $type . '.not';
							
			$protocolPathPre1 = $this->_sepPath . 'var/prepost/' . $row['task'] . '-' . substr($row['sesam_date'], 0, 4) . substr($row['sesam_date'], 5, 2) . substr($row['sesam_date'], 8, 2) . '_' . 
							str_pad($row['cnt'], 3 ,'0', STR_PAD_LEFT) . '_' . $row['saveset'] . '.pre';
							
			$protocolPathPre2 = $this->_sepPath . 'var/prepost/' . $row['task'] . '-' . substr($row['sesam_date'], 0, 4) . substr($row['sesam_date'], 5, 2) . substr($row['sesam_date'], 8, 2) . '_' . 
							str_pad($row['cnt'], 3 ,'0', STR_PAD_LEFT) . '_' . $type . '.pre';
							
			$protocolPathPost1 = $this->_sepPath . 'var/prepost/' . $row['task'] . '-' . substr($row['sesam_date'], 0, 4) . substr($row['sesam_date'], 5, 2) . substr($row['sesam_date'], 8, 2) . '_' . 
							str_pad($row['cnt'], 3 ,'0', STR_PAD_LEFT) . '_' . $row['saveset'] . '.pos';
							
			$protocolPathPost2 = $this->_sepPath . 'var/prepost/' . $row['task'] . '-' . substr($row['sesam_date'], 0, 4) . substr($row['sesam_date'], 5, 2) . substr($row['sesam_date'], 8, 2) . '_' . 
							str_pad($row['cnt'], 3 ,'0', STR_PAD_LEFT) . '_' . $type . '.pos';
				
	
			// Main Protocol
			if (is_file($protocolPathMain1)) {
				$entry->setMainProtocolPath($protocolPathMain1);
			}
			else {
				$entry->setMainProtocolPath($protocolPathMain2);
			}
			
			// Pre Protocol
			if (is_file($protocolPathPre1)) {
				$entry->setPreProtocolPath($protocolPathPre1);
			}
			else {
				$entry->setPreProtocolPath($protocolPathPre2);
			}
			
			// Post Protocol
			if (is_file($protocolPathPost1)) {
				$entry->setPostProtocolPath($protocolPathPost1);
			}
			else {
				$entry->setPostProtocolPath($protocolPathPost2);
			}
		
		}
		elseif ($isMigrationJob) {
			$entry->setMainProtocolPath($this->_sepPath .  'var/lis/' . $row['migration_task'] . '-' . $row['task']. '_' . $row['migration_id'] . '.mig');
		}
		
		return $entry;
	}
	
	public function deleteEntry($entry) {

	}
	
	public function finishedBackend() {
		
	}
	
	public function initBackend() {
		
	}
}
?>
