-- psql -h db1 -d gdr_drupal7_live -U ltaein -f mcl_create_table_cvterm_image.sql
-- psql -h db2 -d gdr_drupal7_test -U taein -f mcl_create_table_cvterm_image.sql
--
-- Change path
--
SET SEARCH_PATH TO chado;
--
-- Name: cvterm_image; Type: TABLE; Schema: chado; Owner: -; Tablespace: 
--
CREATE TABLE cvterm_image (
    cvterm_image_id integer NOT NULL,
    cvterm_id integer NOT NULL,
    eimage_id integer NOT NULL
);
--
-- Name: cvterm_image_cvterm_image_id_seq; Type: SEQUENCE; Schema: chado; Owner: -
--
CREATE SEQUENCE cvterm_image_cvterm_image_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;
--
-- Setup SEQUENCE
--
ALTER SEQUENCE cvterm_image_cvterm_image_id_seq OWNED BY cvterm_image.cvterm_image_id;
ALTER TABLE ONLY cvterm_image ALTER COLUMN cvterm_image_id SET DEFAULT nextval('cvterm_image_cvterm_image_id_seq'::regclass);
--
-- Add PRIMARY KEY
--
ALTER TABLE ONLY cvterm_image ADD CONSTRAINT cvterm_image_a_pkey PRIMARY KEY (cvterm_image_id);
--
-- Add UNIQUE
--
ALTER TABLE ONLY cvterm_image ADD CONSTRAINT cvterm_image_cvterm_id_eimage_id_key UNIQUE (cvterm_id, eimage_id);
--
-- Add FOREIGN KEY
--
ALTER TABLE ONLY cvterm_image ADD CONSTRAINT cvterm_image_cvterm_id_fkey FOREIGN KEY (cvterm_id) REFERENCES cvterm(cvterm_id) ON DELETE CASCADE;
ALTER TABLE ONLY cvterm_image ADD CONSTRAINT cvterm_image_eimage_id_fkey FOREIGN KEY (eimage_id) REFERENCES eimage(eimage_id) ON DELETE CASCADE;
