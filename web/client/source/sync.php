<?php 
/**
 * I need php5-sqlite bzw. php5-pgsql and php5-bz2 and php5-zlib
 */

error_reporting(E_ALL ^ E_NOTICE);

if (!function_exists('curl_init')) {
	die("ERROR: Please install php5-curl!\n");
	exit(1);
}

// Wenn das Skript länger als 300s läuft, dann wird es abgebrochen.
set_time_limit(300);

function isWindows() {
	if (PHP_OS == 'WINNT') {
		return true;
	}
	else{
		return false;
	}
}
if (isWindows()) {
	define('PROGRAM_PATH', getcwd() . '\\' );
	define('LOCK_FILE', PROGRAM_PATH . '\siedl-backupsyncer.lock');
}
else {
	define('PROGRAM_PATH', getcwd() . '/' );
	define('LOCK_FILE', '/var/lock/siedl-backupsyncer.lock');
}

if (!is_dir(PROGRAM_PATH)) {
	echo "Cannot get program dir!";
	exit(1);
}

// Im PROGRAM_PATH muss eine sync.php, config.php, VERSION existieren, ansonsten befinden wir uns in einem falschen Verzeichnis:
if (!is_readable(PROGRAM_PATH . '/sync.php')) {
	echo "File sync.php does not exists in '". PROGRAM_PATH ."'. Is the program executed in the PROGRAM ROOT?";
}
if (!is_readable(PROGRAM_PATH . '/config.php')) {
	echo "File config.php does not exists in '". PROGRAM_PATH ."'. Is the program executed in the PROGRAM ROOT?";
}
if (!is_readable(PROGRAM_PATH . '/VERSION')) {
	echo "File VERSION does not exists in '". PROGRAM_PATH ."'. Is the program executed in the PROGRAM ROOT?";
}

error_reporting(E_ALL);		// falls die Zeitzone nicht existiert, soll ein NOTICE geschrieben werden..
date_default_timezone_set( @date_default_timezone_get() );
error_reporting(E_ALL ^ E_NOTICE);

include PROGRAM_PATH . "config.php";
include PROGRAM_PATH . "HttpClient.class.php";
include PROGRAM_PATH . "BackupBackendEntry.class.php";
include PROGRAM_PATH . "Tar.class.php";

error_reporting(E_ALL ^ E_NOTICE);

// Wir sperren eine lock- Datei, damit der syncer nicht doppelt ausgefuehrt werden kann..
$fp = fopen (LOCK_FILE, 'w');
if (!flock ($fp, LOCK_EX | LOCK_NB)) {
  die ('Process is already running!');
}

// Wenn ein Fehler aufgetreten ist, wird am Ende des Syncs nicht übermittelt, dass der Sync
// erfolgreich asugeführt wurde.
$errorOccurred = false;

// Settings:
// DON'T CHANGE THIS LINES!!!!
define('PROGRAM_VERSION', str_replace(array("\n", "\r", "\r\n"), "", file_get_contents(PROGRAM_PATH . "VERSION")) );
define('BACKUP_BACKEND', 'Sesam');
define('PATH', './');
define('ARCHITECTURE', array_pop( explode(" ", php_uname()) ) );	// x86_46
								// ACHTUNG: Ich denke, auf Windows wird immer 64bit geliefert!
								// Diese Konstante wird aber derzeit nur f�r Linux verwendet..
define('SYNC_MAX_DAYS', 7);		// Bis zu wie viele Tage soll zurück synchronisiert werden?

$cacheData = '';

if (!isWindows()) {
	if (ARCHITECTURE == 'i686') {
		/*if (!function_exists('sqlite_open')) {
			echo "php5-sqlite module not installed!";
			exit (1);	
		}*/
	}
	else {
		if (!function_exists('pg_pconnect')) {
			echo "php5-pgsql module not installed!";
			exit (1);
		}	
	}
}
else {
	// Windows nutzt nur sqlite
	if (!function_exists('sqlite_open')) {
		echo "php5-sqlite module not installed!";
		exit (1);	
	}
}

if (!function_exists('bzcompress')) {
	echo "php5-bz2 module not installed!";
	exit (1);
}
if (!function_exists('gzcompress')) {
	echo "php5-zlib module not installed!";
	exit (1);
}

