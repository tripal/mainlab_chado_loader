-- psql -h db1 -d cottongen_drupal7_live -U ltaein -f mcl_create_table_organism_pub.sql
--
-- Change path
--
SET SEARCH_PATH TO chado;
--
-- Name: organism_pub; Type: TABLE; Schema: chado; Owner: -; Tablespace: 
--
CREATE TABLE organism_pub (
    organism_pub_id integer NOT NULL,
    organism_id integer NOT NULL,
    pub_id integer NOT NULL
);
--
-- Name: organism_pub_organism_pub_id_seq; Type: SEQUENCE; Schema: chado; Owner: -
--
CREATE SEQUENCE organism_pub_organism_pub_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;
--
-- Setup SEQUENCE
--
ALTER SEQUENCE organism_pub_organism_pub_id_seq OWNED BY organism_pub.organism_pub_id;
ALTER TABLE ONLY organism_pub ALTER COLUMN organism_pub_id SET DEFAULT nextval('organism_pub_organism_pub_id_seq'::regclass);
--
-- Add PRIMARY KEY
--
ALTER TABLE ONLY organism_pub ADD CONSTRAINT organism_pub_a_pkey PRIMARY KEY (organism_pub_id);
--
-- Add UNIQUE
--
ALTER TABLE ONLY organism_pub ADD CONSTRAINT organism_pub_organism_id_pub_id_key UNIQUE (organism_id, pub_id);
--
-- Add FOREIGN KEY
--
ALTER TABLE ONLY organism_pub ADD CONSTRAINT organism_pub_organism_id_fkey FOREIGN KEY (organism_id) REFERENCES organism(organism_id) ON DELETE CASCADE;
ALTER TABLE ONLY organism_pub ADD CONSTRAINT organism_pub_pub_id_fkey FOREIGN KEY (pub_id) REFERENCES pub(pub_id) ON DELETE CASCADE;
