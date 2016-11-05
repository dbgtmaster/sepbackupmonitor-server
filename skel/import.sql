--
-- PostgreSQL database dump
--

SET statement_timeout = 0;
SET client_encoding = 'UTF8';
SET standard_conforming_strings = off;
SET check_function_bodies = false;
SET client_min_messages = warning;
SET escape_string_warning = off;

SET search_path = public, pg_catalog;

SET default_tablespace = '';

SET default_with_oids = false;

--
-- Name: backup_customer_groups; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE backup_customer_groups (
    id integer NOT NULL,
    name character varying,
    auto_expand boolean DEFAULT false,
    description text,
    "position" integer
);


ALTER TABLE public.backup_customer_groups OWNER TO postgres;

--
-- Name: backup_customer_groups_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE backup_customer_groups_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MAXVALUE
    NO MINVALUE
    CACHE 1;


ALTER TABLE public.backup_customer_groups_id_seq OWNER TO postgres;

--
-- Name: backup_customer_groups_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE backup_customer_groups_id_seq OWNED BY backup_customer_groups.id;


--
-- Name: backup_jobs_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE backup_jobs_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MAXVALUE
    NO MINVALUE
    CACHE 1;


ALTER TABLE public.backup_jobs_id_seq OWNER TO postgres;

--
-- Name: backup_jobs; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE backup_jobs (
    id integer DEFAULT nextval('backup_jobs_id_seq'::regclass) NOT NULL,
    customer_id integer NOT NULL,
    software_id character varying,
    taskname character varying,
    state character varying,
    msg character varying,
    start_time timestamp without time zone,
    end_time timestamp without time zone,
    date timestamp without time zone,
    type integer DEFAULT 0,
    group_softwareid character varying,
    backup_software character varying,
    exclude_list text,
    source_list text,
    data_size bigint,
    job_type character varying,
    throughput bigint,
    client_name character varying,
    drive_name character varying,
    drive_type character varying,
    drive_client_name character varying,
    job_state_changed boolean DEFAULT true
);


ALTER TABLE public.backup_jobs OWNER TO postgres;

--
-- Name: COLUMN backup_jobs.software_id; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN backup_jobs.software_id IS 'ID des Jobs in der Backupsoftware (SEP,..)';


--
-- Name: COLUMN backup_jobs.type; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN backup_jobs.type IS '0 -> job, 1 -> group';


--
-- Name: COLUMN backup_jobs.group_softwareid; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN backup_jobs.group_softwareid IS 'BackupSoftware ID der Gruppe, in welcher sich dieser Job befindet.';


--
-- Name: COLUMN backup_jobs.backup_software; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN backup_jobs.backup_software IS 'Name der verwendeten Backup- Software';


--
-- Name: COLUMN backup_jobs.data_size; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN backup_jobs.data_size IS 'Gesicherte Daten in Bytes';


--
-- Name: COLUMN backup_jobs.throughput; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN backup_jobs.throughput IS 'Datendurchsatz in MB/h';


--
-- Name: COLUMN backup_jobs.job_state_changed; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN backup_jobs.job_state_changed IS 'Wenn sich der Status eines Jobs geändert bzw. ein neuer Job hinzugefügt wurde, so ist der Wert true. Benötigt intranet_server BackupMonitorDoCacheThread';


--
-- Name: backup_jobs_log; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE backup_jobs_log (
    id integer NOT NULL,
    customerid integer,
    taskname character varying(400),
    userid integer,
    log text,
    date timestamp without time zone DEFAULT now()
);


ALTER TABLE public.backup_jobs_log OWNER TO postgres;

--
-- Name: backup_jobs_log_ID_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE "backup_jobs_log_ID_seq"
    START WITH 1
    INCREMENT BY 1
    NO MAXVALUE
    NO MINVALUE
    CACHE 1;


ALTER TABLE public."backup_jobs_log_ID_seq" OWNER TO postgres;

