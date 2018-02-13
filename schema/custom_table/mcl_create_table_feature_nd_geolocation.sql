-- psql -h db1 -d gdr_drupal7_live -U ltaein -f mcl_create_table_feature_nd_geolocation.sql
-- psql -h db2 -d gdr_drupal7_test -U taein -f mcl_create_table_feature_nd_geolocation.sql
--
-- Change path
--
SET SEARCH_PATH TO chado;
--
-- Name: feature_nd_geolocation; Type: TABLE; Schema: chado; Owner: -; Tablespace: 
--
CREATE TABLE feature_nd_geolocation (
    feature_nd_geolocation_id integer NOT NULL,
    feature_id integer NOT NULL,
    nd_geolocation_id integer NOT NULL
);
--
-- Name: feature_nd_geolocation_feature_nd_geolocation_id_seq; Type: SEQUENCE; Schema: chado; Owner: -
--
CREATE SEQUENCE feature_nd_geolocation_feature_nd_geolocation_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;
--
-- Setup SEQUENCE
--
ALTER SEQUENCE feature_nd_geolocation_feature_nd_geolocation_id_seq OWNED BY feature_nd_geolocation.feature_nd_geolocation_id;
ALTER TABLE ONLY feature_nd_geolocation ALTER COLUMN feature_nd_geolocation_id SET DEFAULT nextval('feature_nd_geolocation_feature_nd_geolocation_id_seq'::regclass);
--
-- Add PRIMARY KEY
--
ALTER TABLE ONLY feature_nd_geolocation ADD CONSTRAINT feature_nd_geolocation_pkey PRIMARY KEY (feature_nd_geolocation_id);
--
-- Add UNIQUE
--
ALTER TABLE ONLY feature_nd_geolocation ADD CONSTRAINT feature_nd_geolocation_feature_id_nd_geolocation_id_key UNIQUE (feature_id, nd_geolocation_id);
--
-- Add FOREIGN KEY
--
ALTER TABLE ONLY feature_nd_geolocation  ADD CONSTRAINT feature_nd_geolocation_feature_id_fkey FOREIGN KEY (feature_id) REFERENCES feature(feature_id) ON DELETE CASCADE;
ALTER TABLE ONLY feature_nd_geolocation ADD CONSTRAINT feature_nd_geolocation_nd_geolocation_id_fkey FOREIGN KEY (nd_geolocation_id) REFERENCES nd_geolocation(nd_geolocation_id) ON DELETE CASCADE;
