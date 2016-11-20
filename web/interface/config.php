<?php
$CONFIG = array();
$configLines = gzfile('cat /usr/local/etc/sepbackupmonitor/server.ini');
$lastSection = '';
foreach($configLines AS $line) {

	$line = trim($line);
	if (empty($line)) continue;

	// Ignore commments
	// TODO...

	if (substr($line, 0, 1) == '[') {
		$lastSection = array_pop(explode(']', array_shift(explode('[', $line) ));
			continue;
	}

	if ($lastSection == '') {
		die('ERR: Config is invalid! #1');
	}
}