--
-- Name: backup_jobs_log_ID_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE "backup_jobs_log_ID_seq" OWNED BY backup_jobs_log.id;


--
-- Name: backup_jobs_protocols; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE backup_jobs_protocols (
    id integer NOT NULL,
    backup_job_id integer,
    protocol_uncompressed_size integer,
    protocol text,
    protocol_type character varying
);


ALTER TABLE public.backup_jobs_protocols OWNER TO postgres;

--
-- Name: backup_jobs_protocols_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE backup_jobs_protocols_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MAXVALUE
    NO MINVALUE
    CACHE 1;


ALTER TABLE public.backup_jobs_protocols_id_seq OWNER TO postgres;

--
-- Name: backup_jobs_protocols_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE backup_jobs_protocols_id_seq OWNED BY backup_jobs_protocols.id;


--
-- Name: customers; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE customers (
    sync_id integer,
    customer_name character varying,
    address character varying,
    plz integer,
    city character varying,
    main_email character varying,
    main_email_domain character varying,
    backup_enabled boolean DEFAULT false,
    backup_interface_key character varying(32),
    backup_prospective_jobs_mon integer DEFAULT 0,
    backup_prospective_jobs_tue integer DEFAULT 0,
    backup_prospective_jobs_wed integer DEFAULT 0,
    backup_prospective_jobs_thu integer DEFAULT 0,
    backup_prospective_jobs_fri integer DEFAULT 0,
    backup_prospective_jobs_sat integer DEFAULT 0,
    backup_prospective_jobs_sun integer DEFAULT 0,
    backup_last_hello integer DEFAULT 0,
    backup_sync_client_version character varying,
    backup_group integer DEFAULT 0,
    id integer NOT NULL,
    backup_disaster_last_update bigint DEFAULT 0
);


ALTER TABLE public.customers OWNER TO postgres;

--
-- Name: COLUMN customers.backup_last_hello; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN customers.backup_last_hello IS 'Zeitstempel, wann sich Backup- Server sich das letzte mal gemeldet hat.';


--
-- Name: COLUMN customers.backup_disaster_last_update; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON COLUMN customers.backup_disaster_last_update IS 'Wann zuletzt die Disaster- Meldung synchronisiert wurde. (=Timestamp der Datei var/work/disaster.txt)';


--
-- Name: customers_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE customers_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MAXVALUE
    NO MINVALUE
    CACHE 1;


ALTER TABLE public.customers_id_seq OWNER TO postgres;

--
-- Name: customers_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE customers_id_seq OWNED BY customers.id;


--
-- Name: groups_memberships; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE groups_memberships (
    user_group integer NOT NULL,
    system_group integer NOT NULL
);


ALTER TABLE public.groups_memberships OWNER TO postgres;

--
-- Name: groups_sequence; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE groups_sequence
    START WITH 1
    INCREMENT BY 1
    NO MAXVALUE
    NO MINVALUE
    CACHE 1;


ALTER TABLE public.groups_sequence OWNER TO postgres;

--
-- Name: system_groups; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE system_groups (
    id integer NOT NULL,
    name character varying NOT NULL,
    description character varying
);


ALTER TABLE public.system_groups OWNER TO postgres;

--
-- Name: system_groups_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE system_groups_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MAXVALUE
    NO MINVALUE
    CACHE 1;


ALTER TABLE public.system_groups_id_seq OWNER TO postgres;

--
-- Name: system_groups_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE system_groups_id_seq OWNED BY system_groups.id;


--
-- Name: tcp_actions; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE tcp_actions (
    group_name character varying NOT NULL,
    action_name character varying NOT NULL,
    id integer NOT NULL,
    system_group_id integer DEFAULT (-1) NOT NULL
);


ALTER TABLE public.tcp_actions OWNER TO postgres;

--
-- Name: TABLE tcp_actions; Type: COMMENT; Schema: public; Owner: postgres
--

COMMENT ON TABLE tcp_actions IS 'Liste aller Aktionen, die der TCP Server ausführen kann...';