// $url:		String url
// $postData:	array(key => value)
function postHttp($url, $postData = array()) {

	$ch = curl_init();
	curl_setopt($ch, CURLOPT_URL,'https://intranet.siedl.net/customer_interface/' . $url);
	curl_setopt($ch, CURLOPT_POST, 1);
	curl_setopt($ch, CURLOPT_HEADER, 0);
	curl_setopt($ch, CURLOPT_RETURNTRANSFER, 1);

	curl_setopt($ch, CURLOPT_POSTFIELDS, http_build_query($postData));

    // Proxy konfigurieren, falls in der Konfiguration definiert:
    if (defined('PROXY_HOSTNAME')) {
    	curl_setopt($ch, CURLOPT_PROXY, PROXY_HOSTNAME);
    }
	if (defined('PROXY_USERNAME') && defined('PROXY_PASSWORD')) {
		    curl_setopt($ch, CURLOPT_PROXYUSERPWD, PROXY_USERNAME .':'. PROXY_PASSWORD);
	}

	$return = curl_exec($ch);
	if ($return === false) {
		die('HTTPS Request failed! Address: https://intranet.siedl.net/customer_interface/' . $url . ', Error message: ' . curl_error($ch));
	}
	curl_close($ch);

	return $return;
}

// Rekursives kopieren...:
function copyr($source, $dest)
{
	// Check for symlinks
	if (is_link($source)) {
		return symlink(readlink($source), $dest);
	}

	// Simple copy for a file
	if (is_file($source)) {
		return copy($source, $dest);
	}

	// Make destination directory
	if (!is_dir($dest)) {
		mkdir($dest);
	}

	// Loop through the folder
	$dir = dir($source);
	while (false !== $entry = $dir->read()) {
		// Skip pointers
		if ($entry == '.' || $entry == '..') {
			continue;
		}

		// Deep copy directories
		copyr("$source/$entry", "$dest/$entry");
	}

	// Clean up
	$dir->close();
	return true;
}

// Protokoll zum Siedl- Server übertragen:
// $type: pre, post oder main
// $entry: Das BackendBackendEntry Objekt.
// $protocolPath: Pfad zum Protokoll..
// $protocolSize: Die Größe des Protokolls..
function sendProtocol($type, $entry, $protocolPath) {
	
	global $cacheData, $errorOccurred;
		
	echo "Check Protocol '{$type}': ";
	
	if ($protocolPath === false) {
		echo "no protocol available.";
		return;
	}
	
	$fp = @fopen($protocolPath, 'r');
	
	if ($fp === false) {
		echo "no protocol available.";
		return;
	}
	
	$filestat = @fstat($fp);
		
	// Wurde Modifikations- Datum vom Protokoll bereits im Cache abgelegt??
	if (array_key_exists('protocol_mod_time_' . $type, $cacheData[$entry->getSoftwareId()] )) {

		// Protokoll wurde bereits synchronisiert, schauen, ob eine Änderung statt gefunden hat...
		if ($cacheData[$entry->getSoftwareId()]['protocol_mod_time_' . $type] == $filestat['mtime']) {
			
			echo "no change... ";
			return;
		}
	}
	
	$cacheData[$entry->getSoftwareId()]['protocol_mod_time_' . $type] = $filestat['mtime'];
		
	// Größe des Protocols:
	$protocolSize = @filesize($protocolPath);
	$bytesPiecesSize = 1024 * 512;		// 512KB wird mit jedem Zug �bertragen...
	$pieces = (int) @ceil($protocolSize / $bytesPiecesSize);
	if ($pieces == 0) {
		echo 'Protocol is empty... ';
	}
	else {
		echo "$pieces parts, start sync: ";
	}
	$success = true;
	for ($i = 1; $i <= $pieces; $i++) {

		// Only sync the first and the last 8 parts...
		$protocolTruncated = false;
		if ($i > 8 && $i < ($pieces - 7)) {
			$i = $pieces - 7;
			$protocolTruncated = true;

			echo "!PROTOCOL TOO LARGE, TRUNCATED! ";
		}

		echo "$i ";

		
		fseek($fp, ((($i - 1) * $bytesPiecesSize) ));
		$protocol = fread($fp, (($i * $bytesPiecesSize) - 1) - ((($i - 1) * $bytesPiecesSize) ) + 1);
		if ($protocolTuncated) {
			$protocol = "===========================\n\n\n       Protocol is too large, truncated......!!!\n\n\n===========================\n" . $protocol;
		}

		if ($i == 1) {
			$doAction = 'resetProtocol';
		}
		else {
			$doAction = 'appendProtocol';
		}
		// Übertragen:
		$response = postHttp('sync_backup_status.php?action=sync_protocol&'. $doAction .'&customerkey='. CUSTOMER_SYNC_KEY .'&customerid='. CUSTOMER_ID
							, array('data' => base64_encode( gzcompress($protocol) ), 'softwareId' => $entry->getSoftwareId() ,
									'protocolType' => $type) );
		if ($response == 'ok') {

		}
		else {
			echo "FATAL ERROR 1: '{$response}'";
			$errorOccurred = true;
			
			// Wenn Fehler aufgetreten, so Protokoll beim nächsten sync nochmals synchroniseren...
			unset($cacheData[$entry->getSoftwareId()]['protocol_mod_time_' . $type]);
		}
	}
}

