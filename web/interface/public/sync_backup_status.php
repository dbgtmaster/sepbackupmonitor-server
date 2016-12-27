<?php 
require_once('config.php');
require_once('Db.class.php');
require_once('DbResult.class.php');
require_once('BackupBackendEntry.class.php');

define('PROTOCOL_PATH', '/var/lib/sepbackupmonitor-server/backup-protocols/');
define('DISASTER_PATH', '/var/lib/sepbackupmonitor-server/disasters/');
define('PROTOCOL_LINES_PER_PAGE', 1000);		// Pro Protokollseite X Zeilen anzeigen.

$customerid = (int) $_GET['customerid'];
$customerkey = $_GET['customerkey'];

$db = new Db($CONFIG['database']['hostname'], $CONFIG['database']['username'], $CONFIG['database']['password'], $CONFIG['database']['database']);

// Überprüfen, ob Kunde mit diesem KEY Daten übermitteln darf???
$key = $db->escape($customerkey);
$exists = $db->query("SELECT COUNT(*) AS count FROM customers 
						WHERE id = {$customerid} AND backup_enabled = true AND backup_interface_key = '$key'")->fetch('count');

if ($exists == '0') die('No permission to use this interface');

if ($_GET['action'] == 'init') {
	
	/**
	 * Dinge, die vor dem synchronisieren passieren...
	 * Gibt zB. die aktuelle Version der siedl_client_syncer zurück...
	 */
	die( file_get_contents('client/sepbackupmonitor-syncer.VERSION') );
}
elseif ($_GET['action'] == 'get_newest_client') {
	
	// Überträgt zum Cient die neuerste Client- Version:
	header('Content-Description: File Transfer');
	header("Content-Type: application/x-tar");
	header('Content-Length: ' . filesize('client/sepbackupmonitor-syncer.tar'));
	header('Content-Disposition: attachment; filename=' . basename("sepbackupmonitor-syncer.tar"));
	header("Content-Transfer-Encoding: binary");
	die(file_get_contents('client/sepbackupmonitor-syncer.tar'));
}
elseif ($_GET['action'] == 'sync_job') {

	/**
	 * Nun werden die übermittelten Daten verarbeitet:
	 */
	$entry = unserialize( bzdecompress( base64_decode($_POST['data']) ) );

	// Haben wir verarbeitbare Daten erhalten??
	if ($entry === false) {
		echo "ROW: {$_POST['data']}";
		die('unserialize failed, returns false! md5 of data: ' . md5($_POST['data']) . ' , size: ' . strlen($_POST['data']));
	}

	if (!is_object($entry) || get_class($entry) != 'BackupBackendEntry') die('Invalid data arrived. #0');

	if ($entry->getSoftwareId() == '') die("Invalid saveset. #1");
	if ($entry->getTaskName() == '') die('Invalid saveset. #2');
	if ($entry->getStartTime() == '') die('Invalid saveset. #3');
	if (!is_numeric($entry->getState()) || $entry->getState() < 0 || $entry->getState() > 99) die('Invalid saveset. #6');
	if ($entry->getType() !== BackupJobType::$GROUP && $entry->getType() !== BackupJobType::$JOB  && $entry->getType() !== BackupJobType::$PROTOCOL
		&& $entry->getType() !== BackupJobType::$MIGRATION_JOB && $entry->getType() !== BackupJobType::$MIGRATION_JOB_GROUP) die('Invalid saveset. #7');
	if ($entry->getGroupSoftwareId() == '') die('Invalid saveset. #8');
	if (!is_array($entry->getSource())) die('Invalid saveset. #9');
	if (!is_array($entry->getExclude())) die('Invalid saveset. #10');	
	
	// Datensatz verarbeiten:
	$cols = array();
	$cols['customer_id'] = $customerid;
	$cols['software_id'] = $entry->getSoftwareId();
	$cols['taskname'] = $entry->getTaskName();
	$cols['start_time'] = $entry->getStartTime();
	// Wenn keine Endzeit definiert ist, so setzen wir die Startzeit (laufende Jobs,..)
	$cols['end_time'] = (mb_strlen($entry->getEndTime()) < 2) ? $entry->getStartTime() : $entry->getEndTime();
	$cols['date'] = $entry->getDate();
	$cols['state'] = $entry->getState();
	$cols['msg'] = $entry->getMessage();
	$cols['type'] = $entry->getType();
	$cols['group_softwareid'] = $entry->getGroupSoftwareId();
	$cols['source_list'] = implode("\n", $entry->getSource());
	$cols['exclude_list'] = implode("\n", $entry->getExclude());
	$cols['data_size'] = (int) $entry->getDataSize();
	$cols['throughput'] = (int) $entry->getThroughput();
	$cols['job_type'] = $entry->getJobType();
	$cols['client_name'] = $entry->getClientName();
	$cols['drive_name'] = $entry->getDriveName();
	$cols['drive_type'] = $entry->getDriveType();
	$cols['drive_client_name'] = $entry->getDriveClientName();
	$cols['job_state_changed'] = 'true';

	// Ab mit den Daten in die Datenbank...
	// Überprüfen, ob dieser Saveset schon existiert, wenn ja, so wird bloss ein Update durchgeführt:
	$e_saveset = $db->escape( $entry->getSoftwareId() );
	$row = $db->query("SELECT id FROM backup_jobs WHERE customer_id = {$customerid} AND software_id = '{$e_saveset}'")->fetch();
	if (!is_array($row)) {
		
		// Saveset existiert noch nicht, neuen Datensatz erstellen:
		$db->insert($cols, 'backup_jobs');
	}
	else {
	
		// Saveset existiert schon, geänderte Daten updaten.
		$db->update($cols, 'backup_jobs', array('id' => $row['id']) );
	}
	
	// Alles glatt gelaufen:
	die('ok');
}
elseif ($_GET['action'] == 'sync_disaster') {
	
	$data = unserialize( bzdecompress( base64_decode($_POST['data']) ) );
	if (!is_array) {
		die('invalid data reveived.');
	}
	$fileStamp = (int) $data['filestamp'];
	$disasterText = $data['text'];
	
	if (empty($disasterText)) {
		die("sorry, disaster is empty!");
	}
	
	$diasterPath = PROTOCOL_PATH . '/' . $customerid;
	
	/**
	 * Disaster auf das Filesystem schreiben:
	 */
	if (!$fDisaster = fopen($diasterPath, 'w+')) {
		die("could not open $diasterPath for writing");
	}
	fseek($fDisaster, 0);
	if (!fwrite($fDisaster, gzcompress($disasterText))) {
		die("could not write $diasterPath");
	}
	fclose($fDisaster);
	
	// Und Timestamp in die Datenbank:
	$db->update(array('backup_disaster_last_update' => $fileStamp), 'customers', array('id' => $customerid) );
	
	die("ok");
}
elseif ($_GET['action'] == 'sync_protocol') {

	$softwareId = $db->escape( $_POST['softwareId'] );
	$protocolType = $db->escape($_POST['protocolType']);
	
	$validProtocolTypes = array('pre', 'post', 'main');
	if (!in_array($protocolType, $validProtocolTypes)) {
		die("invalid protocol type '$protocolType'");
	}
	
	// von der Software- ID die JOB- ID auslesen:
	$r = $db->query("SELECT id, date FROM backup_jobs WHERE customer_id = {$customerid} AND software_id = '". $db->escape($softwareId) ."'")->fetch();
	if (!is_array($r)) {
		die('software id ' . $softwareId . ' does not exists!');
	}
	$jobId = (int) $r['id'];
	
	$protocolPath = PROTOCOL_PATH . '/' . ($jobId % 100) . '/' . ($jobId % 99);
	$protocolFile = $protocolPath  . "/{$jobId}_$protocolType";
	
	if (isset($_GET['resetProtocol'])) {
		$i = 1;
		@unlink($protocolFile);
		while (@unlink($protocolFile . '_' . $i)) {
			$i++;
		}
	}

	$protocolToAdd = explode("\n", gzuncompress( base64_decode($_POST['data'])));
	
	@mkdir($protocolPath, 0770, true);
	
	if (!$fMain = fopen($protocolFile, 'w+')) {
		die("could not open $protocolFile for writing");
	}
	// Anzahl der aktuell verfügbaren Seiten des Protokolls:
	if (filesize($protocolFile) > 0) {
		$pagesProt = fread($fMain, filesize($protocolFile));
	}
	else {
		$pagesProt = '';
	}
	if (empty($pagesProt)) {
		fseek($fMain, 0);
		if (!fwrite($fMain, '1')) {
			die("could not write $protocolFile");
		}
		$pagesProt = 1;
	}

	// Nun öffnen wir das letzte Protokoll, und fügen die Zeilen hinzu...
	while (true) {
		
		if (file_exists($protocolFile . '_' . $pagesProt)) {
			$lastProtocolPage = file_get_contents($protocolFile . '_' . $pagesProt);
			$lastProtocolPage = explode("\n", gzuncompress($lastProtocolPage));
		}
		else {
			$lastProtocolPage = array();
		}

		$lastProtocolPageCount = count($lastProtocolPage);
		$isFirstAppend = true;
		for ($i = $lastProtocolPageCount; $i <= PROTOCOL_LINES_PER_PAGE; $i++) {
			if (count($protocolToAdd) == 0) break;
			if ($isFirstAppend) {
				// Die erste hinzuzufuegende Zeile haengen wir an die letzte dran, da sonst mit jeder neuen Uebertragung ein Zeilenumbruch erzeugt wird...
				$lastProtocolPage[ count($lastProtocolPage) - 1 ] = $lastProtocolPage[ count($lastProtocolPage) - 1 ] . array_shift($protocolToAdd);	
				$isFirstAppend = false;
			}
			else {
				$lastProtocolPage[] = array_shift($protocolToAdd);
			}
		}
		
		// Protokollseite
		$data = implode("\n", $lastProtocolPage);
		if (!file_put_contents($protocolFile . '_' . $pagesProt, gzcompress($data))) {
			die("Cannot write to: " . $protocolFile . '_' . $pagesProt);
		}
		
		// Wenn $protocolToAdd nun nicht leer ist, so haben wir das Limit des letzten Protokolls erreicht, und müssen
		// in einer neuen Datei fortsetzen:
		if (count ($protocolToAdd) > 0) {
			$pagesProt++;
			fseek($fMain, 0);
			if (!fwrite($fMain, $pagesProt)) {
				die("could not write $protocolFile");
			}
		}
		else {
			// Protokoll geschrieben...
			break;
		}
	}

	fclose($fMain);
	
	die("ok");
}
elseif ($_GET['action'] == 'sync_hello') {

	$syncVersion = $db->escape($_POST['sync_version']);
	
	$time = time();
	$db->query("UPDATE customers SET backup_last_hello = $time, backup_sync_client_version = '$syncVersion' WHERE id = $customerid");
	die("ok");
}
else {
	die('unknown action!');
}
?>

