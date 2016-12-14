<?php
	class DbResult {
		
        /**
         * Query Result
         *
         * @var resource
         */
        protected $_result = null;
        
        /**
         * Konstruktor, übergibt der Klasse den Result
         *
         * @param $result resource
         */
        public function __construct($result) {

            $this->_result = $result;
        }
		
		public function fetch($column = null) {
			
			if (!$this->_result) {
				throw new DbFetchException();
			}

			if (is_null($column)) {
				return pg_fetch_assoc($this->_result);
			}
			else {
				$row = pg_fetch_assoc($this->_result);
				return $row[$column];
			}
		}
		
        /**
         * Holt sämtliche Abfragewerte
         *
         * @param string $key: Spaltenname, welcher als Key benutzt werden soll, zB. "id".
         * @param array $arrayMerge: Array, an welchem die Ergebnisse der Datenbankanfrage angehängt werden.
         * @return array
         */
        public function fetchAll($key = null, $arrayMerge = array()) {

        	if (is_null($key)) {
        		while ($row = $this->fetch()) $arrayMerge[] = $row;
        	}
        	else {
        		while ($row = $this->fetch()) $arrayMerge[$row[$key]] = $row;
        	}
            
            return $arrayMerge;
        }
		
		public function numRows() {
			return pg_num_rows($this->_result);
		}
		
		public function affectedRows() {
		
			return @pg_affected_rows($this->_result);
		}
		
		public function getResult() {
			return $this->_result;
		}
		
		private function _dbFetchException() {
			throw new DbFetchException();
		}
	}

?>