try {
	
	// Cache- Datei anlegen:
	if (!is_dir(PATH . "cache")) {
		mkdir(PATH . "cache") or die("Could not create folder 'cache'!\n");
	}
	if (!is_file(PATH . "cache/cache")) {
		touch(PATH . "cache/cache");
	}
	
	echo "Check if newer version of this client is available... ";
	$actClientVersion = postHttp('sync_backup_status.php?action=init&customerkey='. CUSTOMER_SYNC_KEY .'&customerid='. CUSTOMER_ID
							, array() );
	$actClientVersion = str_replace(array("\n", "\r", "\r\n"), "", $actClientVersion);
	
	if ($actClientVersion == 'No permission to use this interface') {
		die("\n\nCannot connect interface, permission error! Check customer_id AND customer_interface_key!\n");
	}

	echo "client version '". PROGRAM_VERSION ."', newest version '{$actClientVersion}'\n";
	if ($actClientVersion == PROGRAM_VERSION) {
		echo "Client is up to date...";
	}
	else {
		echo "A newer version is available... Start the update now...\n";
		echo "Download client...\n";
		
		file_put_contents(PROGRAM_PATH . 'cache/client.tar', 
							postHttp('sync_backup_status.php?action=get_newest_client&customerkey='. CUSTOMER_SYNC_KEY .'&customerid='. CUSTOMER_ID, array() ) 
						);


		$tar = new Archive_Tar(PROGRAM_PATH . 'cache/client.tar');
		$tar->extract(PROGRAM_PATH . 'cache');
		
		// Überprüfen ob das Versions- File mit der zu erwartenden Version korrekt ist...?
		$downloadedClientVersion = file_get_contents(PROGRAM_PATH . 'cache/source/VERSION');
		$downloadedClientVersion = str_replace(array("\n", "\r", "\r\n"), "", $downloadedClientVersion);
		if ($downloadedClientVersion != $actClientVersion) {
			echo 'Upgrade process error... Newest version is "'. $actClientVersion .'", downloaded version is "'. $downloadedClientVersion .'"';
			exit(1);
		}
		
		// Und alles überschreiben:
		copyr(PROGRAM_PATH . 'cache/source', PATH);
		
		// Cache Datei löschen:
		@unlink(PROGRAM_PATH . 'cache/cache');
		if (!is_file(PROGRAM_PATH . "cache/cache")) {
			touch(PROGRAM_PATH . "cache/cache");
		}
		
		echo "Upgrade successful! Bye!\n";
		exit;
	}
	echo "\n";
	echo "========================================\n";

	// Wenn Parameter "testSoftware" übergeben wurde, so hier das Skript beenden...
	if (@$argv[1] == 'testSoftware') {
		exit(0);
	}
	

	// Aktuellen Daten aus dem Cache lesen:
	$f = fopen(PROGRAM_PATH . "cache/cache", 'r') or die('Could net rad file "cache/cache"');
	while (!feof($f)) {
    	$cacheData .= fgets($f);
	}
	fclose($f);
	$cacheData = unserialize($cacheData);
	
	if (!is_array($cacheData)) $cacheData = array();
	
	// Alles, was älter als 5 Tage ist, wird aus dem Cache gelöscht...
	foreach($cacheData AS $key => $data) {
		
		if (strtotime($data['obj']->getStartTime()) < (time() - 86400*(SYNC_MAX_DAYS + 1)) ) {
			unset($cacheData[$key]);
		}
	}

	require_once(PROGRAM_PATH . 'backupBackends/BackupBackend' . BACKUP_BACKEND . '.class.php');
	eval('$backend = new BackupBackend' . BACKUP_BACKEND . ';');
	
	$backend->initBackend();

	$backend->reloadNewBackupEntries();
	while($entry = $backend->fetchBackupEntry()) {

		echo "========================================\n";
		echo "JOB: " . $entry->getTaskName() . ", start: " . $entry->getStartTime() . " | SOFTWARE ID: " . $entry->getSoftwareId() . "\n";

		/**
		 * Senden des JOBS:
		 * 
		 * Wir überprüfen, ob der JOB bereits im Cache angelegt wurde, wenn nicht, so kann dieser Job synchronisiert werden...
		 * Wurde dieser bereits im Cache angelegt, so schauen wir, ob sich Daten des Jobs geändert haben, wenn ja, so synchronisieren
		 * wir...
		 */
	
		// Begin Abschnitt Job senden:
		do {
			
			echo "Check Job... ";
			
			// Wurde er bereits im Cache angelegt??
			$jobSyncFailed = false;
			if (array_key_exists($entry->getSoftwareId(), $cacheData)) {

				// Job wurde bereits angelegt, schauen, ob sich Daten gegenüber zum Cache geändert haben...
				$checkMethods = array('getTaskName', 'getSoftwareId', 'getStartTime', 'getEndTime', 'getDate', 
										'getMessage', 'getType', 'getState', 'getGroupSoftwareId', 'getSource', 'getExclude',
										'getDataSize', 'getJobType', 'getThroughput');
				$dataChanged = false;
				foreach($checkMethods AS $check) {
				
					if ($entry->$check() != $cacheData[$entry->getSoftwareId()]['obj']->$check()) {
						$dataChanged = true;

						break;
					}
				}
				
				// Wenn sich Daten zum Cache geändert haben, so fortfahren, ansonsten können wir das synchronisneren des Jobs überprüfen..
				if (!$dataChanged) {
					echo "no change available ";
					break;
				}
			}
			else {
				echo "new job... ";
			}
			
			echo "start sync... ";
			
			// Daten serialisieren & übertragen:
			$data = base64_encode( bzcompress( serialize( $entry ) ) );
			$response = postHttp('sync_backup_status.php?action=sync_job&customerkey='. CUSTOMER_SYNC_KEY .'&customerid='. CUSTOMER_ID
									, array('data' => $data) );
			
			// Http Respone muss ein "ok" zurückgeben, ansonsten ist was schief gelaufen...
			if ($response != 'ok') {
				echo "\nFATAL ERROR 2: \"$response\"";
				$jobSyncFailed = true;
				$errorOccurred = true;
			}
			else {
				
				echo "sync ok";
				$cacheData[$entry->getSoftwareId()]['obj'] = $entry;
			}
			
			break;
			
		} while(false);	// ENDE Abschnitt Job senden
		
		echo "\n";

		if ($jobSyncFailed) continue;
		
		
		/**
		 * Senden des Protokolls:
		 * 
		 * Wir überprüfen, ob im Cache bereits ein Datum abgelegt wurde, wann das Protkoll das letzte mal modifiziert wurde...
		 * Ist keines vorhanden, so syncen wir das Protokoll, ansonsten schauen wir, ob sich das Modifizierungsdatum verändert hat...
		 */
		
		// Begin Abschnitt Protokoll senden:
		sendProtocol('pre', $entry, $entry->getPreProtocolPath());
		echo "\n";
		sendProtocol('main', $entry, $entry->getMainProtocolPath());
		echo "\n";
		sendProtocol('post', $entry, $entry->getPostProtocolPath());
		echo "\n";
		
	}
	
	$backend->finishedBackend();

	// Cache mit allen Änderungen schreiben:
	file_put_contents(PROGRAM_PATH . "cache/cache", serialize($cacheData) ) or die("Could not write cache!\n");
	
	// BackupMonitor mitteilen, dass Synchronisierung beendet wurde:
	echo 'Sync hello... ';
	if ($errorOccurred) {
		echo "Sync of some jobs failed, i do net send the hello command. Search for 'FATAL ERROR'.";
	}
	else {
		
		$response = postHttp('sync_backup_status.php?action=sync_hello&customerkey='. CUSTOMER_SYNC_KEY .'&customerid='. CUSTOMER_ID, array('sync_version' => PROGRAM_VERSION));
		if ($response != 'ok') {
			echo "error: '$response'";
		}
		else {
			echo "ok";
		}
	}
	echo "\n";
	
}
catch (Exception $e) {
	echo "Allgemeiner Fehler:\n";
	echo $e->getMessage();
}
?>
