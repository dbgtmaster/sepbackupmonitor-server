<?php
$CONFIG = array();
$configLines = gzfile('/usr/local/etc/sepbackupmonitor/server.ini');
$lastSection = '';
$cntLine = 0;
foreach($configLines AS $line) {
	$cntLine++;
	$line = trim($line);
	if (empty($line)) continue;

	// Ignore commments
	if (substr($line, 0, 1) == '#' || substr($line, 0, 1) == '/') {
		continue;
	}

 	if (substr($line, 0, 1) == '[') {
		$lastSection = array_shift(explode(']', array_pop(explode('[', $line) )));
		continue;
	}

	if ($lastSection == '') {
		die('ERR: Config is invalid! line '. $cntLine .' #1');
	}

	// Key starts at first =
	$ex = explode('=', $line);
	if (count($ex) < 2) {
		die('ERR: Config is invalid! line '. $cntLine .' #2');
	}
	$key = trim($ex[0]);
	unset($ex[0]);
	$value = trim( implode('=', $ex) );

	$CONFIG[$lastSection][$key] = $value;
}