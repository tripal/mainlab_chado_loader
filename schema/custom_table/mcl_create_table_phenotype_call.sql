-- psql -h db1 -d gdr_drupal7_live -U ltaein -f mcl_create_table_phenotype_call.sql
-- psql -h db2 -d gdr_drupal7_test -U taein -f mcl_create_table_phenotype_call.sql
--
-- Change path
--
SET SEARCH_PATH TO chado;
--
-- Name: phenotype_call; Type: TABLE; Schema: chado; Owner: -; Tablespace: 
--
CREATE TABLE phenotype_call (
    phenotype_call_id integer NOT NULL,
    project_id integer NOT NULL,
    stock_id integer NOT NULL,
    cvterm_id integer NOT NULL,
    phenotype_id integer NOT NULL,
    nd_geolocation_id integer NOT NULL,
    "time" timestamp without time zone
);
--
-- Name: phenotype_call_phenotype_call_id_seq; Type: SEQUENCE; Schema: chado; Owner: -
--
CREATE SEQUENCE phenotype_call_phenotype_call_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;
--
-- Setup SEQUENCE
--
ALTER SEQUENCE phenotype_call_phenotype_call_id_seq OWNED BY phenotype_call.phenotype_call_id;
ALTER TABLE ONLY phenotype_call ALTER COLUMN phenotype_call_id SET DEFAULT nextval('phenotype_call_phenotype_call_id_seq'::regclass);
--
-- Add PRIMARY KEY
--
ALTER TABLE ONLY phenotype_call ADD CONSTRAINT phenotype_call_a_pkey PRIMARY KEY (phenotype_call_id);
--
-- Add UNIQUE
--
ALTER TABLE ONLY phenotype_call ADD CONSTRAINT phenotype_call_project_id_stock_id_cvterm_id_nd_geolocation_key UNIQUE (project_id, stock_id, cvterm_id, nd_geolocation_id);
--
-- Add FOREIGN KEY
--
ALTER TABLE ONLY phenotype_call ADD CONSTRAINT phenotype_call_cvterm_id_fkey FOREIGN KEY (cvterm_id) REFERENCES cvterm(cvterm_id) ON DELETE CASCADE;
ALTER TABLE ONLY phenotype_call ADD CONSTRAINT phenotype_call_nd_geolocation_id_fkey FOREIGN KEY (nd_geolocation_id) REFERENCES nd_geolocation(nd_geolocation_id) ON DELETE CASCADE;
ALTER TABLE ONLY phenotype_call ADD CONSTRAINT phenotype_call_phenotype_id_fkey FOREIGN KEY (phenotype_id) REFERENCES phenotype(phenotype_id) ON DELETE CASCADE;
ALTER TABLE ONLY phenotype_call ADD CONSTRAINT phenotype_call_project_id_fkey FOREIGN KEY (project_id) REFERENCES project(project_id) ON DELETE CASCADE;
ALTER TABLE ONLY phenotype_call ADD CONSTRAINT phenotype_call_stock_id_fkey FOREIGN KEY (stock_id) REFERENCES stock(stock_id) ON DELETE CASCADE;