--
-- Name: tcp_actions_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE tcp_actions_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MAXVALUE
    NO MINVALUE
    CACHE 1;


ALTER TABLE public.tcp_actions_id_seq OWNER TO postgres;

--
-- Name: tcp_actions_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE tcp_actions_id_seq OWNED BY tcp_actions.id;


--
-- Name: user_groups; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE user_groups (
    id integer NOT NULL,
    name character varying NOT NULL,
    locked boolean DEFAULT false,
    ldapfilter1_enabled boolean DEFAULT false NOT NULL,
    ldapfilter2_enabled boolean DEFAULT false NOT NULL,
    ldapfilter3_enabled boolean DEFAULT false NOT NULL,
    ldapfilter1_scope smallint DEFAULT 0 NOT NULL,
    ldapfilter2_scope smallint DEFAULT 0 NOT NULL,
    ldapfilter3_scope smallint DEFAULT 0 NOT NULL,
    ldapfilter1_dn character varying,
    ldapfilter2_dn character varying,
    ldapfilter3_dn character varying,
    ldapfilter1_filter character varying,
    ldapfilter2_filter character varying,
    ldapfilter3_filter character varying
);


ALTER TABLE public.user_groups OWNER TO postgres;

--
-- Name: user_memberships_to_groups; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE user_memberships_to_groups (
    user_id integer NOT NULL,
    group_id integer NOT NULL
);


ALTER TABLE public.user_memberships_to_groups OWNER TO postgres;

--
-- Name: user_memberships_to_systemgroups; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE user_memberships_to_systemgroups (
    user_id integer NOT NULL,
    systemgroup_id integer NOT NULL
);


ALTER TABLE public.user_memberships_to_systemgroups OWNER TO postgres;

--
-- Name: users_sequence; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE users_sequence
    START WITH 1
    INCREMENT BY 1
    NO MAXVALUE
    NO MINVALUE
    CACHE 1;


ALTER TABLE public.users_sequence OWNER TO postgres;

--
-- Name: users; Type: TABLE; Schema: public; Owner: postgres; Tablespace: 
--

CREATE TABLE users (
    id integer DEFAULT nextval('users_sequence'::regclass) NOT NULL,
    username character varying NOT NULL,
    surname character varying,
    givenname character varying,
    displayname character varying,
    mail character varying,
    ldap_id character varying NOT NULL
);


ALTER TABLE public.users OWNER TO postgres;

--
-- Name: users_groups_id_seq; Type: SEQUENCE; Schema: public; Owner: postgres
--

CREATE SEQUENCE users_groups_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MAXVALUE
    NO MINVALUE
    CACHE 1;


ALTER TABLE public.users_groups_id_seq OWNER TO postgres;

--
-- Name: users_groups_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: postgres
--

ALTER SEQUENCE users_groups_id_seq OWNED BY user_groups.id;


--
-- Name: id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY backup_customer_groups ALTER COLUMN id SET DEFAULT nextval('backup_customer_groups_id_seq'::regclass);


--
-- Name: id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY backup_jobs_log ALTER COLUMN id SET DEFAULT nextval('"backup_jobs_log_ID_seq"'::regclass);


--
-- Name: id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY backup_jobs_protocols ALTER COLUMN id SET DEFAULT nextval('backup_jobs_protocols_id_seq'::regclass);


--
-- Name: id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY customers ALTER COLUMN id SET DEFAULT nextval('customers_id_seq'::regclass);


--
-- Name: id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY notifications ALTER COLUMN id SET DEFAULT nextval('notifications_id_seq'::regclass);


--
-- Name: id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY system_groups ALTER COLUMN id SET DEFAULT nextval('system_groups_id_seq'::regclass);


--
-- Name: id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY tcp_actions ALTER COLUMN id SET DEFAULT nextval('tcp_actions_id_seq'::regclass);


--
-- Name: id; Type: DEFAULT; Schema: public; Owner: postgres
--

