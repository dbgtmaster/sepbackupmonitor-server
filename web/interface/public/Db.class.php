<?php

    class Db {

		/**
		 * Datenbankverbindung
		 *
		 * @var resource
		 */
        private $_connection    = null;
        
        /**
         * Anzahl an Queries die pro Verbindung benutzt werden
         *
         * @var integer
         */
        private $_queries       = 0;
        
        /**
         * Läuft gerade eine Transaktion?
         *
         * @var bool
         */
        private $_inTransaction = false;
        
        /**
         * Speichert die letzte Datenbankabfrage ab.
         * 
         * @var Result
         */
        private $_lastResult 		= null;
        
        /**
         * Sollen alle SQLs mitgeloggt werden?
         *
         * @var bool
         */
        private $_doLogging 		= false;
        
        /**
         * Dürfen Transaktionen gestartet / gestoppt werden? Wird für den Daemon benötigt, so dass während eines Events bei der Abarbeitung die Transaktion durch eine Methode nicht beendet
         * werden darf.
         *
         * @var bool
         */
        private $_canChangeTransactionStatus = true;
        
        /**
         * Stellt eine Verbindung zur Datenbank her
         *
         * @param $host string
         * @param $db_name string
         * ....
         * @param bool $diebyConnectError: Gibt eine Fehlermeldung aus, wenn Datenbankverbindung nicht aufgebaut werden kann und bricht das Skript ab.
         */
        public function __construct($host, $user, $password, $database, $dieByConnectError = true) {

        	// Verbindungsparamter
        	$connectString = 'host='. $host .' dbname='. $database .' user='. $user .' password='.$password;
		
        	if (!defined('DATABASE_RETRY_CONNECT')) {
	        	$this->_connection = pg_pconnect($connectString, PGSQL_CONNECT_FORCE_NEW) or
				                die('Kann nicht zur Datenbank verbinden.');
        	}
        	else {
        		while (true) {
        			Cl::out('Connect to database...');
	        		$this->_connection = @pg_pconnect($connectString, PGSQL_CONNECT_FORCE_NEW);
	       			if ($this->_connection) break;
	        		sleep(1);
        		}
        	}
        }
        
        /**
         * Beendet die Verbindung zur Datenbank.
         */
        public function __destruct() {
        	@pg_close($this->_connection);
        }
        
		/**
         * Trennt die zuvor mit connect aufgebaute Verbindung
         */
        public function disconnect() {
        	@pg_close($this->_connection);
        }
        
        /**
         * Prüft ob ein Fehler aufgetreten ist
         *
         * @return boolean
         */
        public function isError() {
        	
            $error = pg_errormessage();
            return !empty($error);
        }
        
        /**
         * Gibt die Verbindungsressource zurück.
         *
         * @return ressource
         */
        public function getConnection() {
        	return $this->_connection;
        }
        
        /**
         * Führt eine Sqls auf und gibt das Result Object zurück.
         *
         * @param string $query
         * @param bool $showError: Soll eine Exception im Fehlerfall geworfen werden?
         * @return mixed
         */
        public function query($query, $showError = true) {

        		
        		/**
        		 * SQLs werden mitgeloggt.
        		 */
        		
			$result = @pg_query($this->_connection, $query);

			//echo $runtime . "ms - $query\n";
			
            if($result) {
            	$this->_lastResult = $result;
                return new DbResult($result);
            }
            elseif($showError) {
            	$error = $this->getLastError();

                throw new SQLException($error, $query);
            }
            else {
            	$this->_lastResult = false;
            }
        }
        
		
        /**
         * Kann das Loggen der Datenbankabfragen aktivieren / deaktiveren. Wird es deaktiviert und in der Konfiguration ist es aktiviert,
         * wird nichts geloggt.
         *
         * @param bool $bool: true => Es wird geloggt, false => NEIN
         */
        public function doLogging($bool) {
        	
        	$this->_doLogging = $bool;
        }
        
        /**
         * Holt Ergebnisse aus einem ResultObject. Diese Methode wird nur dafür benötigt, damit beim aktuellen Code nichts abgeändert
         * werden muss. Daher sollte diese Methode nicht mehr verwendet werden, sondern $result->fetch().
         *
         * @param unknown_type $resultObject
         */
        public function fetch($resultObject) {

        	return $resultObject->fetch();
        }
        
		/**
		 * Beginnt eine Transaction
		 */
		public function startTransaction() {
	
			if (! $this->_canChangeTransactionStatus) return;
			
			$this->query('BEGIN');
			
			$this->_inTransaction = true;
		}
		
		/**
		 * Können Transaktionen gestartet / gestoppt werden?
		 *
		 * @param bool $bool => true -> ja, false -> nein
		 */
		public function canChangeTransactionStatus($bool) {
			
			$this->_canChangeTransactionStatus = $bool;
		}
		
		/**
		 * Beendet eine Transaction und leert alle Transaction Querys
		 */
		public function rollback() {
			
			if (! $this->_canChangeTransactionStatus) return;
			
			$this->query('ROLLBACK');
			
			$this->_inTransaction = false;
		}
		
		/**
		 * Schließt eine Transaction ab, indem es die einzelnen Querys bearbeitet
		 */
		public function commit() {
			
			if (! $this->_canChangeTransactionStatus) return;
			
			$this->query('COMMIT');
			
			$this->_inTransaction = false;
		}
		
        /**
         * Läuft gerade eine Transaction?
         *
         * @return bool true, wenn gerade eine Transaktion läuft, ansonsten false
         */
        public function isInTransaction() {
        	return $this->_inTransaction;
        }
        
        public function numRows($result) {
        	return pg_num_rows($result->getResult());
        }
        
         /**
         * Gibt die Anzahl der veränderten Datensätze zurück. Sollte das letzte Query unbuffered
         * ausgeführt worden sein, so liefert diese Funktion kein Ergebnis.
         *
         * @return int: Anzahl geänderter Datensätze
         */
        public function affectedRows($result='') {
        	
        	// Wenn kein $result übergeben wurde, so den letzten verwenden.
        	if (empty($result)) {
        		$result = $this->_lastResult;
        		
        		// Wenn bei der letzten SQL ein Fehler aufgetreten ist, dann "0" zurückgeben:
        		if ($result === false) {
        			return 0;
        		}
        	}
        	else {
        		$result = $result->getResult();
        	}
        	
            return pg_affected_rows($result);
        }
        
        /**
         * Ermittelt die zuletzt automatisch eingefügte ID! Sollte ein Query
         * unbuffered ausgeführt worden sein, so liefert diese Funktion zu dieser letzten
         * Query keine ID!!!
         *
         * @param string $seq: name der Sequenz, von welcher die letzte ID ermittelt werden soll. Im Normalfall ist es [tabellenname]_id_seq.
         * @param bool $modifySequenz: Standardmäßig true, dann wird _id_seq an den Sequenznamen geändert, bei false bleibt der Sequenzname unverändert.
         * 
         * @return int   Letzt einfgefügte ID
         */
        public function lastId($seq, $modifySequenz=true) {
        	
        	if ($modifySequenz) {
        		$seq .= "_id_seq";
        	}
        	
            $id = $this->query("SELECT currval('$seq') AS currval")->fetch('currval');
            return $id;
        }
        
        /**
         * Gibt die Anzahl der abgearbeiteten Querys zurück
         *
         * @return integer
         */
        public function queryCount() {
            return $this->_queries;
        }

        /**
         * "Entschärft" alle gefährlichen Zeichen in einem Wert
         *
         * @param $param string
         * @return string
         */
    	public function escape($param) {
        	
        	return pg_escape_string($param);
        }
        
   		public function escape_bytea($param) {
        	
        	return pg_escape_bytea($param);
        }
        
        public function escapeBytea($data) {
  	
        	return pg_escape_bytea($data);
        }
        
        public function unescapeBytea($data) {
        	
        	return pg_unescape_bytea($data);
        }
        
        /**
         * Gibt die zuletzt eingetragene ID zurück
         *
         * @param string $seq: Tabellenname, von der die letzt eingefügte ID ermittelt werden soll.
         * @param bool $modfiy_sequenz: Soll an die $seq "_id_seq" angehängt werden? Standardmäßig ist dieses erforderlich.
         * @return integer
         */
        public function getLastId($seq, $modifySequenz=true) {
        	
        	if ($modifySequenz) {
        		$seq .= "_id_seq";
        	}
        	
            $id = $this->query("SELECT currval('$seq') AS currval")->fetch('currval');
            return $id;
        }
        
        /**
         * Gibt den zuletzt aufgetretenen Fehler zurück
         *
         * @return string
         */
        public function getLastError() {
			return pg_errormessage($this->_connection);
        }
        
        /**
         * Baut eine Insert Abfrage zusammen und führt diese aus.
         *
         * @param array $values
         * @param string $table
         * @param array $dontEscape: Spalten, welche nicht escaped werden sollten.
         */
        public function insert(array $values, $table, $dontEscape = array()) {
        	
        	$query = 'INSERT INTO ' . $table . ' (';
		
			$cols = array();
			foreach($values AS $colname => $value) {
				
				$cols[] = $colname;
				
				// Werte escapen:
				if (!in_array($colname, $dontEscape)) {
					$keys[$colname] = '\'' . $this->escape($value) . '\'';
				}
				else {
					$keys[$colname] = $value;
				}
			}
			$query .= implode(",", $cols);
			
			$query .= ') VALUES (';
			
			$query .= implode(',', $keys);
			
			$query .= ')';
			
			return $this->query($query);
        }
        
        /**
         * Baut eine Update Anfrage zusammen und führt diese aus:
         *
         * @param array $update
         * @param string $table
         * @param array $where
         */
        public function update(array $update, $table, $where = array(), $whereEscape = true) {
        	
        	$query = 'UPDATE ' . $table . ' SET ';
        	
        	$cols = array();
			foreach ($update as $key => $value) {

				$cols[] = $key . ' = \'' . $this->escape($value) . '\'';
			}

			$query .= implode(',', $cols);
			
			if (is_string($where)) {
				$query .= 'WHERE ' . $where;
			}
			elseif (count($where) > 0) {
				
				$query .= " WHERE ";
				$first = true;
				foreach($where AS $name => $value) {
					
					if ($first) {
						$first = false;
					}
					else {
						$query .= " AND ";
					}
					
					if (is_numeric($value)) {
						$query .= $name . '=' . $value;
					}
					elseif (is_string($value)) {
						$query .= $name . "= '". $this->escape($value) ."'";
					}
					elseif (is_bool($value)) {
						$query .= $name . '=';
						if ($value) {
							$query .= 'true';
						}
						else {
							$query .= 'false';
						}
					}
					else {
						$query .= $name . " = '". $this->escape($value) ."'";
					}
					
				}
			}

			return $this->query($query);
        }
        
        /**
         * Kopiert bestimmte Einträge in einer Tabelle und fügt diese in einer anderen ein.
         *
         * @param string $fromTable: Von welcher Tabelle sollen die Einträge kopiert werden?
         * @param string $toTable: In welcher Tabelle sollen diese Einträge eingefügt werden?
         * @param array $columns: Welche Spalten sollen kopiert werden und in welche Zielspalte eingefügt werden?
         * 							Beispiel: new Array('from_column' => 'to_column', 'id' => 'orig_id'
         * 							from_column ist die Spalte der $fromTabelle, der Inhalt der Spalte wird in der Tabelle $toTable eingefügt. Das gleiche gilt
         * 							natürlich auch für id => orig_id.
         * @param array $where: Bedingung. Wenn diese erfüllt wird, wird diese Zeile der Tabelle $fromTable kopiert. Werte des Arrays werden escaped!
         * 							Beispiel: array('userid' => 12). 
         * 
         * @return DbReult Object
         */
        public function copyRows($fromTable, $toTable, $columns, $where = array()) {
        	
        	// Query zusammenbauen:
        	$query = 'INSERT INTO ' . $toTable . ' ('. implode(',', $columns) .') SELECT '. implode(',', array_keys($columns)) . ' FROM ' . $fromTable;
        	
        	// Falls es auch noch Bedingungen gibt, dann auch diese anfügen:
        	if (count($where) > 0) {
        		
        		$query .= ' WHERE ';
        	}
        	
        	$first = true;
        	foreach($where AS $key => $value) {
        		
        		$query .= ' ' . $key . '=\'' . $this->escape($value) . '\'';
        		
        		if ($first) {
        			$first = false;
        			continue;
        		}
        		
        		$query .= ' AND ';
        	}
        	
        	// Query ausführen und DbResult Objekt zurückgeben:
        	return $this->query($query);
        }
    }

?>