ALTER TABLE ONLY user_groups ALTER COLUMN id SET DEFAULT nextval('users_groups_id_seq'::regclass);


--
-- Name: system_groups_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY system_groups
    ADD CONSTRAINT system_groups_pkey PRIMARY KEY (id);


--
-- Name: users_groups_pkey; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY user_groups
    ADD CONSTRAINT users_groups_pkey PRIMARY KEY (id);


--
-- Name: users_primarykey; Type: CONSTRAINT; Schema: public; Owner: postgres; Tablespace: 
--

ALTER TABLE ONLY users
    ADD CONSTRAINT users_primarykey PRIMARY KEY (id);


--
-- Name: backup_customer_groups_id; Type: INDEX; Schema: public; Owner: postgres; Tablespace: 
--

CREATE UNIQUE INDEX backup_customer_groups_id ON backup_customer_groups USING btree (id);


--
-- Name: backup_jobs_date; Type: INDEX; Schema: public; Owner: postgres; Tablespace: 
--

CREATE INDEX backup_jobs_date ON backup_jobs USING btree (date);


--
-- Name: backup_jobs_id; Type: INDEX; Schema: public; Owner: postgres; Tablespace: 
--

CREATE UNIQUE INDEX backup_jobs_id ON backup_jobs USING btree (id);


--
-- Name: backup_jobs_job_state_changed; Type: INDEX; Schema: public; Owner: postgres; Tablespace: 
--

CREATE INDEX backup_jobs_job_state_changed ON backup_jobs USING btree (job_state_changed);


--
-- Name: backup_jobs_protocols_id; Type: INDEX; Schema: public; Owner: postgres; Tablespace: 
--

CREATE UNIQUE INDEX backup_jobs_protocols_id ON backup_jobs_protocols USING btree (id);


--
-- Name: backup_jobs_protocols_job_id; Type: INDEX; Schema: public; Owner: postgres; Tablespace: 
--

CREATE INDEX backup_jobs_protocols_job_id ON backup_jobs_protocols USING btree (backup_job_id);


--
-- Name: backup_jobs_software_ingroup; Type: INDEX; Schema: public; Owner: postgres; Tablespace: 
--

CREATE INDEX backup_jobs_software_ingroup ON backup_jobs USING btree (group_softwareid);


--
-- Name: backup_lobs_log_id; Type: INDEX; Schema: public; Owner: postgres; Tablespace: 
--

CREATE UNIQUE INDEX backup_lobs_log_id ON backup_jobs_log USING btree (id);


--
-- Name: customers_customer_name; Type: INDEX; Schema: public; Owner: postgres; Tablespace: 
--

CREATE INDEX customers_customer_name ON customers USING btree (lower((customer_name)::text));


--
-- Name: customers_id; Type: INDEX; Schema: public; Owner: postgres; Tablespace: 
--

CREATE UNIQUE INDEX customers_id ON customers USING btree (sync_id);


--
-- Name: grouo_action; Type: INDEX; Schema: public; Owner: postgres; Tablespace: 
--

CREATE UNIQUE INDEX grouo_action ON tcp_actions USING btree (group_name, action_name);


--
-- Name: groups_memberships_id; Type: INDEX; Schema: public; Owner: postgres; Tablespace: 
--

CREATE UNIQUE INDEX groups_memberships_id ON groups_memberships USING btree (user_group, system_group);


--
-- Name: user_memberships_to_groups_ids; Type: INDEX; Schema: public; Owner: postgres; Tablespace: 
--

CREATE UNIQUE INDEX user_memberships_to_groups_ids ON user_memberships_to_groups USING btree (user_id, group_id);


--
-- Name: user_memberships_to_systemgroups_id; Type: INDEX; Schema: public; Owner: postgres; Tablespace: 
--

CREATE UNIQUE INDEX user_memberships_to_systemgroups_id ON user_memberships_to_systemgroups USING btree (user_id, systemgroup_id);


--
-- Name: users_id; Type: INDEX; Schema: public; Owner: postgres; Tablespace: 
--

CREATE UNIQUE INDEX users_id ON users USING btree (id);


--
-- Name: users_ldap_id; Type: INDEX; Schema: public; Owner: postgres; Tablespace: 
--

CREATE UNIQUE INDEX users_ldap_id ON users USING btree (ldap_id);


--
-- Name: users_lowerUsername; Type: INDEX; Schema: public; Owner: postgres; Tablespace: 
--

CREATE UNIQUE INDEX "users_lowerUsername" ON users USING btree (lower((username)::text));


--
-- Name: users_username; Type: INDEX; Schema: public; Owner: postgres; Tablespace: 
--

CREATE UNIQUE INDEX users_username ON users USING btree (username);


--
-- Name: public; Type: ACL; Schema: -; Owner: postgres
--

REVOKE ALL ON SCHEMA public FROM PUBLIC;
REVOKE ALL ON SCHEMA public FROM postgres;
GRANT ALL ON SCHEMA public TO postgres;
GRANT ALL ON SCHEMA public TO PUBLIC;

--
-- Name: tcp_actions_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('tcp_actions_id_seq', 35, true);


--
-- Data for Name: tcp_actions; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY tcp_actions (group_name, action_name, id, system_group_id) FROM stdin;
main    userLogin   2   1
main    connectTcp  4   1
system  getAllSystemGroups  6   2
system  saveTcpCommandsPermissions  7   2
loggedInUser    getUserColumn   1   3
main    serverVersion   3   3
system  getAllTcpCommands   5   2
backupMonitor   getCustomers    15  4
customers   getCustomer 17  3
customers   modCustomer 18  3
customers   generateNewBackupMonitorKey 19  3
backupMonitor   getJobs 20  4
backupMonitor   getJob  21  4
backupMonitor   getJobProtocol  22  4
customers   search  16  3
notifications   create  27  -1
backupMonitor   getBackupStateOfDay 25  4
system  serverStatistics    23  2
backupMonitor   getCustomerGroups   28  4
customers   customerEditable    31  3
customers   removeCustomer  30  3
backupMonitor   customerGroupMod    29  4
backupMonitor   customerGroupDelete 32  4
notifications   getLastMessages 26  1
backupMonitor   getCustomerInterfaceAddress 33  4
backupMonitor   getLog  34  4
backupMonitor   createLog   35  4
administration  testLDAPGroupMemberships    8   5
administration  userGroupMod    9   5
administration  getAllUserGroups    10  5
administration  getUserGroup    11  5
administration  modGroupToSystemgroupsMemberships   12  5
administration  userGroupDelete 13  5
administration  userGroupToSystemGroups 14  5
loggedInUser    hasTcpPermission    24  3
\.


--
-- Name: system_groups_id_seq; Type: SEQUENCE SET; Schema: public; Owner: postgres
--

SELECT pg_catalog.setval('system_groups_id_seq', 5, true);


--
-- Data for Name: system_groups; Type: TABLE DATA; Schema: public; Owner: postgres
--

COPY system_groups (id, name, description) FROM stdin;
1   LoggedOff   Dies ist eine virtuelle Gruppe, daher sind keine Zuordnungen möglich!\nMitglied dieser Gruppe ist man, wenn man sich gegen den Tcp- Server noch nicht authentifiziert hat.
2   InternalSysAdmin    Benutzer dieser Gruppe können auf Softwarespezifische Punkte zugreifen (Tcp- Aktions Rechteverwaltung,...). Benutzer nur in diese Gruppe hinzufügen, wenn man auch weiß, was man tut!
3   UserAccount Diese stellt Funktionen dar, die jeder Account zur Funktionalität benötigt. Ist ein Benutzer nicht in dieser System Gruppe, ist die Funktionalität der Software <b>nicht</b> gewährleistet!
4   BackupMonitor   Benutzer dieser Gruppe können auf den Backup- Monitor zugreifen.
5   Software- Administration    Kann Administrative Software- Operationen ausführen.
\.
