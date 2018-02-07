--
-- PostgreSQL database dump
--

SET statement_timeout = 0;
SET lock_timeout = 0;
SET client_encoding = 'UTF8';
SET standard_conforming_strings = on;
SET check_function_bodies = false;
SET client_min_messages = warning;

SET search_path = public, pg_catalog;

--
-- Name: concat(anynonarray, anynonarray); Type: FUNCTION; Schema: public; Owner: -
--

CREATE FUNCTION concat(anynonarray, anynonarray) RETURNS text
    LANGUAGE sql
    AS $_$SELECT CAST($1 AS text) || CAST($2 AS text);$_$;


--
-- Name: concat(anynonarray, text); Type: FUNCTION; Schema: public; Owner: -
--

CREATE FUNCTION concat(anynonarray, text) RETURNS text
    LANGUAGE sql
    AS $_$SELECT CAST($1 AS text) || $2;$_$;


--
-- Name: concat(text, anynonarray); Type: FUNCTION; Schema: public; Owner: -
--

CREATE FUNCTION concat(text, anynonarray) RETURNS text
    LANGUAGE sql
    AS $_$SELECT $1 || CAST($2 AS text);$_$;


--
-- Name: concat(text, text); Type: FUNCTION; Schema: public; Owner: -
--

CREATE FUNCTION concat(text, text) RETURNS text
    LANGUAGE sql
    AS $_$SELECT $1 || $2;$_$;


--
-- Name: first(anyelement, anyelement); Type: FUNCTION; Schema: public; Owner: -
--

CREATE FUNCTION first(anyelement, anyelement) RETURNS anyelement
    LANGUAGE sql
    AS $_$SELECT COALESCE($1, $2);$_$;


--
-- Name: gensas_update_seq_rank(integer); Type: FUNCTION; Schema: public; Owner: -
--

CREATE FUNCTION gensas_update_seq_rank(seq_group_id integer) RETURNS void
    LANGUAGE plpgsql
    AS $$
      DECLARE
        i int;
        id int;
        r1 RECORD;
      BEGIN
        i := 0;
        id := seq_group_id;

        -- assign rank to each sequence in a group
        FOR r1 IN
          SELECT GS.seq_id
          FROM gensas_seq GS
          WHERE GS.seq_group_id = id
          ORDER BY GS.name
        LOOP
          -- update gensas_seq.rank
          UPDATE gensas_seq SET rank = i WHERE seq_id = r1.seq_id;
          i := i + 1;
        END LOOP;
      END;
      $$;


--
-- Name: gensas_update_seq_rank_all(); Type: FUNCTION; Schema: public; Owner: -
--

CREATE FUNCTION gensas_update_seq_rank_all() RETURNS void
    LANGUAGE plpgsql
    AS $$
      DECLARE
        r1 RECORD;
      BEGIN
        -- get all sequence groups
        FOR r1 IN
          SELECT seq_group_id FROM gensas_seq_group
        LOOP
          PERFORM gensas_update_seq_rank(r1.seq_group_id);
        END LOOP;
      END;
      $$;


--
-- Name: greatest(numeric, numeric); Type: FUNCTION; Schema: public; Owner: -
--

CREATE FUNCTION "greatest"(numeric, numeric) RETURNS numeric
    LANGUAGE sql
    AS $_$SELECT CASE WHEN (($1 > $2) OR ($2 IS NULL)) THEN $1 ELSE $2 END;$_$;


--
-- Name: greatest(numeric, numeric, numeric); Type: FUNCTION; Schema: public; Owner: -
--

CREATE FUNCTION "greatest"(numeric, numeric, numeric) RETURNS numeric
    LANGUAGE sql
    AS $_$SELECT greatest($1, greatest($2, $3));$_$;


--
-- Name: rand(); Type: FUNCTION; Schema: public; Owner: -
--

CREATE FUNCTION rand() RETURNS double precision
    LANGUAGE sql
    AS $$SELECT random();$$;


--
-- Name: substring_index(text, text, integer); Type: FUNCTION; Schema: public; Owner: -
--

CREATE FUNCTION substring_index(text, text, integer) RETURNS text
    LANGUAGE sql
    AS $_$SELECT array_to_string((string_to_array($1, $2)) [1:$3], $2);$_$;


--
-- Name: first(anyelement); Type: AGGREGATE; Schema: public; Owner: -
--

CREATE AGGREGATE first(anyelement) (
    SFUNC = public.first,
    STYPE = anyelement
);


SET default_tablespace = '';

SET default_with_oids = false;

--
-- Name: actions; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE actions (
    aid character varying(255) DEFAULT '0'::character varying NOT NULL,
    type character varying(32) DEFAULT ''::character varying NOT NULL,
    callback character varying(255) DEFAULT ''::character varying NOT NULL,
    parameters bytea NOT NULL,
    label character varying(255) DEFAULT '0'::character varying NOT NULL
);


--
-- Name: TABLE actions; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE actions IS 'Stores action information.';


--
-- Name: COLUMN actions.aid; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN actions.aid IS 'Primary Key: Unique actions ID.';


--
-- Name: COLUMN actions.type; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN actions.type IS 'The object that that action acts on (node, user, comment, system or custom types.)';


--
-- Name: COLUMN actions.callback; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN actions.callback IS 'The callback function that executes when the action runs.';


--
-- Name: COLUMN actions.parameters; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN actions.parameters IS 'Parameters to be passed to the callback function.';


--
-- Name: COLUMN actions.label; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN actions.label IS 'Label of the action.';


--
-- Name: authmap; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE authmap (
    aid integer NOT NULL,
    uid integer DEFAULT 0 NOT NULL,
    authname character varying(128) DEFAULT ''::character varying NOT NULL,
    module character varying(128) DEFAULT ''::character varying NOT NULL,
    CONSTRAINT authmap_aid_check CHECK ((aid >= 0))
);


--
-- Name: TABLE authmap; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE authmap IS 'Stores distributed authentication mapping.';


--
-- Name: COLUMN authmap.aid; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN authmap.aid IS 'Primary Key: Unique authmap ID.';


--
-- Name: COLUMN authmap.uid; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN authmap.uid IS 'User''s users.uid.';


--
-- Name: COLUMN authmap.authname; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN authmap.authname IS 'Unique authentication name.';


--
-- Name: COLUMN authmap.module; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN authmap.module IS 'Module which is controlling the authentication.';


--
-- Name: authmap_aid_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE authmap_aid_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


--
-- Name: authmap_aid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: -
--

ALTER SEQUENCE authmap_aid_seq OWNED BY authmap.aid;


--
-- Name: batch; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE batch (
    bid bigint NOT NULL,
    token character varying(64) NOT NULL,
    "timestamp" integer NOT NULL,
    batch bytea,
    CONSTRAINT batch_bid_check CHECK ((bid >= 0))
);


--
-- Name: TABLE batch; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE batch IS 'Stores details about batches (processes that run in multiple HTTP requests).';


--
-- Name: COLUMN batch.bid; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN batch.bid IS 'Primary Key: Unique batch ID.';


--
-- Name: COLUMN batch.token; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN batch.token IS 'A string token generated against the current user''s session id and the batch id, used to ensure that only the user who submitted the batch can effectively access it.';


--
-- Name: COLUMN batch."timestamp"; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN batch."timestamp" IS 'A Unix timestamp indicating when this batch was submitted for processing. Stale batches are purged at cron time.';


--
-- Name: COLUMN batch.batch; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN batch.batch IS 'A serialized array containing the processing data for the batch.';


--
-- Name: bims_archive_type; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE bims_archive_type (
    archive_type_id integer NOT NULL,
    type character varying(255) NOT NULL,
    prop text,
    rank integer DEFAULT 0 NOT NULL
);


--
-- Name: bims_archive_type_archive_type_id_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE bims_archive_type_archive_type_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


--
-- Name: bims_archive_type_archive_type_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: -
--

ALTER SEQUENCE bims_archive_type_archive_type_id_seq OWNED BY bims_archive_type.archive_type_id;


--
-- Name: bims_crop; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE bims_crop (
    crop_id integer NOT NULL,
    name character varying(255) NOT NULL,
    label character varying(255) NOT NULL,
    image_url character varying(255) NOT NULL,
    prop text
);


--
-- Name: bims_crop_crop_id_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE bims_crop_crop_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


--
-- Name: bims_crop_crop_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: -
--

ALTER SEQUENCE bims_crop_crop_id_seq OWNED BY bims_crop.crop_id;


--
-- Name: bims_crop_organism; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE bims_crop_organism (
    crop_id integer NOT NULL,
    organism_id integer NOT NULL
);


--
-- Name: bims_file; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE bims_file (
    file_id integer NOT NULL,
    type character varying(255) NOT NULL,
    filename character varying(255) NOT NULL,
    filepath character varying(512) NOT NULL,
    filesize integer DEFAULT 0 NOT NULL,
    uri character varying(255) NOT NULL,
    user_id integer NOT NULL,
    submit_date timestamp without time zone NOT NULL,
    prop text,
    program_id integer,
    description text
);


--
-- Name: bims_file_file_id_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE bims_file_file_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


--
-- Name: bims_file_file_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: -
--

ALTER SEQUENCE bims_file_file_id_seq OWNED BY bims_file.file_id;


--
-- Name: bims_instruction; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE bims_instruction (
    instruction_id integer NOT NULL,
    id character varying(255) NOT NULL,
    instruction text
);


--
-- Name: bims_instruction_instruction_id_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE bims_instruction_instruction_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


--
-- Name: bims_instruction_instruction_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: -
--

ALTER SEQUENCE bims_instruction_instruction_id_seq OWNED BY bims_instruction.instruction_id;


--
-- Name: bims_list; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE bims_list (
    list_id integer NOT NULL,
    name character varying(255) NOT NULL,
    type character varying(255) NOT NULL,
    program_id integer NOT NULL,
    user_id integer NOT NULL,
    prop text,
    create_date timestamp without time zone NOT NULL,
    update_date timestamp without time zone,
    description text,
    shared integer DEFAULT 0 NOT NULL
);


--
-- Name: bims_list_list_id_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE bims_list_list_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


--
-- Name: bims_list_list_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: -
--

ALTER SEQUENCE bims_list_list_id_seq OWNED BY bims_list.list_id;


--
-- Name: bims_mview_cross_stats; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE bims_mview_cross_stats (
    node_id integer NOT NULL,
    nd_experiment_id integer NOT NULL,
    name character varying(255) NOT NULL,
    cvterm_id integer NOT NULL,
    stats text,
    num_data integer DEFAULT 0 NOT NULL
);


--
-- Name: bims_mview_descriptor; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE bims_mview_descriptor (
    cv_id integer NOT NULL,
    category character varying(255),
    name character varying(255) NOT NULL,
    cvterm_id integer NOT NULL,
    alias character varying(255),
    format character varying(255),
    prop text,
    definition text
);


--
-- Name: bims_mview_phenotype_stats; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE bims_mview_phenotype_stats (
    node_id integer NOT NULL,
    stats text,
    cvterm_id integer NOT NULL,
    name character varying(255) NOT NULL,
    num_data integer DEFAULT 0 NOT NULL
);


--
-- Name: bims_mview_stock_stats; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE bims_mview_stock_stats (
    node_id integer NOT NULL,
    stock_id integer NOT NULL,
    name character varying(255) NOT NULL,
    cvterm_id integer NOT NULL,
    num_data integer DEFAULT 0 NOT NULL,
    stats text
);


--
-- Name: bims_node; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE bims_node (
    node_id integer NOT NULL,
    name character varying(100) NOT NULL,
    type character varying(100) NOT NULL,
    prop text,
    root_id integer,
    crop_id integer,
    owner_id integer NOT NULL,
    project_id integer,
    trial_tree text,
    breed_line_tree text,
    cross_tree text,
    description text,
    access integer DEFAULT 1 NOT NULL
);


--
-- Name: bims_node_node_id_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE bims_node_node_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


--
-- Name: bims_node_node_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: -
--

ALTER SEQUENCE bims_node_node_id_seq OWNED BY bims_node.node_id;


--
-- Name: bims_node_relationship; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE bims_node_relationship (
    relationship_id integer NOT NULL,
    parent_id integer NOT NULL,
    child_id integer NOT NULL
);


--
-- Name: bims_node_relationship_relationship_id_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE bims_node_relationship_relationship_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


--
-- Name: bims_node_relationship_relationship_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: -
--

ALTER SEQUENCE bims_node_relationship_relationship_id_seq OWNED BY bims_node_relationship.relationship_id;


--
-- Name: bims_program_member; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE bims_program_member (
    program_member_id integer NOT NULL,
    program_id integer NOT NULL,
    user_id integer NOT NULL,
    permission character varying(255) NOT NULL
);


--
-- Name: bims_program_member_program_member_id_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE bims_program_member_program_member_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


--
-- Name: bims_program_member_program_member_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: -
--

ALTER SEQUENCE bims_program_member_program_member_id_seq OWNED BY bims_program_member.program_member_id;


--
-- Name: bims_user; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE bims_user (
    user_id integer NOT NULL,
    name character varying(255) NOT NULL,
    uid integer NOT NULL,
    mail character varying(255) NOT NULL,
    breeder integer DEFAULT 0 NOT NULL,
    contact_id integer,
    prop text
);


--
-- Name: block; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE block (
    bid integer NOT NULL,
    module character varying(64) DEFAULT ''::character varying NOT NULL,
    delta character varying(32) DEFAULT '0'::character varying NOT NULL,
    theme character varying(64) DEFAULT ''::character varying NOT NULL,
    status smallint DEFAULT 0 NOT NULL,
    weight integer DEFAULT 0 NOT NULL,
    region character varying(64) DEFAULT ''::character varying NOT NULL,
    custom smallint DEFAULT 0 NOT NULL,
    visibility smallint DEFAULT 0 NOT NULL,
    pages text NOT NULL,
    title character varying(255) DEFAULT ''::character varying NOT NULL,
    cache smallint DEFAULT 1 NOT NULL
);


--
-- Name: TABLE block; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE block IS 'Stores block settings, such as region and visibility settings.';


--
-- Name: COLUMN block.bid; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN block.bid IS 'Primary Key: Unique block ID.';


--
-- Name: COLUMN block.module; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN block.module IS 'The module from which the block originates; for example, ''user'' for the Who''s Online block, and ''block'' for any custom blocks.';


--
-- Name: COLUMN block.delta; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN block.delta IS 'Unique ID for block within a module.';


--
-- Name: COLUMN block.theme; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN block.theme IS 'The theme under which the block settings apply.';


--
-- Name: COLUMN block.status; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN block.status IS 'Block enabled status. (1 = enabled, 0 = disabled)';


--
-- Name: COLUMN block.weight; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN block.weight IS 'Block weight within region.';


--
-- Name: COLUMN block.region; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN block.region IS 'Theme region within which the block is set.';


--
-- Name: COLUMN block.custom; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN block.custom IS 'Flag to indicate how users may control visibility of the block. (0 = Users cannot control, 1 = On by default, but can be hidden, 2 = Hidden by default, but can be shown)';


--
-- Name: COLUMN block.visibility; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN block.visibility IS 'Flag to indicate how to show blocks on pages. (0 = Show on all pages except listed pages, 1 = Show only on listed pages, 2 = Use custom PHP code to determine visibility)';


--
-- Name: COLUMN block.pages; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN block.pages IS 'Contents of the "Pages" block; contains either a list of paths on which to include/exclude the block or PHP code, depending on "visibility" setting.';


--
-- Name: COLUMN block.title; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN block.title IS 'Custom title for the block. (Empty string will use block default title, <none> will remove the title, text will cause block to use specified title.)';


--
-- Name: COLUMN block.cache; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN block.cache IS 'Binary flag to indicate block cache mode. (-2: Custom cache, -1: Do not cache, 1: Cache per role, 2: Cache per user, 4: Cache per page, 8: Block cache global) See DRUPAL_CACHE_* constants in ../includes/common.inc for more detailed information.';


--
-- Name: block_bid_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE block_bid_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


--
-- Name: block_bid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: -
--

ALTER SEQUENCE block_bid_seq OWNED BY block.bid;


--
-- Name: block_custom; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE block_custom (
    bid integer NOT NULL,
    body text,
    info character varying(128) DEFAULT ''::character varying NOT NULL,
    format character varying(255),
    CONSTRAINT block_custom_bid_check CHECK ((bid >= 0))
);


--
-- Name: TABLE block_custom; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE block_custom IS 'Stores contents of custom-made blocks.';


--
-- Name: COLUMN block_custom.bid; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN block_custom.bid IS 'The block''s block.bid.';


--
-- Name: COLUMN block_custom.body; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN block_custom.body IS 'Block contents.';


--
-- Name: COLUMN block_custom.info; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN block_custom.info IS 'Block description.';


--
-- Name: COLUMN block_custom.format; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN block_custom.format IS 'The filter_format.format of the block body.';


--
-- Name: block_custom_bid_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE block_custom_bid_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


--
-- Name: block_custom_bid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: -
--

ALTER SEQUENCE block_custom_bid_seq OWNED BY block_custom.bid;


--
-- Name: block_node_type; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE block_node_type (
    module character varying(64) NOT NULL,
    delta character varying(32) NOT NULL,
    type character varying(32) NOT NULL
);


--
-- Name: TABLE block_node_type; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE block_node_type IS 'Sets up display criteria for blocks based on content types';


--
-- Name: COLUMN block_node_type.module; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN block_node_type.module IS 'The block''s origin module, from block.module.';


--
-- Name: COLUMN block_node_type.delta; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN block_node_type.delta IS 'The block''s unique delta within module, from block.delta.';


--
-- Name: COLUMN block_node_type.type; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN block_node_type.type IS 'The machine-readable name of this type from node_type.type.';


--
-- Name: block_role; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE block_role (
    module character varying(64) NOT NULL,
    delta character varying(32) NOT NULL,
    rid bigint NOT NULL,
    CONSTRAINT block_role_rid_check CHECK ((rid >= 0))
);


--
-- Name: TABLE block_role; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE block_role IS 'Sets up access permissions for blocks based on user roles';


--
-- Name: COLUMN block_role.module; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN block_role.module IS 'The block''s origin module, from block.module.';


--
-- Name: COLUMN block_role.delta; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN block_role.delta IS 'The block''s unique delta within module, from block.delta.';


--
-- Name: COLUMN block_role.rid; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN block_role.rid IS 'The user''s role ID from users_roles.rid.';


--
-- Name: blocked_ips; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE blocked_ips (
    iid integer NOT NULL,
    ip character varying(40) DEFAULT ''::character varying NOT NULL,
    CONSTRAINT blocked_ips_iid_check CHECK ((iid >= 0))
);


--
-- Name: TABLE blocked_ips; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE blocked_ips IS 'Stores blocked IP addresses.';


--
-- Name: COLUMN blocked_ips.iid; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN blocked_ips.iid IS 'Primary Key: unique ID for IP addresses.';


--
-- Name: COLUMN blocked_ips.ip; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN blocked_ips.ip IS 'IP address';


--
-- Name: blocked_ips_iid_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE blocked_ips_iid_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


--
-- Name: blocked_ips_iid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: -
--

ALTER SEQUENCE blocked_ips_iid_seq OWNED BY blocked_ips.iid;


--
-- Name: book; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE book (
    mlid bigint DEFAULT 0 NOT NULL,
    nid bigint DEFAULT 0 NOT NULL,
    bid bigint DEFAULT 0 NOT NULL,
    CONSTRAINT book_bid_check CHECK ((bid >= 0)),
    CONSTRAINT book_mlid_check CHECK ((mlid >= 0)),
    CONSTRAINT book_nid_check CHECK ((nid >= 0))
);


--
-- Name: TABLE book; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE book IS 'Stores book outline information. Uniquely connects each node in the outline to a link in menu_links';


--
-- Name: COLUMN book.mlid; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN book.mlid IS 'The book page''s menu_links.mlid.';


--
-- Name: COLUMN book.nid; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN book.nid IS 'The book page''s node.nid.';


--
-- Name: COLUMN book.bid; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN book.bid IS 'The book ID is the book.nid of the top-level page.';


--
-- Name: cache; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE cache (
    cid character varying(255) DEFAULT ''::character varying NOT NULL,
    data bytea,
    expire integer DEFAULT 0 NOT NULL,
    created integer DEFAULT 0 NOT NULL,
    serialized smallint DEFAULT 0 NOT NULL
);


--
-- Name: TABLE cache; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE cache IS 'Generic cache table for caching things not separated out into their own tables. Contributed modules may also use this to store cached items.';


--
-- Name: COLUMN cache.cid; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN cache.cid IS 'Primary Key: Unique cache ID.';


--
-- Name: COLUMN cache.data; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN cache.data IS 'A collection of data to cache.';


--
-- Name: COLUMN cache.expire; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN cache.expire IS 'A Unix timestamp indicating when the cache entry should expire, or 0 for never.';


--
-- Name: COLUMN cache.created; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN cache.created IS 'A Unix timestamp indicating when the cache entry was created.';


--
-- Name: COLUMN cache.serialized; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN cache.serialized IS 'A flag to indicate whether content is serialized (1) or not (0).';


--
-- Name: cache_block; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE cache_block (
    cid character varying(255) DEFAULT ''::character varying NOT NULL,
    data bytea,
    expire integer DEFAULT 0 NOT NULL,
    created integer DEFAULT 0 NOT NULL,
    serialized smallint DEFAULT 0 NOT NULL
);


--
-- Name: TABLE cache_block; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE cache_block IS 'Cache table for the Block module to store already built blocks, identified by module, delta, and various contexts which may change the block, such as theme, locale, and caching mode defined for the block.';


--
-- Name: COLUMN cache_block.cid; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN cache_block.cid IS 'Primary Key: Unique cache ID.';


--
-- Name: COLUMN cache_block.data; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN cache_block.data IS 'A collection of data to cache.';


--
-- Name: COLUMN cache_block.expire; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN cache_block.expire IS 'A Unix timestamp indicating when the cache entry should expire, or 0 for never.';


--
-- Name: COLUMN cache_block.created; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN cache_block.created IS 'A Unix timestamp indicating when the cache entry was created.';


--
-- Name: COLUMN cache_block.serialized; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN cache_block.serialized IS 'A flag to indicate whether content is serialized (1) or not (0).';


--
-- Name: cache_bootstrap; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE cache_bootstrap (
    cid character varying(255) DEFAULT ''::character varying NOT NULL,
    data bytea,
    expire integer DEFAULT 0 NOT NULL,
    created integer DEFAULT 0 NOT NULL,
    serialized smallint DEFAULT 0 NOT NULL
);


--
-- Name: TABLE cache_bootstrap; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE cache_bootstrap IS 'Cache table for data required to bootstrap Drupal, may be routed to a shared memory cache.';


--
-- Name: COLUMN cache_bootstrap.cid; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN cache_bootstrap.cid IS 'Primary Key: Unique cache ID.';


--
-- Name: COLUMN cache_bootstrap.data; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN cache_bootstrap.data IS 'A collection of data to cache.';


--
-- Name: COLUMN cache_bootstrap.expire; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN cache_bootstrap.expire IS 'A Unix timestamp indicating when the cache entry should expire, or 0 for never.';


--
-- Name: COLUMN cache_bootstrap.created; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN cache_bootstrap.created IS 'A Unix timestamp indicating when the cache entry was created.';


--
-- Name: COLUMN cache_bootstrap.serialized; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN cache_bootstrap.serialized IS 'A flag to indicate whether content is serialized (1) or not (0).';


--
-- Name: cache_field; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE cache_field (
    cid character varying(255) DEFAULT ''::character varying NOT NULL,
    data bytea,
    expire integer DEFAULT 0 NOT NULL,
    created integer DEFAULT 0 NOT NULL,
    serialized smallint DEFAULT 0 NOT NULL
);


--
-- Name: TABLE cache_field; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE cache_field IS 'Cache table for the Field module to store already built field information.';


--
-- Name: COLUMN cache_field.cid; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN cache_field.cid IS 'Primary Key: Unique cache ID.';


--
-- Name: COLUMN cache_field.data; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN cache_field.data IS 'A collection of data to cache.';


--
-- Name: COLUMN cache_field.expire; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN cache_field.expire IS 'A Unix timestamp indicating when the cache entry should expire, or 0 for never.';


--
-- Name: COLUMN cache_field.created; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN cache_field.created IS 'A Unix timestamp indicating when the cache entry was created.';


--
-- Name: COLUMN cache_field.serialized; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN cache_field.serialized IS 'A flag to indicate whether content is serialized (1) or not (0).';


--
-- Name: cache_filter; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE cache_filter (
    cid character varying(255) DEFAULT ''::character varying NOT NULL,
    data bytea,
    expire integer DEFAULT 0 NOT NULL,
    created integer DEFAULT 0 NOT NULL,
    serialized smallint DEFAULT 0 NOT NULL
);


--
-- Name: TABLE cache_filter; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE cache_filter IS 'Cache table for the Filter module to store already filtered pieces of text, identified by text format and hash of the text.';


--
-- Name: COLUMN cache_filter.cid; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN cache_filter.cid IS 'Primary Key: Unique cache ID.';


--
-- Name: COLUMN cache_filter.data; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN cache_filter.data IS 'A collection of data to cache.';


--
-- Name: COLUMN cache_filter.expire; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN cache_filter.expire IS 'A Unix timestamp indicating when the cache entry should expire, or 0 for never.';


--
-- Name: COLUMN cache_filter.created; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN cache_filter.created IS 'A Unix timestamp indicating when the cache entry was created.';


--
-- Name: COLUMN cache_filter.serialized; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN cache_filter.serialized IS 'A flag to indicate whether content is serialized (1) or not (0).';


--
-- Name: cache_form; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE cache_form (
    cid character varying(255) DEFAULT ''::character varying NOT NULL,
    data bytea,
    expire integer DEFAULT 0 NOT NULL,
    created integer DEFAULT 0 NOT NULL,
    serialized smallint DEFAULT 0 NOT NULL
);


--
-- Name: TABLE cache_form; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE cache_form IS 'Cache table for the form system to store recently built forms and their storage data, to be used in subsequent page requests.';


--
-- Name: COLUMN cache_form.cid; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN cache_form.cid IS 'Primary Key: Unique cache ID.';


--
-- Name: COLUMN cache_form.data; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN cache_form.data IS 'A collection of data to cache.';


--
-- Name: COLUMN cache_form.expire; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN cache_form.expire IS 'A Unix timestamp indicating when the cache entry should expire, or 0 for never.';


--
-- Name: COLUMN cache_form.created; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN cache_form.created IS 'A Unix timestamp indicating when the cache entry was created.';


--
-- Name: COLUMN cache_form.serialized; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN cache_form.serialized IS 'A flag to indicate whether content is serialized (1) or not (0).';


--
-- Name: cache_image; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE cache_image (
    cid character varying(255) DEFAULT ''::character varying NOT NULL,
    data bytea,
    expire integer DEFAULT 0 NOT NULL,
    created integer DEFAULT 0 NOT NULL,
    serialized smallint DEFAULT 0 NOT NULL
);


--
-- Name: TABLE cache_image; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE cache_image IS 'Cache table used to store information about image manipulations that are in-progress.';


--
-- Name: COLUMN cache_image.cid; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN cache_image.cid IS 'Primary Key: Unique cache ID.';


--
-- Name: COLUMN cache_image.data; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN cache_image.data IS 'A collection of data to cache.';


--
-- Name: COLUMN cache_image.expire; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN cache_image.expire IS 'A Unix timestamp indicating when the cache entry should expire, or 0 for never.';


--
-- Name: COLUMN cache_image.created; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN cache_image.created IS 'A Unix timestamp indicating when the cache entry was created.';


--
-- Name: COLUMN cache_image.serialized; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN cache_image.serialized IS 'A flag to indicate whether content is serialized (1) or not (0).';


--
-- Name: cache_menu; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE cache_menu (
    cid character varying(255) DEFAULT ''::character varying NOT NULL,
    data bytea,
    expire integer DEFAULT 0 NOT NULL,
    created integer DEFAULT 0 NOT NULL,
    serialized smallint DEFAULT 0 NOT NULL
);


--
-- Name: TABLE cache_menu; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE cache_menu IS 'Cache table for the menu system to store router information as well as generated link trees for various menu/page/user combinations.';


--
-- Name: COLUMN cache_menu.cid; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN cache_menu.cid IS 'Primary Key: Unique cache ID.';


--
-- Name: COLUMN cache_menu.data; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN cache_menu.data IS 'A collection of data to cache.';


--
-- Name: COLUMN cache_menu.expire; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN cache_menu.expire IS 'A Unix timestamp indicating when the cache entry should expire, or 0 for never.';


--
-- Name: COLUMN cache_menu.created; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN cache_menu.created IS 'A Unix timestamp indicating when the cache entry was created.';


--
-- Name: COLUMN cache_menu.serialized; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN cache_menu.serialized IS 'A flag to indicate whether content is serialized (1) or not (0).';


--
-- Name: cache_page; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE cache_page (
    cid character varying(255) DEFAULT ''::character varying NOT NULL,
    data bytea,
    expire integer DEFAULT 0 NOT NULL,
    created integer DEFAULT 0 NOT NULL,
    serialized smallint DEFAULT 0 NOT NULL
);


--
-- Name: TABLE cache_page; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE cache_page IS 'Cache table used to store compressed pages for anonymous users, if page caching is enabled.';


--
-- Name: COLUMN cache_page.cid; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN cache_page.cid IS 'Primary Key: Unique cache ID.';


--
-- Name: COLUMN cache_page.data; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN cache_page.data IS 'A collection of data to cache.';


--
-- Name: COLUMN cache_page.expire; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN cache_page.expire IS 'A Unix timestamp indicating when the cache entry should expire, or 0 for never.';


--
-- Name: COLUMN cache_page.created; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN cache_page.created IS 'A Unix timestamp indicating when the cache entry was created.';


--
-- Name: COLUMN cache_page.serialized; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN cache_page.serialized IS 'A flag to indicate whether content is serialized (1) or not (0).';


--
-- Name: cache_path; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE cache_path (
    cid character varying(255) DEFAULT ''::character varying NOT NULL,
    data bytea,
    expire integer DEFAULT 0 NOT NULL,
    created integer DEFAULT 0 NOT NULL,
    serialized smallint DEFAULT 0 NOT NULL
);


--
-- Name: TABLE cache_path; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE cache_path IS 'Cache table for path alias lookup.';


--
-- Name: COLUMN cache_path.cid; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN cache_path.cid IS 'Primary Key: Unique cache ID.';


--
-- Name: COLUMN cache_path.data; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN cache_path.data IS 'A collection of data to cache.';


--
-- Name: COLUMN cache_path.expire; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN cache_path.expire IS 'A Unix timestamp indicating when the cache entry should expire, or 0 for never.';


--
-- Name: COLUMN cache_path.created; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN cache_path.created IS 'A Unix timestamp indicating when the cache entry was created.';


--
-- Name: COLUMN cache_path.serialized; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN cache_path.serialized IS 'A flag to indicate whether content is serialized (1) or not (0).';


--
-- Name: cache_update; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE cache_update (
    cid character varying(255) DEFAULT ''::character varying NOT NULL,
    data bytea,
    expire integer DEFAULT 0 NOT NULL,
    created integer DEFAULT 0 NOT NULL,
    serialized smallint DEFAULT 0 NOT NULL
);


--
-- Name: TABLE cache_update; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE cache_update IS 'Cache table for the Update module to store information about available releases, fetched from central server.';


--
-- Name: COLUMN cache_update.cid; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN cache_update.cid IS 'Primary Key: Unique cache ID.';


--
-- Name: COLUMN cache_update.data; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN cache_update.data IS 'A collection of data to cache.';


--
-- Name: COLUMN cache_update.expire; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN cache_update.expire IS 'A Unix timestamp indicating when the cache entry should expire, or 0 for never.';


--
-- Name: COLUMN cache_update.created; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN cache_update.created IS 'A Unix timestamp indicating when the cache entry was created.';


--
-- Name: COLUMN cache_update.serialized; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN cache_update.serialized IS 'A flag to indicate whether content is serialized (1) or not (0).';


--
-- Name: cache_views; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE cache_views (
    cid character varying(255) DEFAULT ''::character varying NOT NULL,
    data bytea,
    expire integer DEFAULT 0 NOT NULL,
    created integer DEFAULT 0 NOT NULL,
    serialized smallint DEFAULT 0 NOT NULL
);


--
-- Name: TABLE cache_views; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE cache_views IS 'Generic cache table for caching things not separated out into their own tables. Contributed modules may also use this to store cached items.';


--
-- Name: COLUMN cache_views.cid; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN cache_views.cid IS 'Primary Key: Unique cache ID.';


--
-- Name: COLUMN cache_views.data; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN cache_views.data IS 'A collection of data to cache.';


--
-- Name: COLUMN cache_views.expire; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN cache_views.expire IS 'A Unix timestamp indicating when the cache entry should expire, or 0 for never.';


--
-- Name: COLUMN cache_views.created; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN cache_views.created IS 'A Unix timestamp indicating when the cache entry was created.';


--
-- Name: COLUMN cache_views.serialized; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN cache_views.serialized IS 'A flag to indicate whether content is serialized (1) or not (0).';


--
-- Name: cache_views_data; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE cache_views_data (
    cid character varying(255) DEFAULT ''::character varying NOT NULL,
    data bytea,
    expire integer DEFAULT 0 NOT NULL,
    created integer DEFAULT 0 NOT NULL,
    serialized smallint DEFAULT 1 NOT NULL
);


--
-- Name: TABLE cache_views_data; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE cache_views_data IS 'Cache table for views to store pre-rendered queries, results, and display output.';


--
-- Name: COLUMN cache_views_data.cid; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN cache_views_data.cid IS 'Primary Key: Unique cache ID.';


--
-- Name: COLUMN cache_views_data.data; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN cache_views_data.data IS 'A collection of data to cache.';


--
-- Name: COLUMN cache_views_data.expire; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN cache_views_data.expire IS 'A Unix timestamp indicating when the cache entry should expire, or 0 for never.';


--
-- Name: COLUMN cache_views_data.created; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN cache_views_data.created IS 'A Unix timestamp indicating when the cache entry was created.';


--
-- Name: COLUMN cache_views_data.serialized; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN cache_views_data.serialized IS 'A flag to indicate whether content is serialized (1) or not (0).';


--
-- Name: comment; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE comment (
    cid integer NOT NULL,
    pid integer DEFAULT 0 NOT NULL,
    nid integer DEFAULT 0 NOT NULL,
    uid integer DEFAULT 0 NOT NULL,
    subject character varying(64) DEFAULT ''::character varying NOT NULL,
    hostname character varying(128) DEFAULT ''::character varying NOT NULL,
    created integer DEFAULT 0 NOT NULL,
    changed integer DEFAULT 0 NOT NULL,
    status integer DEFAULT 1 NOT NULL,
    thread character varying(255) NOT NULL,
    name character varying(60),
    mail character varying(64),
    homepage character varying(255),
    language character varying(12) DEFAULT ''::character varying NOT NULL,
    CONSTRAINT comment_status_check CHECK ((status >= 0))
);


--
-- Name: TABLE comment; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE comment IS 'Stores comments and associated data.';


--
-- Name: COLUMN comment.cid; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN comment.cid IS 'Primary Key: Unique comment ID.';


--
-- Name: COLUMN comment.pid; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN comment.pid IS 'The comment.cid to which this comment is a reply. If set to 0, this comment is not a reply to an existing comment.';


--
-- Name: COLUMN comment.nid; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN comment.nid IS 'The node.nid to which this comment is a reply.';


--
-- Name: COLUMN comment.uid; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN comment.uid IS 'The users.uid who authored the comment. If set to 0, this comment was created by an anonymous user.';


--
-- Name: COLUMN comment.subject; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN comment.subject IS 'The comment title.';


--
-- Name: COLUMN comment.hostname; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN comment.hostname IS 'The author''s host name.';


--
-- Name: COLUMN comment.created; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN comment.created IS 'The time that the comment was created, as a Unix timestamp.';


--
-- Name: COLUMN comment.changed; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN comment.changed IS 'The time that the comment was last edited, as a Unix timestamp.';


--
-- Name: COLUMN comment.status; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN comment.status IS 'The published status of a comment. (0 = Not Published, 1 = Published)';


--
-- Name: COLUMN comment.thread; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN comment.thread IS 'The vancode representation of the comment''s place in a thread.';


--
-- Name: COLUMN comment.name; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN comment.name IS 'The comment author''s name. Uses users.name if the user is logged in, otherwise uses the value typed into the comment form.';


--
-- Name: COLUMN comment.mail; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN comment.mail IS 'The comment author''s e-mail address from the comment form, if user is anonymous, and the ''Anonymous users may/must leave their contact information'' setting is turned on.';


--
-- Name: COLUMN comment.homepage; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN comment.homepage IS 'The comment author''s home page address from the comment form, if user is anonymous, and the ''Anonymous users may/must leave their contact information'' setting is turned on.';


--
-- Name: COLUMN comment.language; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN comment.language IS 'The languages.language of this comment.';


--
-- Name: comment_cid_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE comment_cid_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


--
-- Name: comment_cid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: -
--

ALTER SEQUENCE comment_cid_seq OWNED BY comment.cid;


--
-- Name: contact; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE contact (
    cid integer NOT NULL,
    category character varying(255) DEFAULT ''::character varying NOT NULL,
    recipients text NOT NULL,
    reply text NOT NULL,
    weight integer DEFAULT 0 NOT NULL,
    selected smallint DEFAULT 0 NOT NULL,
    CONSTRAINT contact_cid_check CHECK ((cid >= 0))
);


--
-- Name: TABLE contact; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE contact IS 'Contact form category settings.';


--
-- Name: COLUMN contact.cid; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN contact.cid IS 'Primary Key: Unique category ID.';


--
-- Name: COLUMN contact.category; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN contact.category IS 'Category name.';


--
-- Name: COLUMN contact.recipients; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN contact.recipients IS 'Comma-separated list of recipient e-mail addresses.';


--
-- Name: COLUMN contact.reply; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN contact.reply IS 'Text of the auto-reply message.';


--
-- Name: COLUMN contact.weight; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN contact.weight IS 'The category''s weight.';


--
-- Name: COLUMN contact.selected; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN contact.selected IS 'Flag to indicate whether or not category is selected by default. (1 = Yes, 0 = No)';


--
-- Name: contact_cid_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE contact_cid_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


--
-- Name: contact_cid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: -
--

ALTER SEQUENCE contact_cid_seq OWNED BY contact.cid;


--
-- Name: ctools_css_cache; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE ctools_css_cache (
    cid character varying(128) NOT NULL,
    filename character varying(255),
    css text,
    filter smallint
);


--
-- Name: TABLE ctools_css_cache; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE ctools_css_cache IS 'A special cache used to store CSS that must be non-volatile.';


--
-- Name: COLUMN ctools_css_cache.cid; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN ctools_css_cache.cid IS 'The CSS ID this cache object belongs to.';


--
-- Name: COLUMN ctools_css_cache.filename; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN ctools_css_cache.filename IS 'The filename this CSS is stored in.';


--
-- Name: COLUMN ctools_css_cache.css; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN ctools_css_cache.css IS 'CSS being stored.';


--
-- Name: COLUMN ctools_css_cache.filter; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN ctools_css_cache.filter IS 'Whether or not this CSS needs to be filtered.';


--
-- Name: ctools_object_cache; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE ctools_object_cache (
    sid character varying(64) NOT NULL,
    name character varying(128) NOT NULL,
    obj character varying(128) NOT NULL,
    updated bigint DEFAULT 0 NOT NULL,
    data bytea,
    CONSTRAINT ctools_object_cache_updated_check CHECK ((updated >= 0))
);


--
-- Name: TABLE ctools_object_cache; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE ctools_object_cache IS 'A special cache used to store objects that are being edited; it serves to save state in an ordinarily stateless environment.';


--
-- Name: COLUMN ctools_object_cache.sid; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN ctools_object_cache.sid IS 'The session ID this cache object belongs to.';


--
-- Name: COLUMN ctools_object_cache.name; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN ctools_object_cache.name IS 'The name of the object this cache is attached to.';


--
-- Name: COLUMN ctools_object_cache.obj; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN ctools_object_cache.obj IS 'The type of the object this cache is attached to; this essentially represents the owner so that several sub-systems can use this cache.';


--
-- Name: COLUMN ctools_object_cache.updated; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN ctools_object_cache.updated IS 'The time this cache was created or updated.';


--
-- Name: COLUMN ctools_object_cache.data; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN ctools_object_cache.data IS 'Serialized data being stored.';


--
-- Name: date_format_locale; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE date_format_locale (
    format character varying(100) NOT NULL,
    type character varying(64) NOT NULL,
    language character varying(12) NOT NULL
);


--
-- Name: TABLE date_format_locale; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE date_format_locale IS 'Stores configured date formats for each locale.';


--
-- Name: COLUMN date_format_locale.format; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN date_format_locale.format IS 'The date format string.';


--
-- Name: COLUMN date_format_locale.type; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN date_format_locale.type IS 'The date format type, e.g. medium.';


--
-- Name: COLUMN date_format_locale.language; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN date_format_locale.language IS 'A languages.language for this format to be used with.';


--
-- Name: date_format_type; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE date_format_type (
    type character varying(64) NOT NULL,
    title character varying(255) NOT NULL,
    locked smallint DEFAULT 0 NOT NULL
);


--
-- Name: TABLE date_format_type; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE date_format_type IS 'Stores configured date format types.';


--
-- Name: COLUMN date_format_type.type; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN date_format_type.type IS 'The date format type, e.g. medium.';


--
-- Name: COLUMN date_format_type.title; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN date_format_type.title IS 'The human readable name of the format type.';


--
-- Name: COLUMN date_format_type.locked; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN date_format_type.locked IS 'Whether or not this is a system provided format.';


--
-- Name: date_formats; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE date_formats (
    dfid integer NOT NULL,
    format character varying(100) NOT NULL,
    type character varying(64) NOT NULL,
    locked smallint DEFAULT 0 NOT NULL,
    CONSTRAINT date_formats_dfid_check CHECK ((dfid >= 0))
);


--
-- Name: TABLE date_formats; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE date_formats IS 'Stores configured date formats.';


--
-- Name: COLUMN date_formats.dfid; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN date_formats.dfid IS 'The date format identifier.';


--
-- Name: COLUMN date_formats.format; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN date_formats.format IS 'The date format string.';


--
-- Name: COLUMN date_formats.type; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN date_formats.type IS 'The date format type, e.g. medium.';


--
-- Name: COLUMN date_formats.locked; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN date_formats.locked IS 'Whether or not this format can be modified.';


--
-- Name: date_formats_dfid_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE date_formats_dfid_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


--
-- Name: date_formats_dfid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: -
--

ALTER SEQUENCE date_formats_dfid_seq OWNED BY date_formats.dfid;


--
-- Name: do_group; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE do_group (
    group_id integer NOT NULL,
    name character varying(100) NOT NULL,
    tree_json text,
    group_node_id integer
);


--
-- Name: do_group_group_id_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE do_group_group_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


--
-- Name: do_group_group_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: -
--

ALTER SEQUENCE do_group_group_id_seq OWNED BY do_group.group_id;


--
-- Name: do_node; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE do_node (
    node_id integer NOT NULL,
    name character varying(100) NOT NULL,
    type character varying(100) NOT NULL,
    prop text,
    rank integer,
    description text
);


--
-- Name: do_node_node_id_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE do_node_node_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


--
-- Name: do_node_node_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: -
--

ALTER SEQUENCE do_node_node_id_seq OWNED BY do_node.node_id;


--
-- Name: do_node_relationship; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE do_node_relationship (
    relationship_id integer NOT NULL,
    parent_id integer NOT NULL,
    child_id integer NOT NULL
);


--
-- Name: do_node_relationship_relationship_id_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE do_node_relationship_relationship_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


--
-- Name: do_node_relationship_relationship_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: -
--

ALTER SEQUENCE do_node_relationship_relationship_id_seq OWNED BY do_node_relationship.relationship_id;


--
-- Name: do_overview; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE do_overview (
    overview_id integer NOT NULL,
    group_id integer NOT NULL,
    description text,
    submit_date timestamp without time zone,
    filename character varying(100) NOT NULL,
    status character varying(100)
);


--
-- Name: do_overview_overview_id_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE do_overview_overview_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


--
-- Name: do_overview_overview_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: -
--

ALTER SEQUENCE do_overview_overview_id_seq OWNED BY do_overview.overview_id;


--
-- Name: do_user; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE do_user (
    user_id integer NOT NULL,
    name character varying(255) NOT NULL,
    uid character varying(255) NOT NULL,
    mail character varying(255) NOT NULL,
    prop text
);


--
-- Name: field_config; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE field_config (
    id integer NOT NULL,
    field_name character varying(32) NOT NULL,
    type character varying(128) NOT NULL,
    module character varying(128) DEFAULT ''::character varying NOT NULL,
    active smallint DEFAULT 0 NOT NULL,
    storage_type character varying(128) NOT NULL,
    storage_module character varying(128) DEFAULT ''::character varying NOT NULL,
    storage_active smallint DEFAULT 0 NOT NULL,
    locked smallint DEFAULT 0 NOT NULL,
    data bytea NOT NULL,
    cardinality smallint DEFAULT 0 NOT NULL,
    translatable smallint DEFAULT 0 NOT NULL,
    deleted smallint DEFAULT 0 NOT NULL
);


--
-- Name: COLUMN field_config.id; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN field_config.id IS 'The primary identifier for a field';


--
-- Name: COLUMN field_config.field_name; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN field_config.field_name IS 'The name of this field. Non-deleted field names are unique, but multiple deleted fields can have the same name.';


--
-- Name: COLUMN field_config.type; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN field_config.type IS 'The type of this field.';


--
-- Name: COLUMN field_config.module; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN field_config.module IS 'The module that implements the field type.';


--
-- Name: COLUMN field_config.active; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN field_config.active IS 'Boolean indicating whether the module that implements the field type is enabled.';


--
-- Name: COLUMN field_config.storage_type; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN field_config.storage_type IS 'The storage backend for the field.';


--
-- Name: COLUMN field_config.storage_module; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN field_config.storage_module IS 'The module that implements the storage backend.';


--
-- Name: COLUMN field_config.storage_active; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN field_config.storage_active IS 'Boolean indicating whether the module that implements the storage backend is enabled.';


--
-- Name: COLUMN field_config.locked; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN field_config.locked IS '@TODO';


--
-- Name: COLUMN field_config.data; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN field_config.data IS 'Serialized data containing the field properties that do not warrant a dedicated column.';


--
-- Name: field_config_id_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE field_config_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


--
-- Name: field_config_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: -
--

ALTER SEQUENCE field_config_id_seq OWNED BY field_config.id;


--
-- Name: field_config_instance; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE field_config_instance (
    id integer NOT NULL,
    field_id integer NOT NULL,
    field_name character varying(32) DEFAULT ''::character varying NOT NULL,
    entity_type character varying(32) DEFAULT ''::character varying NOT NULL,
    bundle character varying(128) DEFAULT ''::character varying NOT NULL,
    data bytea NOT NULL,
    deleted smallint DEFAULT 0 NOT NULL
);


--
-- Name: COLUMN field_config_instance.id; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN field_config_instance.id IS 'The primary identifier for a field instance';


--
-- Name: COLUMN field_config_instance.field_id; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN field_config_instance.field_id IS 'The identifier of the field attached by this instance';


--
-- Name: field_config_instance_id_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE field_config_instance_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


--
-- Name: field_config_instance_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: -
--

ALTER SEQUENCE field_config_instance_id_seq OWNED BY field_config_instance.id;


--
-- Name: field_data_body; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE field_data_body (
    entity_type character varying(128) DEFAULT ''::character varying NOT NULL,
    bundle character varying(128) DEFAULT ''::character varying NOT NULL,
    deleted smallint DEFAULT 0 NOT NULL,
    entity_id bigint NOT NULL,
    revision_id bigint,
    language character varying(32) DEFAULT ''::character varying NOT NULL,
    delta bigint NOT NULL,
    body_value text,
    body_summary text,
    body_format character varying(255),
    CONSTRAINT field_data_body_delta_check CHECK ((delta >= 0)),
    CONSTRAINT field_data_body_entity_id_check CHECK ((entity_id >= 0)),
    CONSTRAINT field_data_body_revision_id_check CHECK ((revision_id >= 0))
);


--
-- Name: TABLE field_data_body; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE field_data_body IS 'Data storage for field 2 (body)';


--
-- Name: COLUMN field_data_body.entity_type; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN field_data_body.entity_type IS 'The entity type this data is attached to';


--
-- Name: COLUMN field_data_body.bundle; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN field_data_body.bundle IS 'The field instance bundle to which this row belongs, used when deleting a field instance';


--
-- Name: COLUMN field_data_body.deleted; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN field_data_body.deleted IS 'A boolean indicating whether this data item has been deleted';


--
-- Name: COLUMN field_data_body.entity_id; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN field_data_body.entity_id IS 'The entity id this data is attached to';


--
-- Name: COLUMN field_data_body.revision_id; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN field_data_body.revision_id IS 'The entity revision id this data is attached to, or NULL if the entity type is not versioned';


--
-- Name: COLUMN field_data_body.language; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN field_data_body.language IS 'The language for this data item.';


--
-- Name: COLUMN field_data_body.delta; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN field_data_body.delta IS 'The sequence number for this data item, used for multi-value fields';


--
-- Name: field_data_comment_body; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE field_data_comment_body (
    entity_type character varying(128) DEFAULT ''::character varying NOT NULL,
    bundle character varying(128) DEFAULT ''::character varying NOT NULL,
    deleted smallint DEFAULT 0 NOT NULL,
    entity_id bigint NOT NULL,
    revision_id bigint,
    language character varying(32) DEFAULT ''::character varying NOT NULL,
    delta bigint NOT NULL,
    comment_body_value text,
    comment_body_format character varying(255),
    CONSTRAINT field_data_comment_body_delta_check CHECK ((delta >= 0)),
    CONSTRAINT field_data_comment_body_entity_id_check CHECK ((entity_id >= 0)),
    CONSTRAINT field_data_comment_body_revision_id_check CHECK ((revision_id >= 0))
);


--
-- Name: TABLE field_data_comment_body; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE field_data_comment_body IS 'Data storage for field 1 (comment_body)';


--
-- Name: COLUMN field_data_comment_body.entity_type; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN field_data_comment_body.entity_type IS 'The entity type this data is attached to';


--
-- Name: COLUMN field_data_comment_body.bundle; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN field_data_comment_body.bundle IS 'The field instance bundle to which this row belongs, used when deleting a field instance';


--
-- Name: COLUMN field_data_comment_body.deleted; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN field_data_comment_body.deleted IS 'A boolean indicating whether this data item has been deleted';


--
-- Name: COLUMN field_data_comment_body.entity_id; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN field_data_comment_body.entity_id IS 'The entity id this data is attached to';


--
-- Name: COLUMN field_data_comment_body.revision_id; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN field_data_comment_body.revision_id IS 'The entity revision id this data is attached to, or NULL if the entity type is not versioned';


--
-- Name: COLUMN field_data_comment_body.language; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN field_data_comment_body.language IS 'The language for this data item.';


--
-- Name: COLUMN field_data_comment_body.delta; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN field_data_comment_body.delta IS 'The sequence number for this data item, used for multi-value fields';


--
-- Name: field_data_field_first_anme; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE field_data_field_first_anme (
    entity_type character varying(128) DEFAULT ''::character varying NOT NULL,
    bundle character varying(128) DEFAULT ''::character varying NOT NULL,
    deleted smallint DEFAULT 0 NOT NULL,
    entity_id bigint NOT NULL,
    revision_id bigint,
    language character varying(32) DEFAULT ''::character varying NOT NULL,
    delta bigint NOT NULL,
    field_first_anme_value character varying(255),
    field_first_anme_format character varying(255),
    CONSTRAINT field_data_field_first_anme_delta_check CHECK ((delta >= 0)),
    CONSTRAINT field_data_field_first_anme_entity_id_check CHECK ((entity_id >= 0)),
    CONSTRAINT field_data_field_first_anme_revision_id_check CHECK ((revision_id >= 0))
);


--
-- Name: TABLE field_data_field_first_anme; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE field_data_field_first_anme IS 'Data storage for field 5 (field_first_anme)';


--
-- Name: COLUMN field_data_field_first_anme.entity_type; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN field_data_field_first_anme.entity_type IS 'The entity type this data is attached to';


--
-- Name: COLUMN field_data_field_first_anme.bundle; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN field_data_field_first_anme.bundle IS 'The field instance bundle to which this row belongs, used when deleting a field instance';


--
-- Name: COLUMN field_data_field_first_anme.deleted; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN field_data_field_first_anme.deleted IS 'A boolean indicating whether this data item has been deleted';


--
-- Name: COLUMN field_data_field_first_anme.entity_id; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN field_data_field_first_anme.entity_id IS 'The entity id this data is attached to';


--
-- Name: COLUMN field_data_field_first_anme.revision_id; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN field_data_field_first_anme.revision_id IS 'The entity revision id this data is attached to, or NULL if the entity type is not versioned';


--
-- Name: COLUMN field_data_field_first_anme.language; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN field_data_field_first_anme.language IS 'The language for this data item.';


--
-- Name: COLUMN field_data_field_first_anme.delta; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN field_data_field_first_anme.delta IS 'The sequence number for this data item, used for multi-value fields';


--
-- Name: field_data_field_image; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE field_data_field_image (
    entity_type character varying(128) DEFAULT ''::character varying NOT NULL,
    bundle character varying(128) DEFAULT ''::character varying NOT NULL,
    deleted smallint DEFAULT 0 NOT NULL,
    entity_id bigint NOT NULL,
    revision_id bigint,
    language character varying(32) DEFAULT ''::character varying NOT NULL,
    delta bigint NOT NULL,
    field_image_fid bigint,
    field_image_alt character varying(512),
    field_image_title character varying(1024),
    field_image_width bigint,
    field_image_height bigint,
    CONSTRAINT field_data_field_image_delta_check CHECK ((delta >= 0)),
    CONSTRAINT field_data_field_image_entity_id_check CHECK ((entity_id >= 0)),
    CONSTRAINT field_data_field_image_field_image_fid_check CHECK ((field_image_fid >= 0)),
    CONSTRAINT field_data_field_image_field_image_height_check CHECK ((field_image_height >= 0)),
    CONSTRAINT field_data_field_image_field_image_width_check CHECK ((field_image_width >= 0)),
    CONSTRAINT field_data_field_image_revision_id_check CHECK ((revision_id >= 0))
);


--
-- Name: TABLE field_data_field_image; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE field_data_field_image IS 'Data storage for field 4 (field_image)';


--
-- Name: COLUMN field_data_field_image.entity_type; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN field_data_field_image.entity_type IS 'The entity type this data is attached to';


--
-- Name: COLUMN field_data_field_image.bundle; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN field_data_field_image.bundle IS 'The field instance bundle to which this row belongs, used when deleting a field instance';


--
-- Name: COLUMN field_data_field_image.deleted; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN field_data_field_image.deleted IS 'A boolean indicating whether this data item has been deleted';


--
-- Name: COLUMN field_data_field_image.entity_id; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN field_data_field_image.entity_id IS 'The entity id this data is attached to';


--
-- Name: COLUMN field_data_field_image.revision_id; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN field_data_field_image.revision_id IS 'The entity revision id this data is attached to, or NULL if the entity type is not versioned';


--
-- Name: COLUMN field_data_field_image.language; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN field_data_field_image.language IS 'The language for this data item.';


--
-- Name: COLUMN field_data_field_image.delta; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN field_data_field_image.delta IS 'The sequence number for this data item, used for multi-value fields';


--
-- Name: COLUMN field_data_field_image.field_image_fid; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN field_data_field_image.field_image_fid IS 'The file_managed.fid being referenced in this field.';


--
-- Name: COLUMN field_data_field_image.field_image_alt; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN field_data_field_image.field_image_alt IS 'Alternative image text, for the image''s ''alt'' attribute.';


--
-- Name: COLUMN field_data_field_image.field_image_title; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN field_data_field_image.field_image_title IS 'Image title text, for the image''s ''title'' attribute.';


--
-- Name: COLUMN field_data_field_image.field_image_width; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN field_data_field_image.field_image_width IS 'The width of the image in pixels.';


--
-- Name: COLUMN field_data_field_image.field_image_height; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN field_data_field_image.field_image_height IS 'The height of the image in pixels.';


--
-- Name: field_data_field_tags; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE field_data_field_tags (
    entity_type character varying(128) DEFAULT ''::character varying NOT NULL,
    bundle character varying(128) DEFAULT ''::character varying NOT NULL,
    deleted smallint DEFAULT 0 NOT NULL,
    entity_id bigint NOT NULL,
    revision_id bigint,
    language character varying(32) DEFAULT ''::character varying NOT NULL,
    delta bigint NOT NULL,
    field_tags_tid bigint,
    CONSTRAINT field_data_field_tags_delta_check CHECK ((delta >= 0)),
    CONSTRAINT field_data_field_tags_entity_id_check CHECK ((entity_id >= 0)),
    CONSTRAINT field_data_field_tags_field_tags_tid_check CHECK ((field_tags_tid >= 0)),
    CONSTRAINT field_data_field_tags_revision_id_check CHECK ((revision_id >= 0))
);


--
-- Name: TABLE field_data_field_tags; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE field_data_field_tags IS 'Data storage for field 3 (field_tags)';


--
-- Name: COLUMN field_data_field_tags.entity_type; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN field_data_field_tags.entity_type IS 'The entity type this data is attached to';


--
-- Name: COLUMN field_data_field_tags.bundle; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN field_data_field_tags.bundle IS 'The field instance bundle to which this row belongs, used when deleting a field instance';


--
-- Name: COLUMN field_data_field_tags.deleted; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN field_data_field_tags.deleted IS 'A boolean indicating whether this data item has been deleted';


--
-- Name: COLUMN field_data_field_tags.entity_id; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN field_data_field_tags.entity_id IS 'The entity id this data is attached to';


--
-- Name: COLUMN field_data_field_tags.revision_id; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN field_data_field_tags.revision_id IS 'The entity revision id this data is attached to, or NULL if the entity type is not versioned';


--
-- Name: COLUMN field_data_field_tags.language; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN field_data_field_tags.language IS 'The language for this data item.';


--
-- Name: COLUMN field_data_field_tags.delta; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN field_data_field_tags.delta IS 'The sequence number for this data item, used for multi-value fields';


--
-- Name: field_revision_body; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE field_revision_body (
    entity_type character varying(128) DEFAULT ''::character varying NOT NULL,
    bundle character varying(128) DEFAULT ''::character varying NOT NULL,
    deleted smallint DEFAULT 0 NOT NULL,
    entity_id bigint NOT NULL,
    revision_id bigint NOT NULL,
    language character varying(32) DEFAULT ''::character varying NOT NULL,
    delta bigint NOT NULL,
    body_value text,
    body_summary text,
    body_format character varying(255),
    CONSTRAINT field_revision_body_delta_check CHECK ((delta >= 0)),
    CONSTRAINT field_revision_body_entity_id_check CHECK ((entity_id >= 0)),
    CONSTRAINT field_revision_body_revision_id_check CHECK ((revision_id >= 0))
);


--
-- Name: TABLE field_revision_body; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE field_revision_body IS 'Revision archive storage for field 2 (body)';


--
-- Name: COLUMN field_revision_body.entity_type; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN field_revision_body.entity_type IS 'The entity type this data is attached to';


--
-- Name: COLUMN field_revision_body.bundle; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN field_revision_body.bundle IS 'The field instance bundle to which this row belongs, used when deleting a field instance';


--
-- Name: COLUMN field_revision_body.deleted; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN field_revision_body.deleted IS 'A boolean indicating whether this data item has been deleted';


--
-- Name: COLUMN field_revision_body.entity_id; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN field_revision_body.entity_id IS 'The entity id this data is attached to';


--
-- Name: COLUMN field_revision_body.revision_id; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN field_revision_body.revision_id IS 'The entity revision id this data is attached to';


--
-- Name: COLUMN field_revision_body.language; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN field_revision_body.language IS 'The language for this data item.';


--
-- Name: COLUMN field_revision_body.delta; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN field_revision_body.delta IS 'The sequence number for this data item, used for multi-value fields';


--
-- Name: field_revision_comment_body; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE field_revision_comment_body (
    entity_type character varying(128) DEFAULT ''::character varying NOT NULL,
    bundle character varying(128) DEFAULT ''::character varying NOT NULL,
    deleted smallint DEFAULT 0 NOT NULL,
    entity_id bigint NOT NULL,
    revision_id bigint NOT NULL,
    language character varying(32) DEFAULT ''::character varying NOT NULL,
    delta bigint NOT NULL,
    comment_body_value text,
    comment_body_format character varying(255),
    CONSTRAINT field_revision_comment_body_delta_check CHECK ((delta >= 0)),
    CONSTRAINT field_revision_comment_body_entity_id_check CHECK ((entity_id >= 0)),
    CONSTRAINT field_revision_comment_body_revision_id_check CHECK ((revision_id >= 0))
);


--
-- Name: TABLE field_revision_comment_body; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE field_revision_comment_body IS 'Revision archive storage for field 1 (comment_body)';


--
-- Name: COLUMN field_revision_comment_body.entity_type; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN field_revision_comment_body.entity_type IS 'The entity type this data is attached to';


--
-- Name: COLUMN field_revision_comment_body.bundle; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN field_revision_comment_body.bundle IS 'The field instance bundle to which this row belongs, used when deleting a field instance';


--
-- Name: COLUMN field_revision_comment_body.deleted; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN field_revision_comment_body.deleted IS 'A boolean indicating whether this data item has been deleted';


--
-- Name: COLUMN field_revision_comment_body.entity_id; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN field_revision_comment_body.entity_id IS 'The entity id this data is attached to';


--
-- Name: COLUMN field_revision_comment_body.revision_id; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN field_revision_comment_body.revision_id IS 'The entity revision id this data is attached to';


--
-- Name: COLUMN field_revision_comment_body.language; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN field_revision_comment_body.language IS 'The language for this data item.';


--
-- Name: COLUMN field_revision_comment_body.delta; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN field_revision_comment_body.delta IS 'The sequence number for this data item, used for multi-value fields';


--
-- Name: field_revision_field_first_anme; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE field_revision_field_first_anme (
    entity_type character varying(128) DEFAULT ''::character varying NOT NULL,
    bundle character varying(128) DEFAULT ''::character varying NOT NULL,
    deleted smallint DEFAULT 0 NOT NULL,
    entity_id bigint NOT NULL,
    revision_id bigint NOT NULL,
    language character varying(32) DEFAULT ''::character varying NOT NULL,
    delta bigint NOT NULL,
    field_first_anme_value character varying(255),
    field_first_anme_format character varying(255),
    CONSTRAINT field_revision_field_first_anme_delta_check CHECK ((delta >= 0)),
    CONSTRAINT field_revision_field_first_anme_entity_id_check CHECK ((entity_id >= 0)),
    CONSTRAINT field_revision_field_first_anme_revision_id_check CHECK ((revision_id >= 0))
);


--
-- Name: TABLE field_revision_field_first_anme; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE field_revision_field_first_anme IS 'Revision archive storage for field 5 (field_first_anme)';


--
-- Name: COLUMN field_revision_field_first_anme.entity_type; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN field_revision_field_first_anme.entity_type IS 'The entity type this data is attached to';


--
-- Name: COLUMN field_revision_field_first_anme.bundle; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN field_revision_field_first_anme.bundle IS 'The field instance bundle to which this row belongs, used when deleting a field instance';


--
-- Name: COLUMN field_revision_field_first_anme.deleted; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN field_revision_field_first_anme.deleted IS 'A boolean indicating whether this data item has been deleted';


--
-- Name: COLUMN field_revision_field_first_anme.entity_id; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN field_revision_field_first_anme.entity_id IS 'The entity id this data is attached to';


--
-- Name: COLUMN field_revision_field_first_anme.revision_id; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN field_revision_field_first_anme.revision_id IS 'The entity revision id this data is attached to';


--
-- Name: COLUMN field_revision_field_first_anme.language; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN field_revision_field_first_anme.language IS 'The language for this data item.';


--
-- Name: COLUMN field_revision_field_first_anme.delta; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN field_revision_field_first_anme.delta IS 'The sequence number for this data item, used for multi-value fields';


--
-- Name: field_revision_field_image; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE field_revision_field_image (
    entity_type character varying(128) DEFAULT ''::character varying NOT NULL,
    bundle character varying(128) DEFAULT ''::character varying NOT NULL,
    deleted smallint DEFAULT 0 NOT NULL,
    entity_id bigint NOT NULL,
    revision_id bigint NOT NULL,
    language character varying(32) DEFAULT ''::character varying NOT NULL,
    delta bigint NOT NULL,
    field_image_fid bigint,
    field_image_alt character varying(512),
    field_image_title character varying(1024),
    field_image_width bigint,
    field_image_height bigint,
    CONSTRAINT field_revision_field_image_delta_check CHECK ((delta >= 0)),
    CONSTRAINT field_revision_field_image_entity_id_check CHECK ((entity_id >= 0)),
    CONSTRAINT field_revision_field_image_field_image_fid_check CHECK ((field_image_fid >= 0)),
    CONSTRAINT field_revision_field_image_field_image_height_check CHECK ((field_image_height >= 0)),
    CONSTRAINT field_revision_field_image_field_image_width_check CHECK ((field_image_width >= 0)),
    CONSTRAINT field_revision_field_image_revision_id_check CHECK ((revision_id >= 0))
);


--
-- Name: TABLE field_revision_field_image; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE field_revision_field_image IS 'Revision archive storage for field 4 (field_image)';


--
-- Name: COLUMN field_revision_field_image.entity_type; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN field_revision_field_image.entity_type IS 'The entity type this data is attached to';


--
-- Name: COLUMN field_revision_field_image.bundle; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN field_revision_field_image.bundle IS 'The field instance bundle to which this row belongs, used when deleting a field instance';


--
-- Name: COLUMN field_revision_field_image.deleted; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN field_revision_field_image.deleted IS 'A boolean indicating whether this data item has been deleted';


--
-- Name: COLUMN field_revision_field_image.entity_id; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN field_revision_field_image.entity_id IS 'The entity id this data is attached to';


--
-- Name: COLUMN field_revision_field_image.revision_id; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN field_revision_field_image.revision_id IS 'The entity revision id this data is attached to';


--
-- Name: COLUMN field_revision_field_image.language; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN field_revision_field_image.language IS 'The language for this data item.';


--
-- Name: COLUMN field_revision_field_image.delta; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN field_revision_field_image.delta IS 'The sequence number for this data item, used for multi-value fields';


--
-- Name: COLUMN field_revision_field_image.field_image_fid; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN field_revision_field_image.field_image_fid IS 'The file_managed.fid being referenced in this field.';


--
-- Name: COLUMN field_revision_field_image.field_image_alt; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN field_revision_field_image.field_image_alt IS 'Alternative image text, for the image''s ''alt'' attribute.';


--
-- Name: COLUMN field_revision_field_image.field_image_title; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN field_revision_field_image.field_image_title IS 'Image title text, for the image''s ''title'' attribute.';


--
-- Name: COLUMN field_revision_field_image.field_image_width; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN field_revision_field_image.field_image_width IS 'The width of the image in pixels.';


--
-- Name: COLUMN field_revision_field_image.field_image_height; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN field_revision_field_image.field_image_height IS 'The height of the image in pixels.';


--
-- Name: field_revision_field_tags; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE field_revision_field_tags (
    entity_type character varying(128) DEFAULT ''::character varying NOT NULL,
    bundle character varying(128) DEFAULT ''::character varying NOT NULL,
    deleted smallint DEFAULT 0 NOT NULL,
    entity_id bigint NOT NULL,
    revision_id bigint NOT NULL,
    language character varying(32) DEFAULT ''::character varying NOT NULL,
    delta bigint NOT NULL,
    field_tags_tid bigint,
    CONSTRAINT field_revision_field_tags_delta_check CHECK ((delta >= 0)),
    CONSTRAINT field_revision_field_tags_entity_id_check CHECK ((entity_id >= 0)),
    CONSTRAINT field_revision_field_tags_field_tags_tid_check CHECK ((field_tags_tid >= 0)),
    CONSTRAINT field_revision_field_tags_revision_id_check CHECK ((revision_id >= 0))
);


--
-- Name: TABLE field_revision_field_tags; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE field_revision_field_tags IS 'Revision archive storage for field 3 (field_tags)';


--
-- Name: COLUMN field_revision_field_tags.entity_type; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN field_revision_field_tags.entity_type IS 'The entity type this data is attached to';


--
-- Name: COLUMN field_revision_field_tags.bundle; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN field_revision_field_tags.bundle IS 'The field instance bundle to which this row belongs, used when deleting a field instance';


--
-- Name: COLUMN field_revision_field_tags.deleted; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN field_revision_field_tags.deleted IS 'A boolean indicating whether this data item has been deleted';


--
-- Name: COLUMN field_revision_field_tags.entity_id; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN field_revision_field_tags.entity_id IS 'The entity id this data is attached to';


--
-- Name: COLUMN field_revision_field_tags.revision_id; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN field_revision_field_tags.revision_id IS 'The entity revision id this data is attached to';


--
-- Name: COLUMN field_revision_field_tags.language; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN field_revision_field_tags.language IS 'The language for this data item.';


--
-- Name: COLUMN field_revision_field_tags.delta; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN field_revision_field_tags.delta IS 'The sequence number for this data item, used for multi-value fields';


--
-- Name: file_managed; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE file_managed (
    fid integer NOT NULL,
    uid bigint DEFAULT 0 NOT NULL,
    filename character varying(255) DEFAULT ''::character varying NOT NULL,
    uri character varying(255) DEFAULT ''::character varying NOT NULL,
    filemime character varying(255) DEFAULT ''::character varying NOT NULL,
    filesize bigint DEFAULT 0 NOT NULL,
    status smallint DEFAULT 0 NOT NULL,
    "timestamp" bigint DEFAULT 0 NOT NULL,
    CONSTRAINT file_managed_fid_check CHECK ((fid >= 0)),
    CONSTRAINT file_managed_filesize_check CHECK ((filesize >= 0)),
    CONSTRAINT file_managed_timestamp_check CHECK (("timestamp" >= 0)),
    CONSTRAINT file_managed_uid_check CHECK ((uid >= 0))
);


--
-- Name: TABLE file_managed; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE file_managed IS 'Stores information for uploaded files.';


--
-- Name: COLUMN file_managed.fid; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN file_managed.fid IS 'File ID.';


--
-- Name: COLUMN file_managed.uid; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN file_managed.uid IS 'The users.uid of the user who is associated with the file.';


--
-- Name: COLUMN file_managed.filename; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN file_managed.filename IS 'Name of the file with no path components. This may differ from the basename of the URI if the file is renamed to avoid overwriting an existing file.';


--
-- Name: COLUMN file_managed.uri; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN file_managed.uri IS 'The URI to access the file (either local or remote).';


--
-- Name: COLUMN file_managed.filemime; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN file_managed.filemime IS 'The file''s MIME type.';


--
-- Name: COLUMN file_managed.filesize; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN file_managed.filesize IS 'The size of the file in bytes.';


--
-- Name: COLUMN file_managed.status; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN file_managed.status IS 'A field indicating the status of the file. Two status are defined in core: temporary (0) and permanent (1). Temporary files older than DRUPAL_MAXIMUM_TEMP_FILE_AGE will be removed during a cron run.';


--
-- Name: COLUMN file_managed."timestamp"; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN file_managed."timestamp" IS 'UNIX timestamp for when the file was added.';


--
-- Name: file_managed_fid_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE file_managed_fid_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


--
-- Name: file_managed_fid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: -
--

ALTER SEQUENCE file_managed_fid_seq OWNED BY file_managed.fid;


--
-- Name: file_usage; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE file_usage (
    fid bigint NOT NULL,
    module character varying(255) DEFAULT ''::character varying NOT NULL,
    type character varying(64) DEFAULT ''::character varying NOT NULL,
    id bigint DEFAULT 0 NOT NULL,
    count bigint DEFAULT 0 NOT NULL,
    CONSTRAINT file_usage_count_check CHECK ((count >= 0)),
    CONSTRAINT file_usage_fid_check CHECK ((fid >= 0)),
    CONSTRAINT file_usage_id_check CHECK ((id >= 0))
);


--
-- Name: TABLE file_usage; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE file_usage IS 'Track where a file is used.';


--
-- Name: COLUMN file_usage.fid; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN file_usage.fid IS 'File ID.';


--
-- Name: COLUMN file_usage.module; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN file_usage.module IS 'The name of the module that is using the file.';


--
-- Name: COLUMN file_usage.type; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN file_usage.type IS 'The name of the object type in which the file is used.';


--
-- Name: COLUMN file_usage.id; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN file_usage.id IS 'The primary key of the object using the file.';


--
-- Name: COLUMN file_usage.count; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN file_usage.count IS 'The number of times this file is used by this object.';


--
-- Name: filter; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE filter (
    format character varying(255) NOT NULL,
    module character varying(64) DEFAULT ''::character varying NOT NULL,
    name character varying(32) DEFAULT ''::character varying NOT NULL,
    weight integer DEFAULT 0 NOT NULL,
    status integer DEFAULT 0 NOT NULL,
    settings bytea
);


--
-- Name: TABLE filter; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE filter IS 'Table that maps filters (HTML corrector) to text formats (Filtered HTML).';


--
-- Name: COLUMN filter.format; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN filter.format IS 'Foreign key: The filter_format.format to which this filter is assigned.';


--
-- Name: COLUMN filter.module; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN filter.module IS 'The origin module of the filter.';


--
-- Name: COLUMN filter.name; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN filter.name IS 'Name of the filter being referenced.';


--
-- Name: COLUMN filter.weight; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN filter.weight IS 'Weight of filter within format.';


--
-- Name: COLUMN filter.status; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN filter.status IS 'Filter enabled status. (1 = enabled, 0 = disabled)';


--
-- Name: COLUMN filter.settings; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN filter.settings IS 'A serialized array of name value pairs that store the filter settings for the specific format.';


--
-- Name: filter_format; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE filter_format (
    format character varying(255) NOT NULL,
    name character varying(255) DEFAULT ''::character varying NOT NULL,
    cache smallint DEFAULT 0 NOT NULL,
    status integer DEFAULT 1 NOT NULL,
    weight integer DEFAULT 0 NOT NULL,
    CONSTRAINT filter_format_status_check CHECK ((status >= 0))
);


--
-- Name: TABLE filter_format; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE filter_format IS 'Stores text formats: custom groupings of filters, such as Filtered HTML.';


--
-- Name: COLUMN filter_format.format; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN filter_format.format IS 'Primary Key: Unique machine name of the format.';


--
-- Name: COLUMN filter_format.name; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN filter_format.name IS 'Name of the text format (Filtered HTML).';


--
-- Name: COLUMN filter_format.cache; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN filter_format.cache IS 'Flag to indicate whether format is cacheable. (1 = cacheable, 0 = not cacheable)';


--
-- Name: COLUMN filter_format.status; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN filter_format.status IS 'The status of the text format. (1 = enabled, 0 = disabled)';


--
-- Name: COLUMN filter_format.weight; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN filter_format.weight IS 'Weight of text format to use when listing.';


--
-- Name: flood; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE flood (
    fid integer NOT NULL,
    event character varying(64) DEFAULT ''::character varying NOT NULL,
    identifier character varying(128) DEFAULT ''::character varying NOT NULL,
    "timestamp" integer DEFAULT 0 NOT NULL,
    expiration integer DEFAULT 0 NOT NULL
);


--
-- Name: TABLE flood; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE flood IS 'Flood controls the threshold of events, such as the number of contact attempts.';


--
-- Name: COLUMN flood.fid; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN flood.fid IS 'Unique flood event ID.';


--
-- Name: COLUMN flood.event; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN flood.event IS 'Name of event (e.g. contact).';


--
-- Name: COLUMN flood.identifier; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN flood.identifier IS 'Identifier of the visitor, such as an IP address or hostname.';


--
-- Name: COLUMN flood."timestamp"; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN flood."timestamp" IS 'Timestamp of the event.';


--
-- Name: COLUMN flood.expiration; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN flood.expiration IS 'Expiration timestamp. Expired events are purged on cron run.';


--
-- Name: flood_fid_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE flood_fid_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


--
-- Name: flood_fid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: -
--

ALTER SEQUENCE flood_fid_seq OWNED BY flood.fid;


--
-- Name: gensas_category; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE gensas_category (
    category_id integer NOT NULL,
    category_name character varying NOT NULL,
    category_label character varying NOT NULL,
    rank integer NOT NULL
);


--
-- Name: gensas_category_category_id_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE gensas_category_category_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


--
-- Name: gensas_category_category_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: -
--

ALTER SEQUENCE gensas_category_category_id_seq OWNED BY gensas_category.category_id;


--
-- Name: gensas_db; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE gensas_db (
    db_id integer NOT NULL,
    db_name character varying(100) NOT NULL,
    label character varying(100) NOT NULL,
    url character varying(100),
    url_prefix character varying(100),
    description text,
    regex_id character varying(200),
    regex_def character varying(200),
    regex_hit_id character varying(200),
    regex_hit_acc character varying(200),
    regex_hit_def character varying(200),
    regex_hit_org character varying(200)
);


--
-- Name: gensas_db_db_id_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE gensas_db_db_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


--
-- Name: gensas_db_db_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: -
--

ALTER SEQUENCE gensas_db_db_id_seq OWNED BY gensas_db.db_id;


--
-- Name: gensas_expire; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE gensas_expire (
    task_id character varying(200) NOT NULL,
    reset_date timestamp without time zone,
    last_notify_date timestamp without time zone,
    next_notify_days integer,
    exempt integer DEFAULT 0 NOT NULL
);


--
-- Name: gensas_files; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE gensas_files (
    file_id integer NOT NULL,
    user_id integer NOT NULL,
    file_name character varying(1024) NOT NULL,
    file_path text NOT NULL,
    drupal_path text NOT NULL,
    file_size bigint NOT NULL,
    type character varying(100)
);


--
-- Name: gensas_files_file_id_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE gensas_files_file_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


--
-- Name: gensas_files_file_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: -
--

ALTER SEQUENCE gensas_files_file_id_seq OWNED BY gensas_files.file_id;


--
-- Name: gensas_gff3; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE gensas_gff3 (
    job_id character varying(200) NOT NULL,
    prop text,
    id character varying(255) NOT NULL,
    type character varying(255) NOT NULL,
    start integer,
    parent character varying(255),
    landmark character varying(1024) NOT NULL
);


--
-- Name: gensas_gff3_location_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE gensas_gff3_location_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


--
-- Name: gensas_gff3_location_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: -
--

ALTER SEQUENCE gensas_gff3_location_seq OWNED BY gensas_gff3.start;


--
-- Name: gensas_group; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE gensas_group (
    group_id integer NOT NULL,
    group_name character varying(1024) NOT NULL
);


--
-- Name: gensas_group_group_id_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE gensas_group_group_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


--
-- Name: gensas_group_group_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: -
--

ALTER SEQUENCE gensas_group_group_id_seq OWNED BY gensas_group.group_id;


--
-- Name: gensas_group_seq; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE gensas_group_seq (
    group_id integer NOT NULL,
    seq_id integer NOT NULL,
    type character varying(100) DEFAULT 'unknown'::character varying NOT NULL
);


--
-- Name: gensas_group_task; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE gensas_group_task (
    group_id integer NOT NULL,
    task_id character varying(200) NOT NULL
);


--
-- Name: gensas_group_user; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE gensas_group_user (
    group_id integer NOT NULL,
    user_id integer NOT NULL,
    leader integer DEFAULT 0 NOT NULL,
    read_only integer DEFAULT 0 NOT NULL
);


--
-- Name: gensas_job; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE gensas_job (
    job_id character varying(200) NOT NULL,
    id integer NOT NULL,
    type character varying(50) NOT NULL,
    output text,
    create_date timestamp without time zone,
    submit_date timestamp without time zone,
    complete_date timestamp without time zone,
    log text,
    status integer,
    params text,
    output_parsed text,
    source character varying(1024),
    tool_id integer,
    stderr text,
    retval character varying(50),
    num_run integer,
    name character varying(100),
    prop text
);


--
-- Name: gensas_job_execid; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE gensas_job_execid (
    job_id character varying(200) NOT NULL,
    resource_id integer NOT NULL,
    exec_id integer NOT NULL
);


--
-- Name: gensas_job_files; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE gensas_job_files (
    job_id character varying(200) NOT NULL,
    file_id integer
);


--
-- Name: gensas_job_id_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE gensas_job_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


--
-- Name: gensas_job_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: -
--

ALTER SEQUENCE gensas_job_id_seq OWNED BY gensas_job.id;


--
-- Name: gensas_job_resource; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE gensas_job_resource (
    resource_id integer NOT NULL,
    job_id character varying(200) NOT NULL
);


--
-- Name: gensas_job_stats; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE gensas_job_stats (
    job_id character varying(200) NOT NULL,
    type character varying(50) NOT NULL,
    num_features integer
);


--
-- Name: gensas_library; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE gensas_library (
    library_id integer NOT NULL,
    name character varying(300) NOT NULL,
    type_id integer NOT NULL,
    label character varying(300) NOT NULL
);


--
-- Name: gensas_library_library_id_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE gensas_library_library_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


--
-- Name: gensas_library_library_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: -
--

ALTER SEQUENCE gensas_library_library_id_seq OWNED BY gensas_library.library_id;


--
-- Name: gensas_library_type; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE gensas_library_type (
    library_type_id integer NOT NULL,
    type character varying(300) NOT NULL,
    label character varying(300) NOT NULL
);


--
-- Name: gensas_library_type_library_type_id_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE gensas_library_type_library_type_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


--
-- Name: gensas_library_type_library_type_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: -
--

ALTER SEQUENCE gensas_library_type_library_type_id_seq OWNED BY gensas_library_type.library_type_id;


--
-- Name: gensas_param_group; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE gensas_param_group (
    param_group_id integer NOT NULL,
    name character varying(128) NOT NULL,
    tool_id integer NOT NULL,
    title character varying(255) NOT NULL,
    description text,
    prop text
);


--
-- Name: COLUMN gensas_param_group.name; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN gensas_param_group.name IS 'a computer readable name for the group';


--
-- Name: COLUMN gensas_param_group.title; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN gensas_param_group.title IS 'a human readable name for the group';


--
-- Name: gensas_param_group_param_group_id_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE gensas_param_group_param_group_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


--
-- Name: gensas_param_group_param_group_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: -
--

ALTER SEQUENCE gensas_param_group_param_group_id_seq OWNED BY gensas_param_group.param_group_id;


--
-- Name: gensas_project_type; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE gensas_project_type (
    project_type_id integer NOT NULL,
    type character varying(200) NOT NULL,
    name character varying(200) NOT NULL
);


--
-- Name: gensas_project_type_project_type_id_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE gensas_project_type_project_type_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


--
-- Name: gensas_project_type_project_type_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: -
--

ALTER SEQUENCE gensas_project_type_project_type_id_seq OWNED BY gensas_project_type.project_type_id;


--
-- Name: gensas_publish; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE gensas_publish (
    job_id character varying(200) NOT NULL,
    version integer NOT NULL
);


--
-- Name: gensas_resource; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE gensas_resource (
    resource_id integer NOT NULL,
    name character varying(150) NOT NULL,
    type character varying(10) NOT NULL,
    max_slots integer DEFAULT 1 NOT NULL,
    rank integer NOT NULL,
    enabled integer DEFAULT 1 NOT NULL,
    working_directory text,
    description text,
    seqlib_directory text
);


--
-- Name: COLUMN gensas_resource.type; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN gensas_resource.type IS 'The type of resource (e.g. local, ge or pbs).';


--
-- Name: COLUMN gensas_resource.max_slots; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN gensas_resource.max_slots IS 'The number of free slots on this resource.';


--
-- Name: COLUMN gensas_resource.rank; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN gensas_resource.rank IS 'An integer indicating the order in which resrouces should be selected for use. A lower rank indicates higher priority.';


--
-- Name: COLUMN gensas_resource.enabled; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN gensas_resource.enabled IS 'Set to 0 if this resource should not be used.';


--
-- Name: COLUMN gensas_resource.working_directory; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN gensas_resource.working_directory IS 'The directory on the resource where GenSAS will store temporary needed for and created during execution.';


--
-- Name: COLUMN gensas_resource.description; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN gensas_resource.description IS 'Provides a brief description about this .';


--
-- Name: gensas_resource_library; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE gensas_resource_library (
    resource_id integer NOT NULL,
    library_id character varying(200) NOT NULL,
    file_path text NOT NULL
);


--
-- Name: gensas_resource_resource_id_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE gensas_resource_resource_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


--
-- Name: gensas_resource_resource_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: -
--

ALTER SEQUENCE gensas_resource_resource_id_seq OWNED BY gensas_resource.resource_id;


--
-- Name: gensas_resource_submit; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE gensas_resource_submit (
    resource_id integer NOT NULL,
    hostname character varying(200) NOT NULL,
    ssh_port integer NOT NULL,
    username character varying(200) NOT NULL
);


--
-- Name: gensas_resource_tool; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE gensas_resource_tool (
    resource_id integer NOT NULL,
    tool_id integer NOT NULL,
    tool_path text NOT NULL
);


--
-- Name: gensas_seq; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE gensas_seq (
    seq_id integer NOT NULL,
    seq_group_id integer NOT NULL,
    create_date timestamp without time zone NOT NULL,
    name text NOT NULL,
    user_id integer NOT NULL,
    description text
);


--
-- Name: gensas_seq_group; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE gensas_seq_group (
    seq_group_id integer NOT NULL,
    user_id integer NOT NULL,
    name character varying(255) NOT NULL,
    type character varying(100) NOT NULL,
    filename character varying(1028) DEFAULT 'unknown'::character varying NOT NULL,
    num_seqs integer DEFAULT 0 NOT NULL,
    version character varying(25) DEFAULT ''::character varying NOT NULL,
    filtered text,
    status text,
    filesize integer DEFAULT 0 NOT NULL,
    submit_date timestamp without time zone
);


--
-- Name: gensas_seq_group_seq_group_id_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE gensas_seq_group_seq_group_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


--
-- Name: gensas_seq_group_seq_group_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: -
--

ALTER SEQUENCE gensas_seq_group_seq_group_id_seq OWNED BY gensas_seq_group.seq_group_id;


--
-- Name: gensas_seq_seq_id_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE gensas_seq_seq_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


--
-- Name: gensas_seq_seq_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: -
--

ALTER SEQUENCE gensas_seq_seq_id_seq OWNED BY gensas_seq.seq_id;


--
-- Name: gensas_seq_stats; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE gensas_seq_stats (
    job_id character varying(200) NOT NULL,
    seq_id integer,
    type character varying(50) NOT NULL,
    num_features integer
);


--
-- Name: gensas_task; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE gensas_task (
    task_id character varying(200) NOT NULL,
    task_name character varying(50),
    submit_date timestamp without time zone,
    complete_date timestamp without time zone,
    log text,
    user_id integer,
    status character varying(50),
    task_info text,
    prop text
);


--
-- Name: gensas_task_files; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE gensas_task_files (
    task_id character varying(200) NOT NULL,
    file_id integer
);


--
-- Name: gensas_task_job; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE gensas_task_job (
    task_id character varying(200) NOT NULL,
    job_id character varying(200) NOT NULL,
    is_owner smallint DEFAULT 1 NOT NULL
);


--
-- Name: gensas_task_seq_group; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE gensas_task_seq_group (
    task_id character varying(200) NOT NULL,
    seq_group_id integer
);


--
-- Name: gensas_task_user; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE gensas_task_user (
    task_id character varying(200) NOT NULL,
    user_id integer NOT NULL,
    permission character varying(200) DEFAULT 'readonly'::character varying NOT NULL
);


--
-- Name: gensas_tool; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE gensas_tool (
    tool_id integer NOT NULL,
    tool_name character varying(100) NOT NULL,
    tool_label character varying(100) NOT NULL,
    description text,
    outfiles_desc text,
    url character varying(100),
    is_enabled smallint DEFAULT 1 NOT NULL,
    category_id integer NOT NULL,
    tool_path character varying,
    profile_num integer DEFAULT 1 NOT NULL,
    version character varying(50),
    use_masked integer DEFAULT 1 NOT NULL
);


--
-- Name: COLUMN gensas_tool.profile_num; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN gensas_tool.profile_num IS 'Indicates the installed profile for this tool. This number should be increased when updates are made to the values of this table. The GenSAS Tool installer will check this field with the profile in the GenSASToolExec class. If the profile_number in the record differs from the profile_num provided in the install() function for the tool, then the record will be updated.';


--
-- Name: gensas_tool_param; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE gensas_tool_param (
    tool_param_id integer NOT NULL,
    tool_id integer NOT NULL,
    param_elem_id character varying NOT NULL,
    param_label character varying NOT NULL,
    param_type character varying NOT NULL,
    param_value text,
    param_attr character varying,
    param_default character varying,
    rank integer,
    cmd_prefix character varying(30),
    cmd_surfix character varying(30),
    param_note character varying(1024),
    required integer,
    param_group_id integer
);


--
-- Name: gensas_tool_param_tool_param_id_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE gensas_tool_param_tool_param_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


--
-- Name: gensas_tool_param_tool_param_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: -
--

ALTER SEQUENCE gensas_tool_param_tool_param_id_seq OWNED BY gensas_tool_param.tool_param_id;


--
-- Name: gensas_tool_param_type; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE gensas_tool_param_type (
    tool_param_type_id integer NOT NULL,
    type character varying(300) NOT NULL,
    label character varying(300) NOT NULL,
    description text
);


--
-- Name: gensas_tool_param_type_tool_param_type_id_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE gensas_tool_param_type_tool_param_type_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


--
-- Name: gensas_tool_param_type_tool_param_type_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: -
--

ALTER SEQUENCE gensas_tool_param_type_tool_param_type_id_seq OWNED BY gensas_tool_param_type.tool_param_type_id;


--
-- Name: gensas_tool_tool_id_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE gensas_tool_tool_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


--
-- Name: gensas_tool_tool_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: -
--

ALTER SEQUENCE gensas_tool_tool_id_seq OWNED BY gensas_tool.tool_id;


--
-- Name: gensas_tool_type; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE gensas_tool_type (
    tool_type_id integer NOT NULL,
    project_type_id integer NOT NULL,
    tool_id integer NOT NULL
);


--
-- Name: TABLE gensas_tool_type; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE gensas_tool_type IS 'Associates tools with the type of project they are appropriate for.';


--
-- Name: gensas_tool_type_tool_type_id_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE gensas_tool_type_tool_type_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


--
-- Name: gensas_tool_type_tool_type_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: -
--

ALTER SEQUENCE gensas_tool_type_tool_type_id_seq OWNED BY gensas_tool_type.tool_type_id;


--
-- Name: gensas_usage; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE gensas_usage (
    user_id integer NOT NULL,
    apollo bigint DEFAULT 0 NOT NULL,
    project bigint DEFAULT 0 NOT NULL,
    file bigint DEFAULT 0 NOT NULL,
    update_date timestamp without time zone,
    prop text,
    total bigint DEFAULT 0 NOT NULL
);


--
-- Name: gensas_user_tool; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE gensas_user_tool (
    user_id integer NOT NULL,
    tool_id integer NOT NULL,
    status character varying
);


--
-- Name: gensas_userprop; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE gensas_userprop (
    user_id integer NOT NULL,
    prop text
);


--
-- Name: history; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE history (
    uid integer DEFAULT 0 NOT NULL,
    nid bigint DEFAULT 0 NOT NULL,
    "timestamp" integer DEFAULT 0 NOT NULL,
    CONSTRAINT history_nid_check CHECK ((nid >= 0))
);


--
-- Name: TABLE history; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE history IS 'A record of which users have read which nodes.';


--
-- Name: COLUMN history.uid; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN history.uid IS 'The users.uid that read the node nid.';


--
-- Name: COLUMN history.nid; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN history.nid IS 'The node.nid that was read.';


--
-- Name: COLUMN history."timestamp"; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN history."timestamp" IS 'The Unix timestamp at which the read occurred.';


--
-- Name: image_effects; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE image_effects (
    ieid integer NOT NULL,
    isid bigint DEFAULT 0 NOT NULL,
    weight integer DEFAULT 0 NOT NULL,
    name character varying(255) NOT NULL,
    data bytea NOT NULL,
    CONSTRAINT image_effects_ieid_check CHECK ((ieid >= 0)),
    CONSTRAINT image_effects_isid_check CHECK ((isid >= 0))
);


--
-- Name: TABLE image_effects; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE image_effects IS 'Stores configuration options for image effects.';


--
-- Name: COLUMN image_effects.ieid; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN image_effects.ieid IS 'The primary identifier for an image effect.';


--
-- Name: COLUMN image_effects.isid; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN image_effects.isid IS 'The image_styles.isid for an image style.';


--
-- Name: COLUMN image_effects.weight; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN image_effects.weight IS 'The weight of the effect in the style.';


--
-- Name: COLUMN image_effects.name; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN image_effects.name IS 'The unique name of the effect to be executed.';


--
-- Name: COLUMN image_effects.data; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN image_effects.data IS 'The configuration data for the effect.';


--
-- Name: image_effects_ieid_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE image_effects_ieid_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


--
-- Name: image_effects_ieid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: -
--

ALTER SEQUENCE image_effects_ieid_seq OWNED BY image_effects.ieid;


--
-- Name: image_styles; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE image_styles (
    isid integer NOT NULL,
    name character varying(255) NOT NULL,
    label character varying(255) DEFAULT ''::character varying NOT NULL,
    CONSTRAINT image_styles_isid_check CHECK ((isid >= 0))
);


--
-- Name: TABLE image_styles; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE image_styles IS 'Stores configuration options for image styles.';


--
-- Name: COLUMN image_styles.isid; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN image_styles.isid IS 'The primary identifier for an image style.';


--
-- Name: COLUMN image_styles.name; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN image_styles.name IS 'The style machine name.';


--
-- Name: COLUMN image_styles.label; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN image_styles.label IS 'The style administrative name.';


--
-- Name: image_styles_isid_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE image_styles_isid_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


--
-- Name: image_styles_isid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: -
--

ALTER SEQUENCE image_styles_isid_seq OWNED BY image_styles.isid;


--
-- Name: masquerade; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE masquerade (
    sid character varying(64) DEFAULT ''::character varying NOT NULL,
    uid_from integer DEFAULT 0 NOT NULL,
    uid_as integer DEFAULT 0 NOT NULL
);


--
-- Name: TABLE masquerade; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE masquerade IS 'Each masquerading user has their session recorded into the masquerade table. Each record represents a masquerading user.';


--
-- Name: COLUMN masquerade.sid; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN masquerade.sid IS 'The current session for this masquerading user corresponding to their sessions.sid.';


--
-- Name: COLUMN masquerade.uid_from; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN masquerade.uid_from IS 'The users.uid corresponding to a session.';


--
-- Name: COLUMN masquerade.uid_as; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN masquerade.uid_as IS 'The users.uid this session is masquerading as.';


--
-- Name: masquerade_users; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE masquerade_users (
    uid_from integer DEFAULT 0 NOT NULL,
    uid_to integer DEFAULT 0 NOT NULL
);


--
-- Name: TABLE masquerade_users; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE masquerade_users IS 'Per-user permission table granting permissions to switch as a specific user.';


--
-- Name: COLUMN masquerade_users.uid_from; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN masquerade_users.uid_from IS 'The users.uid that can masquerade as masquerade_users.uid_to.';


--
-- Name: COLUMN masquerade_users.uid_to; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN masquerade_users.uid_to IS 'The users.uid that masquerade_users.uid_from can masquerade as.';


--
-- Name: mcl_data_valid; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE mcl_data_valid (
    data_valid_id integer NOT NULL,
    data_valid_type_id integer NOT NULL,
    name character varying(255) NOT NULL,
    cvterm_id integer
);


--
-- Name: mcl_data_valid_data_valid_id_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE mcl_data_valid_data_valid_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


--
-- Name: mcl_data_valid_data_valid_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: -
--

ALTER SEQUENCE mcl_data_valid_data_valid_id_seq OWNED BY mcl_data_valid.data_valid_id;


--
-- Name: mcl_data_valid_type; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE mcl_data_valid_type (
    data_valid_type_id integer NOT NULL,
    type character varying(255) NOT NULL,
    cv_id integer
);


--
-- Name: mcl_data_valid_type_data_valid_type_id_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE mcl_data_valid_type_data_valid_type_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


--
-- Name: mcl_data_valid_type_data_valid_type_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: -
--

ALTER SEQUENCE mcl_data_valid_type_data_valid_type_id_seq OWNED BY mcl_data_valid_type.data_valid_type_id;


--
-- Name: mcl_file; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE mcl_file (
    file_id integer NOT NULL,
    type character varying(255) NOT NULL,
    filename character varying(255) NOT NULL,
    filepath character varying(512) NOT NULL,
    filesize integer DEFAULT 0 NOT NULL,
    uri character varying(255) NOT NULL,
    user_id integer NOT NULL,
    job_id integer,
    submit_date timestamp without time zone NOT NULL,
    prop text
);


--
-- Name: mcl_file_file_id_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE mcl_file_file_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


--
-- Name: mcl_file_file_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: -
--

ALTER SEQUENCE mcl_file_file_id_seq OWNED BY mcl_file.file_id;


--
-- Name: mcl_job; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE mcl_job (
    job_id integer NOT NULL,
    name character varying(255) NOT NULL,
    type character varying(255) NOT NULL,
    class_name character varying(255),
    status integer,
    param text,
    prop text,
    user_id integer NOT NULL,
    submit_date timestamp without time zone NOT NULL,
    complete_date timestamp without time zone
);


--
-- Name: mcl_job_job_id_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE mcl_job_job_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


--
-- Name: mcl_job_job_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: -
--

ALTER SEQUENCE mcl_job_job_id_seq OWNED BY mcl_job.job_id;


--
-- Name: mcl_template; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE mcl_template (
    template_id integer NOT NULL,
    template character varying(255) NOT NULL,
    template_type_id integer NOT NULL,
    public integer DEFAULT 1
);


--
-- Name: mcl_template_template_id_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE mcl_template_template_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


--
-- Name: mcl_template_template_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: -
--

ALTER SEQUENCE mcl_template_template_id_seq OWNED BY mcl_template.template_id;


--
-- Name: mcl_template_type; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE mcl_template_type (
    template_type_id integer NOT NULL,
    type character varying(255) NOT NULL,
    rank integer NOT NULL
);


--
-- Name: mcl_template_type_template_type_id_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE mcl_template_type_template_type_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


--
-- Name: mcl_template_type_template_type_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: -
--

ALTER SEQUENCE mcl_template_type_template_type_id_seq OWNED BY mcl_template_type.template_type_id;


--
-- Name: mcl_user; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE mcl_user (
    user_id integer NOT NULL,
    name character varying(255) NOT NULL,
    uid integer NOT NULL,
    mail character varying(255) NOT NULL,
    prop text
);


--
-- Name: mcl_var; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE mcl_var (
    var_id integer NOT NULL,
    name character varying(255) NOT NULL,
    type character varying(255) NOT NULL,
    value character varying(1024),
    description text
);


--
-- Name: mcl_var_var_id_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE mcl_var_var_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


--
-- Name: mcl_var_var_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: -
--

ALTER SEQUENCE mcl_var_var_id_seq OWNED BY mcl_var.var_id;


--
-- Name: mdlu_cron_job; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE mdlu_cron_job (
    cron_job_id integer NOT NULL,
    name character varying(255) NOT NULL,
    "interval" character varying(255),
    description text
);


--
-- Name: mdlu_cron_job_cron_job_id_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE mdlu_cron_job_cron_job_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


--
-- Name: mdlu_cron_job_cron_job_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: -
--

ALTER SEQUENCE mdlu_cron_job_cron_job_id_seq OWNED BY mdlu_cron_job.cron_job_id;


--
-- Name: mdlu_database; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE mdlu_database (
    database_id integer NOT NULL,
    database_type_id integer NOT NULL,
    name character varying(255) NOT NULL,
    url character varying(255) NOT NULL,
    description text,
    last_update timestamp without time zone,
    filesize integer,
    filepath character varying(500),
    cmd text
);


--
-- Name: mdlu_database_database_id_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE mdlu_database_database_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


--
-- Name: mdlu_database_database_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: -
--

ALTER SEQUENCE mdlu_database_database_id_seq OWNED BY mdlu_database.database_id;


--
-- Name: mdlu_database_type; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE mdlu_database_type (
    database_type_id integer NOT NULL,
    type character varying(255) NOT NULL,
    label character varying(255) NOT NULL,
    description text
);


--
-- Name: mdlu_database_type_database_type_id_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE mdlu_database_type_database_type_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


--
-- Name: mdlu_database_type_database_type_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: -
--

ALTER SEQUENCE mdlu_database_type_database_type_id_seq OWNED BY mdlu_database_type.database_type_id;


--
-- Name: mdlu_log; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE mdlu_log (
    log_id integer NOT NULL,
    user_id integer NOT NULL,
    update_date timestamp without time zone
);


--
-- Name: mdlu_log_log_id_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE mdlu_log_log_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


--
-- Name: mdlu_log_log_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: -
--

ALTER SEQUENCE mdlu_log_log_id_seq OWNED BY mdlu_log.log_id;


--
-- Name: menu_custom; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE menu_custom (
    menu_name character varying(32) DEFAULT ''::character varying NOT NULL,
    title character varying(255) DEFAULT ''::character varying NOT NULL,
    description text
);


--
-- Name: TABLE menu_custom; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE menu_custom IS 'Holds definitions for top-level custom menus (for example, Main menu).';


--
-- Name: COLUMN menu_custom.menu_name; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN menu_custom.menu_name IS 'Primary Key: Unique key for menu. This is used as a block delta so length is 32.';


--
-- Name: COLUMN menu_custom.title; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN menu_custom.title IS 'Menu title; displayed at top of block.';


--
-- Name: COLUMN menu_custom.description; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN menu_custom.description IS 'Menu description.';


--
-- Name: menu_links; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE menu_links (
    menu_name character varying(32) DEFAULT ''::character varying NOT NULL,
    mlid integer NOT NULL,
    plid bigint DEFAULT 0 NOT NULL,
    link_path character varying(255) DEFAULT ''::character varying NOT NULL,
    router_path character varying(255) DEFAULT ''::character varying NOT NULL,
    link_title character varying(255) DEFAULT ''::character varying NOT NULL,
    options bytea,
    module character varying(255) DEFAULT 'system'::character varying NOT NULL,
    hidden smallint DEFAULT 0 NOT NULL,
    external smallint DEFAULT 0 NOT NULL,
    has_children smallint DEFAULT 0 NOT NULL,
    expanded smallint DEFAULT 0 NOT NULL,
    weight integer DEFAULT 0 NOT NULL,
    depth smallint DEFAULT 0 NOT NULL,
    customized smallint DEFAULT 0 NOT NULL,
    p1 bigint DEFAULT 0 NOT NULL,
    p2 bigint DEFAULT 0 NOT NULL,
    p3 bigint DEFAULT 0 NOT NULL,
    p4 bigint DEFAULT 0 NOT NULL,
    p5 bigint DEFAULT 0 NOT NULL,
    p6 bigint DEFAULT 0 NOT NULL,
    p7 bigint DEFAULT 0 NOT NULL,
    p8 bigint DEFAULT 0 NOT NULL,
    p9 bigint DEFAULT 0 NOT NULL,
    updated smallint DEFAULT 0 NOT NULL,
    CONSTRAINT menu_links_mlid_check CHECK ((mlid >= 0)),
    CONSTRAINT menu_links_p1_check CHECK ((p1 >= 0)),
    CONSTRAINT menu_links_p2_check CHECK ((p2 >= 0)),
    CONSTRAINT menu_links_p3_check CHECK ((p3 >= 0)),
    CONSTRAINT menu_links_p4_check CHECK ((p4 >= 0)),
    CONSTRAINT menu_links_p5_check CHECK ((p5 >= 0)),
    CONSTRAINT menu_links_p6_check CHECK ((p6 >= 0)),
    CONSTRAINT menu_links_p7_check CHECK ((p7 >= 0)),
    CONSTRAINT menu_links_p8_check CHECK ((p8 >= 0)),
    CONSTRAINT menu_links_p9_check CHECK ((p9 >= 0)),
    CONSTRAINT menu_links_plid_check CHECK ((plid >= 0))
);


--
-- Name: TABLE menu_links; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE menu_links IS 'Contains the individual links within a menu.';


--
-- Name: COLUMN menu_links.menu_name; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN menu_links.menu_name IS 'The menu name. All links with the same menu name (such as ''navigation'') are part of the same menu.';


--
-- Name: COLUMN menu_links.mlid; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN menu_links.mlid IS 'The menu link ID (mlid) is the integer primary key.';


--
-- Name: COLUMN menu_links.plid; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN menu_links.plid IS 'The parent link ID (plid) is the mlid of the link above in the hierarchy, or zero if the link is at the top level in its menu.';


--
-- Name: COLUMN menu_links.link_path; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN menu_links.link_path IS 'The Drupal path or external path this link points to.';


--
-- Name: COLUMN menu_links.router_path; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN menu_links.router_path IS 'For links corresponding to a Drupal path (external = 0), this connects the link to a menu_router.path for joins.';


--
-- Name: COLUMN menu_links.link_title; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN menu_links.link_title IS 'The text displayed for the link, which may be modified by a title callback stored in menu_router.';


--
-- Name: COLUMN menu_links.options; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN menu_links.options IS 'A serialized array of options to be passed to the url() or l() function, such as a query string or HTML attributes.';


--
-- Name: COLUMN menu_links.module; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN menu_links.module IS 'The name of the module that generated this link.';


--
-- Name: COLUMN menu_links.hidden; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN menu_links.hidden IS 'A flag for whether the link should be rendered in menus. (1 = a disabled menu item that may be shown on admin screens, -1 = a menu callback, 0 = a normal, visible link)';


--
-- Name: COLUMN menu_links.external; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN menu_links.external IS 'A flag to indicate if the link points to a full URL starting with a protocol,::text like http:// (1 = external, 0 = internal).';


--
-- Name: COLUMN menu_links.has_children; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN menu_links.has_children IS 'Flag indicating whether any links have this link as a parent (1 = children exist, 0 = no children).';


--
-- Name: COLUMN menu_links.expanded; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN menu_links.expanded IS 'Flag for whether this link should be rendered as expanded in menus - expanded links always have their child links displayed, instead of only when the link is in the active trail (1 = expanded, 0 = not expanded)';


--
-- Name: COLUMN menu_links.weight; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN menu_links.weight IS 'Link weight among links in the same menu at the same depth.';


--
-- Name: COLUMN menu_links.depth; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN menu_links.depth IS 'The depth relative to the top level. A link with plid == 0 will have depth == 1.';


--
-- Name: COLUMN menu_links.customized; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN menu_links.customized IS 'A flag to indicate that the user has manually created or edited the link (1 = customized, 0 = not customized).';


--
-- Name: COLUMN menu_links.p1; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN menu_links.p1 IS 'The first mlid in the materialized path. If N = depth, then pN must equal the mlid. If depth > 1 then p(N-1) must equal the plid. All pX where X > depth must equal zero. The columns p1 .. p9 are also called the parents.';


--
-- Name: COLUMN menu_links.p2; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN menu_links.p2 IS 'The second mlid in the materialized path. See p1.';


--
-- Name: COLUMN menu_links.p3; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN menu_links.p3 IS 'The third mlid in the materialized path. See p1.';


--
-- Name: COLUMN menu_links.p4; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN menu_links.p4 IS 'The fourth mlid in the materialized path. See p1.';


--
-- Name: COLUMN menu_links.p5; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN menu_links.p5 IS 'The fifth mlid in the materialized path. See p1.';


--
-- Name: COLUMN menu_links.p6; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN menu_links.p6 IS 'The sixth mlid in the materialized path. See p1.';


--
-- Name: COLUMN menu_links.p7; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN menu_links.p7 IS 'The seventh mlid in the materialized path. See p1.';


--
-- Name: COLUMN menu_links.p8; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN menu_links.p8 IS 'The eighth mlid in the materialized path. See p1.';


--
-- Name: COLUMN menu_links.p9; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN menu_links.p9 IS 'The ninth mlid in the materialized path. See p1.';


--
-- Name: COLUMN menu_links.updated; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN menu_links.updated IS 'Flag that indicates that this link was generated during the update from Drupal 5.';


--
-- Name: menu_links_mlid_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE menu_links_mlid_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


--
-- Name: menu_links_mlid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: -
--

ALTER SEQUENCE menu_links_mlid_seq OWNED BY menu_links.mlid;


--
-- Name: menu_router; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE menu_router (
    path character varying(255) DEFAULT ''::character varying NOT NULL,
    load_functions bytea NOT NULL,
    to_arg_functions bytea NOT NULL,
    access_callback character varying(255) DEFAULT ''::character varying NOT NULL,
    access_arguments bytea,
    page_callback character varying(255) DEFAULT ''::character varying NOT NULL,
    page_arguments bytea,
    delivery_callback character varying(255) DEFAULT ''::character varying NOT NULL,
    fit integer DEFAULT 0 NOT NULL,
    number_parts smallint DEFAULT 0 NOT NULL,
    context integer DEFAULT 0 NOT NULL,
    tab_parent character varying(255) DEFAULT ''::character varying NOT NULL,
    tab_root character varying(255) DEFAULT ''::character varying NOT NULL,
    title character varying(255) DEFAULT ''::character varying NOT NULL,
    title_callback character varying(255) DEFAULT ''::character varying NOT NULL,
    title_arguments character varying(255) DEFAULT ''::character varying NOT NULL,
    theme_callback character varying(255) DEFAULT ''::character varying NOT NULL,
    theme_arguments character varying(255) DEFAULT ''::character varying NOT NULL,
    type integer DEFAULT 0 NOT NULL,
    description text NOT NULL,
    "position" character varying(255) DEFAULT ''::character varying NOT NULL,
    weight integer DEFAULT 0 NOT NULL,
    include_file text
);


--
-- Name: TABLE menu_router; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE menu_router IS 'Maps paths to various callbacks (access, page and title)';


--
-- Name: COLUMN menu_router.path; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN menu_router.path IS 'Primary Key: the Drupal path this entry describes';


--
-- Name: COLUMN menu_router.load_functions; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN menu_router.load_functions IS 'A serialized array of function names (like node_load) to be called to load an object corresponding to a part of the current path.';


--
-- Name: COLUMN menu_router.to_arg_functions; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN menu_router.to_arg_functions IS 'A serialized array of function names (like user_uid_optional_to_arg) to be called to replace a part of the router path with another string.';


--
-- Name: COLUMN menu_router.access_callback; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN menu_router.access_callback IS 'The callback which determines the access to this router path. Defaults to user_access.';


--
-- Name: COLUMN menu_router.access_arguments; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN menu_router.access_arguments IS 'A serialized array of arguments for the access callback.';


--
-- Name: COLUMN menu_router.page_callback; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN menu_router.page_callback IS 'The name of the function that renders the page.';


--
-- Name: COLUMN menu_router.page_arguments; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN menu_router.page_arguments IS 'A serialized array of arguments for the page callback.';


--
-- Name: COLUMN menu_router.delivery_callback; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN menu_router.delivery_callback IS 'The name of the function that sends the result of the page_callback function to the browser.';


--
-- Name: COLUMN menu_router.fit; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN menu_router.fit IS 'A numeric representation of how specific the path is.';


--
-- Name: COLUMN menu_router.number_parts; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN menu_router.number_parts IS 'Number of parts in this router path.';


--
-- Name: COLUMN menu_router.context; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN menu_router.context IS 'Only for local tasks (tabs) - the context of a local task to control its placement.';


--
-- Name: COLUMN menu_router.tab_parent; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN menu_router.tab_parent IS 'Only for local tasks (tabs) - the router path of the parent page (which may also be a local task).';


--
-- Name: COLUMN menu_router.tab_root; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN menu_router.tab_root IS 'Router path of the closest non-tab parent page. For pages that are not local tasks, this will be the same as the path.';


--
-- Name: COLUMN menu_router.title; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN menu_router.title IS 'The title for the current page, or the title for the tab if this is a local task.';


--
-- Name: COLUMN menu_router.title_callback; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN menu_router.title_callback IS 'A function which will alter the title. Defaults to t()';


--
-- Name: COLUMN menu_router.title_arguments; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN menu_router.title_arguments IS 'A serialized array of arguments for the title callback. If empty, the title will be used as the sole argument for the title callback.';


--
-- Name: COLUMN menu_router.theme_callback; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN menu_router.theme_callback IS 'A function which returns the name of the theme that will be used to render this page. If left empty, the default theme will be used.';


--
-- Name: COLUMN menu_router.theme_arguments; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN menu_router.theme_arguments IS 'A serialized array of arguments for the theme callback.';


--
-- Name: COLUMN menu_router.type; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN menu_router.type IS 'Numeric representation of the type of the menu item,::text like MENU_LOCAL_TASK.';


--
-- Name: COLUMN menu_router.description; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN menu_router.description IS 'A description of this item.';


--
-- Name: COLUMN menu_router."position"; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN menu_router."position" IS 'The position of the block (left or right) on the system administration page for this item.';


--
-- Name: COLUMN menu_router.weight; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN menu_router.weight IS 'Weight of the element. Lighter weights are higher up, heavier weights go down.';


--
-- Name: COLUMN menu_router.include_file; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN menu_router.include_file IS 'The file to include for this element, usually the page callback function lives in this file.';


--
-- Name: node; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE node (
    nid integer NOT NULL,
    vid bigint,
    type character varying(32) DEFAULT ''::character varying NOT NULL,
    language character varying(12) DEFAULT ''::character varying NOT NULL,
    title character varying(255) DEFAULT ''::character varying NOT NULL,
    uid integer DEFAULT 0 NOT NULL,
    status integer DEFAULT 1 NOT NULL,
    created integer DEFAULT 0 NOT NULL,
    changed integer DEFAULT 0 NOT NULL,
    comment integer DEFAULT 0 NOT NULL,
    promote integer DEFAULT 0 NOT NULL,
    sticky integer DEFAULT 0 NOT NULL,
    tnid bigint DEFAULT 0 NOT NULL,
    translate integer DEFAULT 0 NOT NULL,
    CONSTRAINT node_nid_check CHECK ((nid >= 0)),
    CONSTRAINT node_tnid_check CHECK ((tnid >= 0)),
    CONSTRAINT node_vid_check CHECK ((vid >= 0))
);


--
-- Name: TABLE node; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE node IS 'The base table for nodes.';


--
-- Name: COLUMN node.nid; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN node.nid IS 'The primary identifier for a node.';


--
-- Name: COLUMN node.vid; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN node.vid IS 'The current node_revision.vid version identifier.';


--
-- Name: COLUMN node.type; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN node.type IS 'The node_type.type of this node.';


--
-- Name: COLUMN node.language; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN node.language IS 'The languages.language of this node.';


--
-- Name: COLUMN node.title; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN node.title IS 'The title of this node, always treated as non-markup plain text.';


--
-- Name: COLUMN node.uid; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN node.uid IS 'The users.uid that owns this node; initially, this is the user that created it.';


--
-- Name: COLUMN node.status; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN node.status IS 'Boolean indicating whether the node is published (visible to non-administrators).';


--
-- Name: COLUMN node.created; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN node.created IS 'The Unix timestamp when the node was created.';


--
-- Name: COLUMN node.changed; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN node.changed IS 'The Unix timestamp when the node was most recently saved.';


--
-- Name: COLUMN node.comment; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN node.comment IS 'Whether comments are allowed on this node: 0 = no, 1 = closed (read only), 2 = open (read/write).';


--
-- Name: COLUMN node.promote; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN node.promote IS 'Boolean indicating whether the node should be displayed on the front page.';


--
-- Name: COLUMN node.sticky; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN node.sticky IS 'Boolean indicating whether the node should be displayed at the top of lists in which it appears.';


--
-- Name: COLUMN node.tnid; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN node.tnid IS 'The translation set id for this node, which equals the node id of the source post in each set.';


--
-- Name: COLUMN node.translate; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN node.translate IS 'A boolean indicating whether this translation page needs to be updated.';


--
-- Name: node_access; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE node_access (
    nid bigint DEFAULT 0 NOT NULL,
    gid bigint DEFAULT 0 NOT NULL,
    realm character varying(255) DEFAULT ''::character varying NOT NULL,
    grant_view integer DEFAULT 0 NOT NULL,
    grant_update integer DEFAULT 0 NOT NULL,
    grant_delete integer DEFAULT 0 NOT NULL,
    CONSTRAINT node_access_gid_check CHECK ((gid >= 0)),
    CONSTRAINT node_access_grant_delete_check CHECK ((grant_delete >= 0)),
    CONSTRAINT node_access_grant_update_check CHECK ((grant_update >= 0)),
    CONSTRAINT node_access_grant_view_check CHECK ((grant_view >= 0)),
    CONSTRAINT node_access_nid_check CHECK ((nid >= 0))
);


--
-- Name: TABLE node_access; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE node_access IS 'Identifies which realm/grant pairs a user must possess in order to view, update, or delete specific nodes.';


--
-- Name: COLUMN node_access.nid; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN node_access.nid IS 'The node.nid this record affects.';


--
-- Name: COLUMN node_access.gid; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN node_access.gid IS 'The grant ID a user must possess in the specified realm to gain this row''s privileges on the node.';


--
-- Name: COLUMN node_access.realm; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN node_access.realm IS 'The realm in which the user must possess the grant ID. Each node access node can define one or more realms.';


--
-- Name: COLUMN node_access.grant_view; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN node_access.grant_view IS 'Boolean indicating whether a user with the realm/grant pair can view this node.';


--
-- Name: COLUMN node_access.grant_update; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN node_access.grant_update IS 'Boolean indicating whether a user with the realm/grant pair can edit this node.';


--
-- Name: COLUMN node_access.grant_delete; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN node_access.grant_delete IS 'Boolean indicating whether a user with the realm/grant pair can delete this node.';


--
-- Name: node_comment_statistics; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE node_comment_statistics (
    nid bigint DEFAULT 0 NOT NULL,
    cid integer DEFAULT 0 NOT NULL,
    last_comment_timestamp integer DEFAULT 0 NOT NULL,
    last_comment_name character varying(60),
    last_comment_uid integer DEFAULT 0 NOT NULL,
    comment_count bigint DEFAULT 0 NOT NULL,
    CONSTRAINT node_comment_statistics_comment_count_check CHECK ((comment_count >= 0)),
    CONSTRAINT node_comment_statistics_nid_check CHECK ((nid >= 0))
);


--
-- Name: TABLE node_comment_statistics; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE node_comment_statistics IS 'Maintains statistics of node and comments posts to show "new" and "updated" flags.';


--
-- Name: COLUMN node_comment_statistics.nid; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN node_comment_statistics.nid IS 'The node.nid for which the statistics are compiled.';


--
-- Name: COLUMN node_comment_statistics.cid; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN node_comment_statistics.cid IS 'The comment.cid of the last comment.';


--
-- Name: COLUMN node_comment_statistics.last_comment_timestamp; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN node_comment_statistics.last_comment_timestamp IS 'The Unix timestamp of the last comment that was posted within this node, from comment.changed.';


--
-- Name: COLUMN node_comment_statistics.last_comment_name; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN node_comment_statistics.last_comment_name IS 'The name of the latest author to post a comment on this node, from comment.name.';


--
-- Name: COLUMN node_comment_statistics.last_comment_uid; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN node_comment_statistics.last_comment_uid IS 'The user ID of the latest author to post a comment on this node, from comment.uid.';


--
-- Name: COLUMN node_comment_statistics.comment_count; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN node_comment_statistics.comment_count IS 'The total number of comments on this node.';


--
-- Name: node_nid_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE node_nid_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


--
-- Name: node_nid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: -
--

ALTER SEQUENCE node_nid_seq OWNED BY node.nid;


--
-- Name: node_revision; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE node_revision (
    nid bigint DEFAULT 0 NOT NULL,
    vid integer NOT NULL,
    uid integer DEFAULT 0 NOT NULL,
    title character varying(255) DEFAULT ''::character varying NOT NULL,
    log text NOT NULL,
    "timestamp" integer DEFAULT 0 NOT NULL,
    status integer DEFAULT 1 NOT NULL,
    comment integer DEFAULT 0 NOT NULL,
    promote integer DEFAULT 0 NOT NULL,
    sticky integer DEFAULT 0 NOT NULL,
    CONSTRAINT node_revision_nid_check CHECK ((nid >= 0)),
    CONSTRAINT node_revision_vid_check CHECK ((vid >= 0))
);


--
-- Name: TABLE node_revision; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE node_revision IS 'Stores information about each saved version of a node.';


--
-- Name: COLUMN node_revision.nid; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN node_revision.nid IS 'The node this version belongs to.';


--
-- Name: COLUMN node_revision.vid; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN node_revision.vid IS 'The primary identifier for this version.';


--
-- Name: COLUMN node_revision.uid; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN node_revision.uid IS 'The users.uid that created this version.';


--
-- Name: COLUMN node_revision.title; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN node_revision.title IS 'The title of this version.';


--
-- Name: COLUMN node_revision.log; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN node_revision.log IS 'The log entry explaining the changes in this version.';


--
-- Name: COLUMN node_revision."timestamp"; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN node_revision."timestamp" IS 'A Unix timestamp indicating when this version was created.';


--
-- Name: COLUMN node_revision.status; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN node_revision.status IS 'Boolean indicating whether the node (at the time of this revision) is published (visible to non-administrators).';


--
-- Name: COLUMN node_revision.comment; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN node_revision.comment IS 'Whether comments are allowed on this node (at the time of this revision): 0 = no, 1 = closed (read only), 2 = open (read/write).';


--
-- Name: COLUMN node_revision.promote; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN node_revision.promote IS 'Boolean indicating whether the node (at the time of this revision) should be displayed on the front page.';


--
-- Name: COLUMN node_revision.sticky; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN node_revision.sticky IS 'Boolean indicating whether the node (at the time of this revision) should be displayed at the top of lists in which it appears.';


--
-- Name: node_revision_vid_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE node_revision_vid_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


--
-- Name: node_revision_vid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: -
--

ALTER SEQUENCE node_revision_vid_seq OWNED BY node_revision.vid;


--
-- Name: node_type; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE node_type (
    type character varying(32) NOT NULL,
    name character varying(255) DEFAULT ''::character varying NOT NULL,
    base character varying(255) NOT NULL,
    module character varying(255) NOT NULL,
    description text NOT NULL,
    help text NOT NULL,
    has_title integer NOT NULL,
    title_label character varying(255) DEFAULT ''::character varying NOT NULL,
    custom smallint DEFAULT 0 NOT NULL,
    modified smallint DEFAULT 0 NOT NULL,
    locked smallint DEFAULT 0 NOT NULL,
    disabled smallint DEFAULT 0 NOT NULL,
    orig_type character varying(255) DEFAULT ''::character varying NOT NULL,
    CONSTRAINT node_type_has_title_check CHECK ((has_title >= 0))
);


--
-- Name: TABLE node_type; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE node_type IS 'Stores information about all defined node types.';


--
-- Name: COLUMN node_type.type; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN node_type.type IS 'The machine-readable name of this type.';


--
-- Name: COLUMN node_type.name; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN node_type.name IS 'The human-readable name of this type.';


--
-- Name: COLUMN node_type.base; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN node_type.base IS 'The base string used to construct callbacks corresponding to this node type.';


--
-- Name: COLUMN node_type.module; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN node_type.module IS 'The module defining this node type.';


--
-- Name: COLUMN node_type.description; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN node_type.description IS 'A brief description of this type.';


--
-- Name: COLUMN node_type.help; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN node_type.help IS 'Help information shown to the user when creating a node of this type.';


--
-- Name: COLUMN node_type.has_title; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN node_type.has_title IS 'Boolean indicating whether this type uses the node.title field.';


--
-- Name: COLUMN node_type.title_label; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN node_type.title_label IS 'The label displayed for the title field on the edit form.';


--
-- Name: COLUMN node_type.custom; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN node_type.custom IS 'A boolean indicating whether this type is defined by a module (FALSE) or by a user via Add content type (TRUE).';


--
-- Name: COLUMN node_type.modified; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN node_type.modified IS 'A boolean indicating whether this type has been modified by an administrator; currently not used in any way.';


--
-- Name: COLUMN node_type.locked; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN node_type.locked IS 'A boolean indicating whether the administrator can change the machine name of this type.';


--
-- Name: COLUMN node_type.disabled; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN node_type.disabled IS 'A boolean indicating whether the node type is disabled.';


--
-- Name: COLUMN node_type.orig_type; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN node_type.orig_type IS 'The original machine-readable name of this node type. This may be different from the current type name if the locked field is 0.';


--
-- Name: queue; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE queue (
    item_id integer NOT NULL,
    name character varying(255) DEFAULT ''::character varying NOT NULL,
    data bytea,
    expire integer DEFAULT 0 NOT NULL,
    created integer DEFAULT 0 NOT NULL,
    CONSTRAINT queue_item_id_check CHECK ((item_id >= 0))
);


--
-- Name: TABLE queue; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE queue IS 'Stores items in queues.';


--
-- Name: COLUMN queue.item_id; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN queue.item_id IS 'Primary Key: Unique item ID.';


--
-- Name: COLUMN queue.name; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN queue.name IS 'The queue name.';


--
-- Name: COLUMN queue.data; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN queue.data IS 'The arbitrary data for the item.';


--
-- Name: COLUMN queue.expire; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN queue.expire IS 'Timestamp when the claim lease expires on the item.';


--
-- Name: COLUMN queue.created; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN queue.created IS 'Timestamp when the item was created.';


--
-- Name: queue_item_id_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE queue_item_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


--
-- Name: queue_item_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: -
--

ALTER SEQUENCE queue_item_id_seq OWNED BY queue.item_id;


--
-- Name: rdf_mapping; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE rdf_mapping (
    type character varying(128) NOT NULL,
    bundle character varying(128) NOT NULL,
    mapping bytea
);


--
-- Name: TABLE rdf_mapping; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE rdf_mapping IS 'Stores custom RDF mappings for user defined content types or overriden module-defined mappings';


--
-- Name: COLUMN rdf_mapping.type; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN rdf_mapping.type IS 'The name of the entity type a mapping applies to (node, user, comment, etc.).';


--
-- Name: COLUMN rdf_mapping.bundle; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN rdf_mapping.bundle IS 'The name of the bundle a mapping applies to.';


--
-- Name: COLUMN rdf_mapping.mapping; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN rdf_mapping.mapping IS 'The serialized mapping of the bundle type and fields to RDF terms.';


--
-- Name: registry; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE registry (
    name character varying(255) DEFAULT ''::character varying NOT NULL,
    type character varying(9) DEFAULT ''::character varying NOT NULL,
    filename character varying(255) NOT NULL,
    module character varying(255) DEFAULT ''::character varying NOT NULL,
    weight integer DEFAULT 0 NOT NULL
);


--
-- Name: TABLE registry; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE registry IS 'Each record is a function, class, or interface name and the file it is in.';


--
-- Name: COLUMN registry.name; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN registry.name IS 'The name of the function, class, or interface.';


--
-- Name: COLUMN registry.type; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN registry.type IS 'Either function or class or interface.';


--
-- Name: COLUMN registry.filename; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN registry.filename IS 'Name of the file.';


--
-- Name: COLUMN registry.module; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN registry.module IS 'Name of the module the file belongs to.';


--
-- Name: COLUMN registry.weight; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN registry.weight IS 'The order in which this module''s hooks should be invoked relative to other modules. Equal-weighted modules are ordered by name.';


--
-- Name: registry_file; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE registry_file (
    filename character varying(255) NOT NULL,
    hash character varying(64) NOT NULL
);


--
-- Name: TABLE registry_file; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE registry_file IS 'Files parsed to build the registry.';


--
-- Name: COLUMN registry_file.filename; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN registry_file.filename IS 'Path to the file.';


--
-- Name: COLUMN registry_file.hash; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN registry_file.hash IS 'sha-256 hash of the file''s contents when last parsed.';


--
-- Name: role; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE role (
    rid integer NOT NULL,
    name character varying(64) DEFAULT ''::character varying NOT NULL,
    weight integer DEFAULT 0 NOT NULL,
    CONSTRAINT role_rid_check CHECK ((rid >= 0))
);


--
-- Name: TABLE role; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE role IS 'Stores user roles.';


--
-- Name: COLUMN role.rid; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN role.rid IS 'Primary Key: Unique role ID.';


--
-- Name: COLUMN role.name; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN role.name IS 'Unique role name.';


--
-- Name: COLUMN role.weight; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN role.weight IS 'The weight of this role in listings and the user interface.';


--
-- Name: role_permission; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE role_permission (
    rid bigint NOT NULL,
    permission character varying(128) DEFAULT ''::character varying NOT NULL,
    module character varying(255) DEFAULT ''::character varying NOT NULL,
    CONSTRAINT role_permission_rid_check CHECK ((rid >= 0))
);


--
-- Name: TABLE role_permission; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE role_permission IS 'Stores the permissions assigned to user roles.';


--
-- Name: COLUMN role_permission.rid; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN role_permission.rid IS 'Foreign Key: role.rid.';


--
-- Name: COLUMN role_permission.permission; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN role_permission.permission IS 'A single permission granted to the role identified by rid.';


--
-- Name: COLUMN role_permission.module; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN role_permission.module IS 'The module declaring the permission.';


--
-- Name: role_rid_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE role_rid_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


--
-- Name: role_rid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: -
--

ALTER SEQUENCE role_rid_seq OWNED BY role.rid;


--
-- Name: search_dataset; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE search_dataset (
    sid bigint DEFAULT 0 NOT NULL,
    type character varying(16) NOT NULL,
    data text NOT NULL,
    reindex bigint DEFAULT 0 NOT NULL,
    CONSTRAINT search_dataset_reindex_check CHECK ((reindex >= 0)),
    CONSTRAINT search_dataset_sid_check CHECK ((sid >= 0))
);


--
-- Name: TABLE search_dataset; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE search_dataset IS 'Stores items that will be searched.';


--
-- Name: COLUMN search_dataset.sid; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN search_dataset.sid IS 'Search item ID, e.g. node ID for nodes.';


--
-- Name: COLUMN search_dataset.type; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN search_dataset.type IS 'Type of item, e.g. node.';


--
-- Name: COLUMN search_dataset.data; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN search_dataset.data IS 'List of space-separated words from the item.';


--
-- Name: COLUMN search_dataset.reindex; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN search_dataset.reindex IS 'Set to force node reindexing.';


--
-- Name: search_index; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE search_index (
    word character varying(50) DEFAULT ''::character varying NOT NULL,
    sid bigint DEFAULT 0 NOT NULL,
    type character varying(16) NOT NULL,
    score real,
    CONSTRAINT search_index_sid_check CHECK ((sid >= 0))
);


--
-- Name: TABLE search_index; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE search_index IS 'Stores the search index, associating words, items and scores.';


--
-- Name: COLUMN search_index.word; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN search_index.word IS 'The search_total.word that is associated with the search item.';


--
-- Name: COLUMN search_index.sid; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN search_index.sid IS 'The search_dataset.sid of the searchable item to which the word belongs.';


--
-- Name: COLUMN search_index.type; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN search_index.type IS 'The search_dataset.type of the searchable item to which the word belongs.';


--
-- Name: COLUMN search_index.score; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN search_index.score IS 'The numeric score of the word, higher being more important.';


--
-- Name: search_node_links; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE search_node_links (
    sid bigint DEFAULT 0 NOT NULL,
    type character varying(16) DEFAULT ''::character varying NOT NULL,
    nid bigint DEFAULT 0 NOT NULL,
    caption text,
    CONSTRAINT search_node_links_nid_check CHECK ((nid >= 0)),
    CONSTRAINT search_node_links_sid_check CHECK ((sid >= 0))
);


--
-- Name: TABLE search_node_links; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE search_node_links IS 'Stores items (like nodes) that link to other nodes, used to improve search scores for nodes that are frequently linked to.';


--
-- Name: COLUMN search_node_links.sid; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN search_node_links.sid IS 'The search_dataset.sid of the searchable item containing the link to the node.';


--
-- Name: COLUMN search_node_links.type; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN search_node_links.type IS 'The search_dataset.type of the searchable item containing the link to the node.';


--
-- Name: COLUMN search_node_links.nid; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN search_node_links.nid IS 'The node.nid that this item links to.';


--
-- Name: COLUMN search_node_links.caption; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN search_node_links.caption IS 'The text used to link to the node.nid.';


--
-- Name: search_total; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE search_total (
    word character varying(50) DEFAULT ''::character varying NOT NULL,
    count real
);


--
-- Name: TABLE search_total; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE search_total IS 'Stores search totals for words.';


--
-- Name: COLUMN search_total.word; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN search_total.word IS 'Primary Key: Unique word in the search index.';


--
-- Name: COLUMN search_total.count; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN search_total.count IS 'The count of the word in the index using Zipf''s law to equalize the probability distribution.';


--
-- Name: semaphore; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE semaphore (
    name character varying(255) DEFAULT ''::character varying NOT NULL,
    value character varying(255) DEFAULT ''::character varying NOT NULL,
    expire double precision NOT NULL
);


--
-- Name: TABLE semaphore; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE semaphore IS 'Table for holding semaphores, locks, flags, etc. that cannot be stored as Drupal variables since they must not be cached.';


--
-- Name: COLUMN semaphore.name; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN semaphore.name IS 'Primary Key: Unique name.';


--
-- Name: COLUMN semaphore.value; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN semaphore.value IS 'A value for the semaphore.';


--
-- Name: COLUMN semaphore.expire; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN semaphore.expire IS 'A Unix timestamp with microseconds indicating when the semaphore should expire.';


--
-- Name: sequences; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE sequences (
    value integer NOT NULL,
    CONSTRAINT sequences_value_check CHECK ((value >= 0))
);


--
-- Name: TABLE sequences; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE sequences IS 'Stores IDs.';


--
-- Name: COLUMN sequences.value; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN sequences.value IS 'The value of the sequence.';


--
-- Name: sequences_value_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE sequences_value_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


--
-- Name: sequences_value_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: -
--

ALTER SEQUENCE sequences_value_seq OWNED BY sequences.value;


--
-- Name: sessions; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE sessions (
    uid bigint NOT NULL,
    sid character varying(128) NOT NULL,
    ssid character varying(128) DEFAULT ''::character varying NOT NULL,
    hostname character varying(128) DEFAULT ''::character varying NOT NULL,
    "timestamp" integer DEFAULT 0 NOT NULL,
    cache integer DEFAULT 0 NOT NULL,
    session bytea,
    CONSTRAINT sessions_uid_check CHECK ((uid >= 0))
);


--
-- Name: TABLE sessions; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE sessions IS 'Drupal''s session handlers read and write into the sessions table. Each record represents a user session, either anonymous or authenticated.';


--
-- Name: COLUMN sessions.uid; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN sessions.uid IS 'The users.uid corresponding to a session, or 0 for anonymous user.';


--
-- Name: COLUMN sessions.sid; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN sessions.sid IS 'A session ID. The value is generated by Drupal''s session handlers.';


--
-- Name: COLUMN sessions.ssid; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN sessions.ssid IS 'Secure session ID. The value is generated by Drupal''s session handlers.';


--
-- Name: COLUMN sessions.hostname; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN sessions.hostname IS 'The IP address that last used this session ID (sid).';


--
-- Name: COLUMN sessions."timestamp"; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN sessions."timestamp" IS 'The Unix timestamp when this session last requested a page. Old records are purged by PHP automatically.';


--
-- Name: COLUMN sessions.cache; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN sessions.cache IS 'The time of this user''s last post. This is used when the site has specified a minimum_cache_lifetime. See cache_get().';


--
-- Name: COLUMN sessions.session; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN sessions.session IS 'The serialized contents of $_SESSION, an array of name/value pairs that persists across page requests by this session ID. Drupal loads $_SESSION from here at the start of each request and saves it at the end.';


--
-- Name: shortcut_set; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE shortcut_set (
    set_name character varying(32) DEFAULT ''::character varying NOT NULL,
    title character varying(255) DEFAULT ''::character varying NOT NULL
);


--
-- Name: TABLE shortcut_set; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE shortcut_set IS 'Stores information about sets of shortcuts links.';


--
-- Name: COLUMN shortcut_set.set_name; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN shortcut_set.set_name IS 'Primary Key: The menu_links.menu_name under which the set''s links are stored.';


--
-- Name: COLUMN shortcut_set.title; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN shortcut_set.title IS 'The title of the set.';


--
-- Name: shortcut_set_users; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE shortcut_set_users (
    uid bigint DEFAULT 0 NOT NULL,
    set_name character varying(32) DEFAULT ''::character varying NOT NULL,
    CONSTRAINT shortcut_set_users_uid_check CHECK ((uid >= 0))
);


--
-- Name: TABLE shortcut_set_users; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE shortcut_set_users IS 'Maps users to shortcut sets.';


--
-- Name: COLUMN shortcut_set_users.uid; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN shortcut_set_users.uid IS 'The users.uid for this set.';


--
-- Name: COLUMN shortcut_set_users.set_name; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN shortcut_set_users.set_name IS 'The shortcut_set.set_name that will be displayed for this user.';


--
-- Name: system; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE system (
    filename character varying(255) DEFAULT ''::character varying NOT NULL,
    name character varying(255) DEFAULT ''::character varying NOT NULL,
    type character varying(12) DEFAULT ''::character varying NOT NULL,
    owner character varying(255) DEFAULT ''::character varying NOT NULL,
    status integer DEFAULT 0 NOT NULL,
    bootstrap integer DEFAULT 0 NOT NULL,
    schema_version smallint DEFAULT (-1) NOT NULL,
    weight integer DEFAULT 0 NOT NULL,
    info bytea
);


--
-- Name: TABLE system; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE system IS 'A list of all modules, themes, and theme engines that are or have been installed in Drupal''s file system.';


--
-- Name: COLUMN system.filename; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN system.filename IS 'The path of the primary file for this item, relative to the Drupal root; e.g. modules/node/node.module.';


--
-- Name: COLUMN system.name; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN system.name IS 'The name of the item; e.g. node.';


--
-- Name: COLUMN system.type; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN system.type IS 'The type of the item, either module, theme, or theme_engine.';


--
-- Name: COLUMN system.owner; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN system.owner IS 'A theme''s ''parent'' . Can be either a theme or an engine.';


--
-- Name: COLUMN system.status; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN system.status IS 'Boolean indicating whether or not this item is enabled.';


--
-- Name: COLUMN system.bootstrap; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN system.bootstrap IS 'Boolean indicating whether this module is loaded during Drupal''s early bootstrapping phase (e.g. even before the page cache is consulted).';


--
-- Name: COLUMN system.schema_version; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN system.schema_version IS 'The module''s database schema version number. -1 if the module is not installed (its tables do not exist); 0 or the largest N of the module''s hook_update_N() function that has either been run or existed when the module was first installed.';


--
-- Name: COLUMN system.weight; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN system.weight IS 'The order in which this module''s hooks should be invoked relative to other modules. Equal-weighted modules are ordered by name.';


--
-- Name: COLUMN system.info; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN system.info IS 'A serialized array containing information from the module''s .info file; keys can include name, description, package, version, core, dependencies, and php.';


--
-- Name: taxonomy_index; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE taxonomy_index (
    nid bigint DEFAULT 0 NOT NULL,
    tid bigint DEFAULT 0 NOT NULL,
    sticky smallint DEFAULT 0,
    created integer DEFAULT 0 NOT NULL,
    CONSTRAINT taxonomy_index_nid_check CHECK ((nid >= 0)),
    CONSTRAINT taxonomy_index_tid_check CHECK ((tid >= 0))
);


--
-- Name: TABLE taxonomy_index; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE taxonomy_index IS 'Maintains denormalized information about node/term relationships.';


--
-- Name: COLUMN taxonomy_index.nid; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN taxonomy_index.nid IS 'The node.nid this record tracks.';


--
-- Name: COLUMN taxonomy_index.tid; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN taxonomy_index.tid IS 'The term ID.';


--
-- Name: COLUMN taxonomy_index.sticky; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN taxonomy_index.sticky IS 'Boolean indicating whether the node is sticky.';


--
-- Name: COLUMN taxonomy_index.created; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN taxonomy_index.created IS 'The Unix timestamp when the node was created.';


--
-- Name: taxonomy_term_data; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE taxonomy_term_data (
    tid integer NOT NULL,
    vid bigint DEFAULT 0 NOT NULL,
    name character varying(255) DEFAULT ''::character varying NOT NULL,
    description text,
    format character varying(255),
    weight integer DEFAULT 0 NOT NULL,
    CONSTRAINT taxonomy_term_data_tid_check CHECK ((tid >= 0)),
    CONSTRAINT taxonomy_term_data_vid_check CHECK ((vid >= 0))
);


--
-- Name: TABLE taxonomy_term_data; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE taxonomy_term_data IS 'Stores term information.';


--
-- Name: COLUMN taxonomy_term_data.tid; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN taxonomy_term_data.tid IS 'Primary Key: Unique term ID.';


--
-- Name: COLUMN taxonomy_term_data.vid; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN taxonomy_term_data.vid IS 'The taxonomy_vocabulary.vid of the vocabulary to which the term is assigned.';


--
-- Name: COLUMN taxonomy_term_data.name; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN taxonomy_term_data.name IS 'The term name.';


--
-- Name: COLUMN taxonomy_term_data.description; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN taxonomy_term_data.description IS 'A description of the term.';


--
-- Name: COLUMN taxonomy_term_data.format; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN taxonomy_term_data.format IS 'The filter_format.format of the description.';


--
-- Name: COLUMN taxonomy_term_data.weight; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN taxonomy_term_data.weight IS 'The weight of this term in relation to other terms.';


--
-- Name: taxonomy_term_data_tid_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE taxonomy_term_data_tid_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


--
-- Name: taxonomy_term_data_tid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: -
--

ALTER SEQUENCE taxonomy_term_data_tid_seq OWNED BY taxonomy_term_data.tid;


--
-- Name: taxonomy_term_hierarchy; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE taxonomy_term_hierarchy (
    tid bigint DEFAULT 0 NOT NULL,
    parent bigint DEFAULT 0 NOT NULL,
    CONSTRAINT taxonomy_term_hierarchy_parent_check CHECK ((parent >= 0)),
    CONSTRAINT taxonomy_term_hierarchy_tid_check CHECK ((tid >= 0))
);


--
-- Name: TABLE taxonomy_term_hierarchy; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE taxonomy_term_hierarchy IS 'Stores the hierarchical relationship between terms.';


--
-- Name: COLUMN taxonomy_term_hierarchy.tid; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN taxonomy_term_hierarchy.tid IS 'Primary Key: The taxonomy_term_data.tid of the term.';


--
-- Name: COLUMN taxonomy_term_hierarchy.parent; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN taxonomy_term_hierarchy.parent IS 'Primary Key: The taxonomy_term_data.tid of the term''s parent. 0 indicates no parent.';


--
-- Name: taxonomy_vocabulary; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE taxonomy_vocabulary (
    vid integer NOT NULL,
    name character varying(255) DEFAULT ''::character varying NOT NULL,
    machine_name character varying(255) DEFAULT ''::character varying NOT NULL,
    description text,
    hierarchy integer DEFAULT 0 NOT NULL,
    module character varying(255) DEFAULT ''::character varying NOT NULL,
    weight integer DEFAULT 0 NOT NULL,
    CONSTRAINT taxonomy_vocabulary_hierarchy_check CHECK ((hierarchy >= 0)),
    CONSTRAINT taxonomy_vocabulary_vid_check CHECK ((vid >= 0))
);


--
-- Name: TABLE taxonomy_vocabulary; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE taxonomy_vocabulary IS 'Stores vocabulary information.';


--
-- Name: COLUMN taxonomy_vocabulary.vid; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN taxonomy_vocabulary.vid IS 'Primary Key: Unique vocabulary ID.';


--
-- Name: COLUMN taxonomy_vocabulary.name; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN taxonomy_vocabulary.name IS 'Name of the vocabulary.';


--
-- Name: COLUMN taxonomy_vocabulary.machine_name; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN taxonomy_vocabulary.machine_name IS 'The vocabulary machine name.';


--
-- Name: COLUMN taxonomy_vocabulary.description; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN taxonomy_vocabulary.description IS 'Description of the vocabulary.';


--
-- Name: COLUMN taxonomy_vocabulary.hierarchy; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN taxonomy_vocabulary.hierarchy IS 'The type of hierarchy allowed within the vocabulary. (0 = disabled, 1 = single, 2 = multiple)';


--
-- Name: COLUMN taxonomy_vocabulary.module; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN taxonomy_vocabulary.module IS 'The module which created the vocabulary.';


--
-- Name: COLUMN taxonomy_vocabulary.weight; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN taxonomy_vocabulary.weight IS 'The weight of this vocabulary in relation to other vocabularies.';


--
-- Name: taxonomy_vocabulary_vid_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE taxonomy_vocabulary_vid_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


--
-- Name: taxonomy_vocabulary_vid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: -
--

ALTER SEQUENCE taxonomy_vocabulary_vid_seq OWNED BY taxonomy_vocabulary.vid;


--
-- Name: tmp; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE tmp (
    id integer NOT NULL,
    col1 character varying,
    col2 character varying,
    col3 character varying,
    col4 character varying
);


--
-- Name: tmp_id_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE tmp_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


--
-- Name: tmp_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: -
--

ALTER SEQUENCE tmp_id_seq OWNED BY tmp.id;


--
-- Name: tripal_custom_tables; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE tripal_custom_tables (
    table_id integer NOT NULL,
    table_name character varying(255),
    schema text,
    mview_id integer,
    CONSTRAINT tripal_custom_tables_table_id_check CHECK ((table_id >= 0))
);


--
-- Name: tripal_custom_tables_table_id_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE tripal_custom_tables_table_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


--
-- Name: tripal_custom_tables_table_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: -
--

ALTER SEQUENCE tripal_custom_tables_table_id_seq OWNED BY tripal_custom_tables.table_id;


--
-- Name: tripal_cv_defaults; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE tripal_cv_defaults (
    cv_default_id integer NOT NULL,
    table_name character varying(128) NOT NULL,
    field_name character varying(128) NOT NULL,
    cv_id integer NOT NULL,
    CONSTRAINT tripal_cv_defaults_cv_default_id_check CHECK ((cv_default_id >= 0))
);


--
-- Name: tripal_cv_defaults_cv_default_id_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE tripal_cv_defaults_cv_default_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


--
-- Name: tripal_cv_defaults_cv_default_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: -
--

ALTER SEQUENCE tripal_cv_defaults_cv_default_id_seq OWNED BY tripal_cv_defaults.cv_default_id;


--
-- Name: tripal_cv_obo; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE tripal_cv_obo (
    obo_id integer NOT NULL,
    name character varying(255),
    path character varying(1024),
    CONSTRAINT tripal_cv_obo_obo_id_check CHECK ((obo_id >= 0))
);


--
-- Name: tripal_cv_obo_obo_id_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE tripal_cv_obo_obo_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


--
-- Name: tripal_cv_obo_obo_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: -
--

ALTER SEQUENCE tripal_cv_obo_obo_id_seq OWNED BY tripal_cv_obo.obo_id;


--
-- Name: tripal_jobs; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE tripal_jobs (
    job_id integer NOT NULL,
    uid bigint,
    job_name character varying(255),
    modulename character varying(50),
    callback character varying(255),
    arguments text,
    progress bigint DEFAULT 0,
    status character varying(50),
    submit_date integer,
    start_time integer,
    end_time integer,
    error_msg text,
    pid bigint,
    priority bigint DEFAULT 0::bigint,
    mlock bigint,
    lock bigint,
    CONSTRAINT tripal_jobs_job_id_check CHECK ((job_id >= 0)),
    CONSTRAINT tripal_jobs_lock_check CHECK ((lock >= 0)),
    CONSTRAINT tripal_jobs_mlock_check CHECK ((mlock >= 0)),
    CONSTRAINT tripal_jobs_pid_check CHECK ((pid >= 0)),
    CONSTRAINT tripal_jobs_priority_check CHECK ((priority >= 0)),
    CONSTRAINT tripal_jobs_progress_check CHECK ((progress >= 0)),
    CONSTRAINT tripal_jobs_uid_check CHECK ((uid >= 0))
);


--
-- Name: COLUMN tripal_jobs.uid; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN tripal_jobs.uid IS 'The Drupal userid of the submitee';


--
-- Name: COLUMN tripal_jobs.modulename; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN tripal_jobs.modulename IS 'The module name that provides the callback for this job';


--
-- Name: COLUMN tripal_jobs.progress; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN tripal_jobs.progress IS 'a value from 0 to 100 indicating percent complete';


--
-- Name: COLUMN tripal_jobs.submit_date; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN tripal_jobs.submit_date IS 'UNIX integer submit time';


--
-- Name: COLUMN tripal_jobs.start_time; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN tripal_jobs.start_time IS 'UNIX integer start time';


--
-- Name: COLUMN tripal_jobs.end_time; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN tripal_jobs.end_time IS 'UNIX integer end time';


--
-- Name: COLUMN tripal_jobs.pid; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN tripal_jobs.pid IS 'The process id for the job';


--
-- Name: COLUMN tripal_jobs.priority; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN tripal_jobs.priority IS 'The job priority';


--
-- Name: COLUMN tripal_jobs.mlock; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN tripal_jobs.mlock IS 'If set to 1 then all jobs for the module are held until this one finishes';


--
-- Name: COLUMN tripal_jobs.lock; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN tripal_jobs.lock IS 'If set to 1 then all jobs are held until this one finishes';


--
-- Name: tripal_jobs_job_id_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE tripal_jobs_job_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


--
-- Name: tripal_jobs_job_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: -
--

ALTER SEQUENCE tripal_jobs_job_id_seq OWNED BY tripal_jobs.job_id;


--
-- Name: tripal_mviews; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE tripal_mviews (
    mview_id integer NOT NULL,
    name character varying(255),
    modulename character varying(50),
    mv_table character varying(128),
    mv_specs text,
    mv_schema text,
    indexed text,
    query text,
    special_index text,
    last_update integer,
    status text,
    comment text,
    CONSTRAINT tripal_mviews_mview_id_check CHECK ((mview_id >= 0))
);


--
-- Name: COLUMN tripal_mviews.modulename; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN tripal_mviews.modulename IS 'The module name that provides the callback for this job';


--
-- Name: COLUMN tripal_mviews.last_update; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN tripal_mviews.last_update IS 'UNIX integer time';


--
-- Name: tripal_mviews_mview_id_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE tripal_mviews_mview_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


--
-- Name: tripal_mviews_mview_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: -
--

ALTER SEQUENCE tripal_mviews_mview_id_seq OWNED BY tripal_mviews.mview_id;


--
-- Name: tripal_node_variables; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE tripal_node_variables (
    node_variable_id integer NOT NULL,
    nid integer NOT NULL,
    variable_id integer NOT NULL,
    value text,
    rank integer DEFAULT 0 NOT NULL
);


--
-- Name: TABLE tripal_node_variables; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE tripal_node_variables IS 'This table is used for storing any type of variable such as a property or setting that should be associated with a Tripal managed Drupal node.  This table is meant to store non-biological information only. All biological data should be housed in the Chado tables. Be aware that any data stored here will not be made visible through services such as Tripal Web Services and therefore can be a good place to hide application specific settings.';


--
-- Name: tripal_node_variables_node_variable_id_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE tripal_node_variables_node_variable_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


--
-- Name: tripal_node_variables_node_variable_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: -
--

ALTER SEQUENCE tripal_node_variables_node_variable_id_seq OWNED BY tripal_node_variables.node_variable_id;


--
-- Name: tripal_toc; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE tripal_toc (
    toc_item_id integer NOT NULL,
    node_type character varying(32) NOT NULL,
    key character varying(255) NOT NULL,
    title character varying(255),
    weight integer,
    hide smallint DEFAULT 0,
    nid integer,
    CONSTRAINT tripal_toc_toc_item_id_check CHECK ((toc_item_id >= 0))
);


--
-- Name: tripal_toc_toc_item_id_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE tripal_toc_toc_item_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


--
-- Name: tripal_toc_toc_item_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: -
--

ALTER SEQUENCE tripal_toc_toc_item_id_seq OWNED BY tripal_toc.toc_item_id;


--
-- Name: tripal_token_formats; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE tripal_token_formats (
    tripal_format_id integer NOT NULL,
    content_type character varying(255) NOT NULL,
    application character varying(255) NOT NULL,
    format text NOT NULL,
    tokens text NOT NULL,
    CONSTRAINT tripal_token_formats_tripal_format_id_check CHECK ((tripal_format_id >= 0))
);


--
-- Name: tripal_token_formats_tripal_format_id_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE tripal_token_formats_tripal_format_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


--
-- Name: tripal_token_formats_tripal_format_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: -
--

ALTER SEQUENCE tripal_token_formats_tripal_format_id_seq OWNED BY tripal_token_formats.tripal_format_id;


--
-- Name: tripal_variables; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE tripal_variables (
    variable_id integer NOT NULL,
    name character varying(255) NOT NULL,
    description text NOT NULL
);


--
-- Name: TABLE tripal_variables; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE tripal_variables IS 'This table houses a list of unique variable names that can be used in the tripal_node_variables table.';


--
-- Name: tripal_variables_variable_id_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE tripal_variables_variable_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


--
-- Name: tripal_variables_variable_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: -
--

ALTER SEQUENCE tripal_variables_variable_id_seq OWNED BY tripal_variables.variable_id;


--
-- Name: tripal_views; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE tripal_views (
    setup_id integer NOT NULL,
    mview_id bigint,
    base_table integer DEFAULT 1,
    table_name character varying(255) DEFAULT ''::character varying NOT NULL,
    priority integer,
    name character varying(255) DEFAULT ''::character varying NOT NULL,
    comment text DEFAULT ''::text,
    CONSTRAINT tripal_views_mview_id_check CHECK ((mview_id >= 0)),
    CONSTRAINT tripal_views_setup_id_check CHECK ((setup_id >= 0))
);


--
-- Name: TABLE tripal_views; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE tripal_views IS 'contains the setups, their materialized view id and base table name that was used.';


--
-- Name: COLUMN tripal_views.setup_id; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN tripal_views.setup_id IS 'the id of the setup';


--
-- Name: COLUMN tripal_views.mview_id; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN tripal_views.mview_id IS 'the materialized view used for this setup';


--
-- Name: COLUMN tripal_views.base_table; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN tripal_views.base_table IS 'either TRUE (1) or FALSE (0) depending on whether the current table should be a bast table of a View';


--
-- Name: COLUMN tripal_views.table_name; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN tripal_views.table_name IS 'the table name being integrated.';


--
-- Name: COLUMN tripal_views.priority; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN tripal_views.priority IS 'when there are 2+ entries for the same table, the entry with the lightest (drupal-style) priority is used.';


--
-- Name: COLUMN tripal_views.name; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN tripal_views.name IS 'Human readable name of this setup';


--
-- Name: COLUMN tripal_views.comment; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN tripal_views.comment IS 'add notes about this views setup';


--
-- Name: tripal_views_field; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE tripal_views_field (
    setup_id bigint NOT NULL,
    column_name character varying(255) NOT NULL,
    name character varying(255) NOT NULL,
    description character varying(255) NOT NULL,
    type character varying(50) NOT NULL,
    CONSTRAINT tripal_views_field_setup_id_check CHECK ((setup_id >= 0))
);


--
-- Name: TABLE tripal_views_field; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE tripal_views_field IS 'keep track of fields available for a given table';


--
-- Name: COLUMN tripal_views_field.setup_id; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN tripal_views_field.setup_id IS 'the id of the setup';


--
-- Name: COLUMN tripal_views_field.column_name; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN tripal_views_field.column_name IS 'the name of the field in the database';


--
-- Name: COLUMN tripal_views_field.name; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN tripal_views_field.name IS 'the human-readable name of the field';


--
-- Name: COLUMN tripal_views_field.description; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN tripal_views_field.description IS 'A short description of the field -seen under the field in the views UI';


--
-- Name: COLUMN tripal_views_field.type; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN tripal_views_field.type IS 'the database type of this field (ie: int, varchar)';


--
-- Name: tripal_views_handlers; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE tripal_views_handlers (
    handler_id integer NOT NULL,
    setup_id bigint NOT NULL,
    column_name character varying(255) DEFAULT ''::character varying NOT NULL,
    handler_type character varying(50) DEFAULT ''::character varying NOT NULL,
    handler_name character varying(255) DEFAULT ''::character varying NOT NULL,
    arguments text DEFAULT ''::text,
    CONSTRAINT tripal_views_handlers_handler_id_check CHECK ((handler_id >= 0)),
    CONSTRAINT tripal_views_handlers_setup_id_check CHECK ((setup_id >= 0))
);


--
-- Name: TABLE tripal_views_handlers; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE tripal_views_handlers IS 'in formation for views: column and views handler name';


--
-- Name: COLUMN tripal_views_handlers.handler_id; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN tripal_views_handlers.handler_id IS 'the id of the handler';


--
-- Name: COLUMN tripal_views_handlers.setup_id; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN tripal_views_handlers.setup_id IS 'setup id from the tripal_views table';


--
-- Name: COLUMN tripal_views_handlers.handler_type; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN tripal_views_handlers.handler_type IS 'identifies the type of hander (e.g. field, filter, sort, argument, relationship, etc.)';


--
-- Name: COLUMN tripal_views_handlers.handler_name; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN tripal_views_handlers.handler_name IS 'the name of the handler';


--
-- Name: COLUMN tripal_views_handlers.arguments; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN tripal_views_handlers.arguments IS 'arguments that may get passed to the handler';


--
-- Name: tripal_views_handlers_handler_id_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE tripal_views_handlers_handler_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


--
-- Name: tripal_views_handlers_handler_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: -
--

ALTER SEQUENCE tripal_views_handlers_handler_id_seq OWNED BY tripal_views_handlers.handler_id;


--
-- Name: tripal_views_join; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE tripal_views_join (
    view_join_id integer NOT NULL,
    setup_id bigint NOT NULL,
    base_table character varying(255) DEFAULT ''::character varying NOT NULL,
    base_field character varying(255) DEFAULT ''::character varying NOT NULL,
    left_table character varying(255) DEFAULT ''::character varying NOT NULL,
    left_field character varying(255) DEFAULT ''::character varying NOT NULL,
    handler character varying(255) DEFAULT ''::character varying NOT NULL,
    relationship_handler character varying(255) DEFAULT 'views_handler_relationship'::character varying NOT NULL,
    relationship_only integer DEFAULT 0,
    arguments text,
    CONSTRAINT tripal_views_join_setup_id_check CHECK ((setup_id >= 0)),
    CONSTRAINT tripal_views_join_view_join_id_check CHECK ((view_join_id >= 0))
);


--
-- Name: TABLE tripal_views_join; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE tripal_views_join IS 'coordinate the joining of tables';


--
-- Name: COLUMN tripal_views_join.view_join_id; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN tripal_views_join.view_join_id IS 'the id of the join';


--
-- Name: COLUMN tripal_views_join.setup_id; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN tripal_views_join.setup_id IS 'setup id from tripal_views table';


--
-- Name: COLUMN tripal_views_join.base_table; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN tripal_views_join.base_table IS 'the name of the base table';


--
-- Name: COLUMN tripal_views_join.base_field; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN tripal_views_join.base_field IS 'the name of the base table column that will be joined';


--
-- Name: COLUMN tripal_views_join.left_table; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN tripal_views_join.left_table IS 'the table on which to perform a left join';


--
-- Name: COLUMN tripal_views_join.left_field; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN tripal_views_join.left_field IS 'the column on which to perform a left join';


--
-- Name: COLUMN tripal_views_join.handler; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN tripal_views_join.handler IS 'the name of the handler';


--
-- Name: COLUMN tripal_views_join.arguments; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN tripal_views_join.arguments IS 'arguments that may get passed to the handler';


--
-- Name: tripal_views_join_view_join_id_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE tripal_views_join_view_join_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


--
-- Name: tripal_views_join_view_join_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: -
--

ALTER SEQUENCE tripal_views_join_view_join_id_seq OWNED BY tripal_views_join.view_join_id;


--
-- Name: tripal_views_setup_id_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE tripal_views_setup_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


--
-- Name: tripal_views_setup_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: -
--

ALTER SEQUENCE tripal_views_setup_id_seq OWNED BY tripal_views.setup_id;


--
-- Name: url_alias; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE url_alias (
    pid integer NOT NULL,
    source character varying(255) DEFAULT ''::character varying NOT NULL,
    alias character varying(255) DEFAULT ''::character varying NOT NULL,
    language character varying(12) DEFAULT ''::character varying NOT NULL,
    CONSTRAINT url_alias_pid_check CHECK ((pid >= 0))
);


--
-- Name: TABLE url_alias; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE url_alias IS 'A list of URL aliases for Drupal paths; a user may visit either the source or destination path.';


--
-- Name: COLUMN url_alias.pid; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN url_alias.pid IS 'A unique path alias identifier.';


--
-- Name: COLUMN url_alias.source; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN url_alias.source IS 'The Drupal path this alias is for; e.g. node/12.';


--
-- Name: COLUMN url_alias.alias; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN url_alias.alias IS 'The alias for this path; e.g. title-of-the-story.';


--
-- Name: COLUMN url_alias.language; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN url_alias.language IS 'The language this alias is for; if ''und'', the alias will be used for unknown languages. Each Drupal path can have an alias for each supported language.';


--
-- Name: url_alias_pid_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE url_alias_pid_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


--
-- Name: url_alias_pid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: -
--

ALTER SEQUENCE url_alias_pid_seq OWNED BY url_alias.pid;


--
-- Name: user_restrictions; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE user_restrictions (
    urid integer NOT NULL,
    mask character varying(255) DEFAULT ''::character varying NOT NULL,
    type character varying(255) DEFAULT ''::character varying NOT NULL,
    subtype character varying(255) DEFAULT ''::character varying NOT NULL,
    status smallint DEFAULT 0 NOT NULL,
    expire integer DEFAULT 0 NOT NULL
);


--
-- Name: TABLE user_restrictions; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE user_restrictions IS 'Stores user restrictions.';


--
-- Name: COLUMN user_restrictions.urid; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN user_restrictions.urid IS 'Primary Key: Unique user restriction ID.';


--
-- Name: COLUMN user_restrictions.mask; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN user_restrictions.mask IS 'Text mask used for filtering restrictions.';


--
-- Name: COLUMN user_restrictions.type; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN user_restrictions.type IS 'Type of access rule: name, mail, or any value defined from a third-party module.';


--
-- Name: COLUMN user_restrictions.subtype; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN user_restrictions.subtype IS 'Sub-type of access rule.';


--
-- Name: COLUMN user_restrictions.status; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN user_restrictions.status IS 'Whether the restriction is to allow (1), or deny access (0).';


--
-- Name: COLUMN user_restrictions.expire; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN user_restrictions.expire IS 'A Unix timestamp indicating when the restriction expires.';


--
-- Name: user_restrictions_urid_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE user_restrictions_urid_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


--
-- Name: user_restrictions_urid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: -
--

ALTER SEQUENCE user_restrictions_urid_seq OWNED BY user_restrictions.urid;


--
-- Name: users; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE users (
    uid bigint DEFAULT 0 NOT NULL,
    name character varying(60) DEFAULT ''::character varying NOT NULL,
    pass character varying(128) DEFAULT ''::character varying NOT NULL,
    mail character varying(254) DEFAULT ''::character varying,
    theme character varying(255) DEFAULT ''::character varying NOT NULL,
    signature character varying(255) DEFAULT ''::character varying NOT NULL,
    signature_format character varying(255),
    created integer DEFAULT 0 NOT NULL,
    access integer DEFAULT 0 NOT NULL,
    login integer DEFAULT 0 NOT NULL,
    status smallint DEFAULT 0 NOT NULL,
    timezone character varying(32),
    language character varying(12) DEFAULT ''::character varying NOT NULL,
    picture integer DEFAULT 0 NOT NULL,
    init character varying(254) DEFAULT ''::character varying,
    data bytea,
    CONSTRAINT users_uid_check CHECK ((uid >= 0))
);


--
-- Name: TABLE users; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE users IS 'Stores user data.';


--
-- Name: COLUMN users.uid; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN users.uid IS 'Primary Key: Unique user ID.';


--
-- Name: COLUMN users.name; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN users.name IS 'Unique user name.';


--
-- Name: COLUMN users.pass; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN users.pass IS 'User''s password (hashed).';


--
-- Name: COLUMN users.mail; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN users.mail IS 'User''s e-mail address.';


--
-- Name: COLUMN users.theme; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN users.theme IS 'User''s default theme.';


--
-- Name: COLUMN users.signature; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN users.signature IS 'User''s signature.';


--
-- Name: COLUMN users.signature_format; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN users.signature_format IS 'The filter_format.format of the signature.';


--
-- Name: COLUMN users.created; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN users.created IS 'Timestamp for when user was created.';


--
-- Name: COLUMN users.access; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN users.access IS 'Timestamp for previous time user accessed the site.';


--
-- Name: COLUMN users.login; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN users.login IS 'Timestamp for user''s last login.';


--
-- Name: COLUMN users.status; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN users.status IS 'Whether the user is active(1) or blocked(0).';


--
-- Name: COLUMN users.timezone; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN users.timezone IS 'User''s time zone.';


--
-- Name: COLUMN users.language; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN users.language IS 'User''s default language.';


--
-- Name: COLUMN users.picture; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN users.picture IS 'Foreign key: file_managed.fid of user''s picture.';


--
-- Name: COLUMN users.init; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN users.init IS 'E-mail address used for initial account creation.';


--
-- Name: COLUMN users.data; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN users.data IS 'A serialized array of name value pairs that are related to the user. Any form values posted during user edit are stored and are loaded into the $user object during user_load(). Use of this field is discouraged and it will likely disappear in a future version of Drupal.';


--
-- Name: users_roles; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE users_roles (
    uid bigint DEFAULT 0 NOT NULL,
    rid bigint DEFAULT 0 NOT NULL,
    CONSTRAINT users_roles_rid_check CHECK ((rid >= 0)),
    CONSTRAINT users_roles_uid_check CHECK ((uid >= 0))
);


--
-- Name: TABLE users_roles; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE users_roles IS 'Maps users to roles.';


--
-- Name: COLUMN users_roles.uid; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN users_roles.uid IS 'Primary Key: users.uid for user.';


--
-- Name: COLUMN users_roles.rid; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN users_roles.rid IS 'Primary Key: role.rid for role.';


--
-- Name: variable; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE variable (
    name character varying(128) DEFAULT ''::character varying NOT NULL,
    value bytea NOT NULL
);


--
-- Name: TABLE variable; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE variable IS 'Named variable/value pairs created by Drupal core or any other module or theme. All variables are cached in memory at the start of every Drupal request so developers should not be careless about what is stored here.';


--
-- Name: COLUMN variable.name; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN variable.name IS 'The name of the variable.';


--
-- Name: COLUMN variable.value; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN variable.value IS 'The value of the variable.';


--
-- Name: views_display; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE views_display (
    vid bigint DEFAULT 0 NOT NULL,
    id character varying(64) DEFAULT ''::character varying NOT NULL,
    display_title character varying(64) DEFAULT ''::character varying NOT NULL,
    display_plugin character varying(64) DEFAULT ''::character varying NOT NULL,
    "position" integer DEFAULT 0,
    display_options text,
    CONSTRAINT views_display_vid_check CHECK ((vid >= 0))
);


--
-- Name: TABLE views_display; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE views_display IS 'Stores information about each display attached to a view.';


--
-- Name: COLUMN views_display.vid; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN views_display.vid IS 'The view this display is attached to.';


--
-- Name: COLUMN views_display.id; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN views_display.id IS 'An identifier for this display; usually generated from the display_plugin, so should be something::text like page or page_1 or block_2, etc.';


--
-- Name: COLUMN views_display.display_title; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN views_display.display_title IS 'The title of the display, viewable by the administrator.';


--
-- Name: COLUMN views_display.display_plugin; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN views_display.display_plugin IS 'The type of the display. Usually page, block or embed, but is pluggable so may be other things.';


--
-- Name: COLUMN views_display."position"; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN views_display."position" IS 'The order in which this display is loaded.';


--
-- Name: COLUMN views_display.display_options; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN views_display.display_options IS 'A serialized array of options for this display; it contains options that are generally only pertinent to that display plugin type.';


--
-- Name: views_view; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE views_view (
    vid integer NOT NULL,
    name character varying(128) DEFAULT ''::character varying NOT NULL,
    description character varying(255) DEFAULT ''::character varying,
    tag character varying(255) DEFAULT ''::character varying,
    base_table character varying(64) DEFAULT ''::character varying NOT NULL,
    human_name character varying(255) DEFAULT ''::character varying,
    core integer DEFAULT 0,
    CONSTRAINT views_view_vid_check CHECK ((vid >= 0))
);


--
-- Name: TABLE views_view; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE views_view IS 'Stores the general data for a view.';


--
-- Name: COLUMN views_view.vid; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN views_view.vid IS 'The view ID of the field, defined by the database.';


--
-- Name: COLUMN views_view.name; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN views_view.name IS 'The unique name of the view. This is the primary field views are loaded from, and is used so that views may be internal and not necessarily in the database. May only be alphanumeric characters plus underscores.';


--
-- Name: COLUMN views_view.description; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN views_view.description IS 'A description of the view for the admin interface.';


--
-- Name: COLUMN views_view.tag; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN views_view.tag IS 'A tag used to group/sort views in the admin interface';


--
-- Name: COLUMN views_view.base_table; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN views_view.base_table IS 'What table this view is based on, such as node, user, comment, or term.';


--
-- Name: COLUMN views_view.human_name; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN views_view.human_name IS 'A human readable name used to be displayed in the admin interface';


--
-- Name: COLUMN views_view.core; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN views_view.core IS 'Stores the drupal core version of the view.';


--
-- Name: views_view_vid_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE views_view_vid_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


--
-- Name: views_view_vid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: -
--

ALTER SEQUENCE views_view_vid_seq OWNED BY views_view.vid;


--
-- Name: watchdog; Type: TABLE; Schema: public; Owner: -; Tablespace: 
--

CREATE TABLE watchdog (
    wid integer NOT NULL,
    uid integer DEFAULT 0 NOT NULL,
    type character varying(64) DEFAULT ''::character varying NOT NULL,
    message text NOT NULL,
    variables bytea NOT NULL,
    severity integer DEFAULT 0 NOT NULL,
    link character varying(255) DEFAULT ''::character varying,
    location text NOT NULL,
    referer text,
    hostname character varying(128) DEFAULT ''::character varying NOT NULL,
    "timestamp" integer DEFAULT 0 NOT NULL,
    CONSTRAINT watchdog_severity_check CHECK ((severity >= 0))
);


--
-- Name: TABLE watchdog; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON TABLE watchdog IS 'Table that contains logs of all system events.';


--
-- Name: COLUMN watchdog.wid; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN watchdog.wid IS 'Primary Key: Unique watchdog event ID.';


--
-- Name: COLUMN watchdog.uid; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN watchdog.uid IS 'The users.uid of the user who triggered the event.';


--
-- Name: COLUMN watchdog.type; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN watchdog.type IS 'Type of log message, for example "user" or "page not found."';


--
-- Name: COLUMN watchdog.message; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN watchdog.message IS 'Text of log message to be passed into the t() function.';


--
-- Name: COLUMN watchdog.variables; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN watchdog.variables IS 'Serialized array of variables that match the message string and that is passed into the t() function.';


--
-- Name: COLUMN watchdog.severity; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN watchdog.severity IS 'The severity level of the event; ranges from 0 (Emergency) to 7 (Debug)';


--
-- Name: COLUMN watchdog.link; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN watchdog.link IS 'Link to view the result of the event.';


--
-- Name: COLUMN watchdog.location; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN watchdog.location IS 'URL of the origin of the event.';


--
-- Name: COLUMN watchdog.referer; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN watchdog.referer IS 'URL of referring page.';


--
-- Name: COLUMN watchdog.hostname; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN watchdog.hostname IS 'Hostname of the user who triggered the event.';


--
-- Name: COLUMN watchdog."timestamp"; Type: COMMENT; Schema: public; Owner: -
--

COMMENT ON COLUMN watchdog."timestamp" IS 'Unix timestamp of when event occurred.';


--
-- Name: watchdog_wid_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE watchdog_wid_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


--
-- Name: watchdog_wid_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: -
--

ALTER SEQUENCE watchdog_wid_seq OWNED BY watchdog.wid;


--
-- Name: aid; Type: DEFAULT; Schema: public; Owner: -
--

ALTER TABLE ONLY authmap ALTER COLUMN aid SET DEFAULT nextval('authmap_aid_seq'::regclass);


--
-- Name: archive_type_id; Type: DEFAULT; Schema: public; Owner: -
--

ALTER TABLE ONLY bims_archive_type ALTER COLUMN archive_type_id SET DEFAULT nextval('bims_archive_type_archive_type_id_seq'::regclass);


--
-- Name: crop_id; Type: DEFAULT; Schema: public; Owner: -
--

ALTER TABLE ONLY bims_crop ALTER COLUMN crop_id SET DEFAULT nextval('bims_crop_crop_id_seq'::regclass);


--
-- Name: file_id; Type: DEFAULT; Schema: public; Owner: -
--

ALTER TABLE ONLY bims_file ALTER COLUMN file_id SET DEFAULT nextval('bims_file_file_id_seq'::regclass);


--
-- Name: instruction_id; Type: DEFAULT; Schema: public; Owner: -
--

ALTER TABLE ONLY bims_instruction ALTER COLUMN instruction_id SET DEFAULT nextval('bims_instruction_instruction_id_seq'::regclass);


--
-- Name: list_id; Type: DEFAULT; Schema: public; Owner: -
--

ALTER TABLE ONLY bims_list ALTER COLUMN list_id SET DEFAULT nextval('bims_list_list_id_seq'::regclass);


--
-- Name: node_id; Type: DEFAULT; Schema: public; Owner: -
--

ALTER TABLE ONLY bims_node ALTER COLUMN node_id SET DEFAULT nextval('bims_node_node_id_seq'::regclass);


--
-- Name: relationship_id; Type: DEFAULT; Schema: public; Owner: -
--

ALTER TABLE ONLY bims_node_relationship ALTER COLUMN relationship_id SET DEFAULT nextval('bims_node_relationship_relationship_id_seq'::regclass);


--
-- Name: program_member_id; Type: DEFAULT; Schema: public; Owner: -
--

ALTER TABLE ONLY bims_program_member ALTER COLUMN program_member_id SET DEFAULT nextval('bims_program_member_program_member_id_seq'::regclass);


--
-- Name: bid; Type: DEFAULT; Schema: public; Owner: -
--

ALTER TABLE ONLY block ALTER COLUMN bid SET DEFAULT nextval('block_bid_seq'::regclass);


--
-- Name: bid; Type: DEFAULT; Schema: public; Owner: -
--

ALTER TABLE ONLY block_custom ALTER COLUMN bid SET DEFAULT nextval('block_custom_bid_seq'::regclass);


--
-- Name: iid; Type: DEFAULT; Schema: public; Owner: -
--

ALTER TABLE ONLY blocked_ips ALTER COLUMN iid SET DEFAULT nextval('blocked_ips_iid_seq'::regclass);


--
-- Name: cid; Type: DEFAULT; Schema: public; Owner: -
--

ALTER TABLE ONLY comment ALTER COLUMN cid SET DEFAULT nextval('comment_cid_seq'::regclass);


--
-- Name: cid; Type: DEFAULT; Schema: public; Owner: -
--

ALTER TABLE ONLY contact ALTER COLUMN cid SET DEFAULT nextval('contact_cid_seq'::regclass);


--
-- Name: dfid; Type: DEFAULT; Schema: public; Owner: -
--

ALTER TABLE ONLY date_formats ALTER COLUMN dfid SET DEFAULT nextval('date_formats_dfid_seq'::regclass);


--
-- Name: group_id; Type: DEFAULT; Schema: public; Owner: -
--

ALTER TABLE ONLY do_group ALTER COLUMN group_id SET DEFAULT nextval('do_group_group_id_seq'::regclass);


--
-- Name: node_id; Type: DEFAULT; Schema: public; Owner: -
--

ALTER TABLE ONLY do_node ALTER COLUMN node_id SET DEFAULT nextval('do_node_node_id_seq'::regclass);


--
-- Name: relationship_id; Type: DEFAULT; Schema: public; Owner: -
--

ALTER TABLE ONLY do_node_relationship ALTER COLUMN relationship_id SET DEFAULT nextval('do_node_relationship_relationship_id_seq'::regclass);


--
-- Name: overview_id; Type: DEFAULT; Schema: public; Owner: -
--

ALTER TABLE ONLY do_overview ALTER COLUMN overview_id SET DEFAULT nextval('do_overview_overview_id_seq'::regclass);


--
-- Name: id; Type: DEFAULT; Schema: public; Owner: -
--

ALTER TABLE ONLY field_config ALTER COLUMN id SET DEFAULT nextval('field_config_id_seq'::regclass);


--
-- Name: id; Type: DEFAULT; Schema: public; Owner: -
--

ALTER TABLE ONLY field_config_instance ALTER COLUMN id SET DEFAULT nextval('field_config_instance_id_seq'::regclass);


--
-- Name: fid; Type: DEFAULT; Schema: public; Owner: -
--

ALTER TABLE ONLY file_managed ALTER COLUMN fid SET DEFAULT nextval('file_managed_fid_seq'::regclass);


--
-- Name: fid; Type: DEFAULT; Schema: public; Owner: -
--

ALTER TABLE ONLY flood ALTER COLUMN fid SET DEFAULT nextval('flood_fid_seq'::regclass);


--
-- Name: category_id; Type: DEFAULT; Schema: public; Owner: -
--

ALTER TABLE ONLY gensas_category ALTER COLUMN category_id SET DEFAULT nextval('gensas_category_category_id_seq'::regclass);


--
-- Name: db_id; Type: DEFAULT; Schema: public; Owner: -
--

ALTER TABLE ONLY gensas_db ALTER COLUMN db_id SET DEFAULT nextval('gensas_db_db_id_seq'::regclass);


--
-- Name: file_id; Type: DEFAULT; Schema: public; Owner: -
--

ALTER TABLE ONLY gensas_files ALTER COLUMN file_id SET DEFAULT nextval('gensas_files_file_id_seq'::regclass);


--
-- Name: group_id; Type: DEFAULT; Schema: public; Owner: -
--

ALTER TABLE ONLY gensas_group ALTER COLUMN group_id SET DEFAULT nextval('gensas_group_group_id_seq'::regclass);


--
-- Name: id; Type: DEFAULT; Schema: public; Owner: -
--

ALTER TABLE ONLY gensas_job ALTER COLUMN id SET DEFAULT nextval('gensas_job_id_seq'::regclass);


--
-- Name: library_id; Type: DEFAULT; Schema: public; Owner: -
--

ALTER TABLE ONLY gensas_library ALTER COLUMN library_id SET DEFAULT nextval('gensas_library_library_id_seq'::regclass);


--
-- Name: library_type_id; Type: DEFAULT; Schema: public; Owner: -
--

ALTER TABLE ONLY gensas_library_type ALTER COLUMN library_type_id SET DEFAULT nextval('gensas_library_type_library_type_id_seq'::regclass);


--
-- Name: param_group_id; Type: DEFAULT; Schema: public; Owner: -
--

ALTER TABLE ONLY gensas_param_group ALTER COLUMN param_group_id SET DEFAULT nextval('gensas_param_group_param_group_id_seq'::regclass);


--
-- Name: project_type_id; Type: DEFAULT; Schema: public; Owner: -
--

ALTER TABLE ONLY gensas_project_type ALTER COLUMN project_type_id SET DEFAULT nextval('gensas_project_type_project_type_id_seq'::regclass);


--
-- Name: resource_id; Type: DEFAULT; Schema: public; Owner: -
--

ALTER TABLE ONLY gensas_resource ALTER COLUMN resource_id SET DEFAULT nextval('gensas_resource_resource_id_seq'::regclass);


--
-- Name: seq_id; Type: DEFAULT; Schema: public; Owner: -
--

ALTER TABLE ONLY gensas_seq ALTER COLUMN seq_id SET DEFAULT nextval('gensas_seq_seq_id_seq'::regclass);


--
-- Name: seq_group_id; Type: DEFAULT; Schema: public; Owner: -
--

ALTER TABLE ONLY gensas_seq_group ALTER COLUMN seq_group_id SET DEFAULT nextval('gensas_seq_group_seq_group_id_seq'::regclass);


--
-- Name: tool_id; Type: DEFAULT; Schema: public; Owner: -
--

ALTER TABLE ONLY gensas_tool ALTER COLUMN tool_id SET DEFAULT nextval('gensas_tool_tool_id_seq'::regclass);


--
-- Name: tool_param_id; Type: DEFAULT; Schema: public; Owner: -
--

ALTER TABLE ONLY gensas_tool_param ALTER COLUMN tool_param_id SET DEFAULT nextval('gensas_tool_param_tool_param_id_seq'::regclass);


--
-- Name: tool_param_type_id; Type: DEFAULT; Schema: public; Owner: -
--

ALTER TABLE ONLY gensas_tool_param_type ALTER COLUMN tool_param_type_id SET DEFAULT nextval('gensas_tool_param_type_tool_param_type_id_seq'::regclass);


--
-- Name: tool_type_id; Type: DEFAULT; Schema: public; Owner: -
--

ALTER TABLE ONLY gensas_tool_type ALTER COLUMN tool_type_id SET DEFAULT nextval('gensas_tool_type_tool_type_id_seq'::regclass);


--
-- Name: ieid; Type: DEFAULT; Schema: public; Owner: -
--

ALTER TABLE ONLY image_effects ALTER COLUMN ieid SET DEFAULT nextval('image_effects_ieid_seq'::regclass);


--
-- Name: isid; Type: DEFAULT; Schema: public; Owner: -
--

ALTER TABLE ONLY image_styles ALTER COLUMN isid SET DEFAULT nextval('image_styles_isid_seq'::regclass);


--
-- Name: data_valid_id; Type: DEFAULT; Schema: public; Owner: -
--

ALTER TABLE ONLY mcl_data_valid ALTER COLUMN data_valid_id SET DEFAULT nextval('mcl_data_valid_data_valid_id_seq'::regclass);


--
-- Name: data_valid_type_id; Type: DEFAULT; Schema: public; Owner: -
--

ALTER TABLE ONLY mcl_data_valid_type ALTER COLUMN data_valid_type_id SET DEFAULT nextval('mcl_data_valid_type_data_valid_type_id_seq'::regclass);


--
-- Name: file_id; Type: DEFAULT; Schema: public; Owner: -
--

ALTER TABLE ONLY mcl_file ALTER COLUMN file_id SET DEFAULT nextval('mcl_file_file_id_seq'::regclass);


--
-- Name: job_id; Type: DEFAULT; Schema: public; Owner: -
--

ALTER TABLE ONLY mcl_job ALTER COLUMN job_id SET DEFAULT nextval('mcl_job_job_id_seq'::regclass);


--
-- Name: template_id; Type: DEFAULT; Schema: public; Owner: -
--

ALTER TABLE ONLY mcl_template ALTER COLUMN template_id SET DEFAULT nextval('mcl_template_template_id_seq'::regclass);


--
-- Name: template_type_id; Type: DEFAULT; Schema: public; Owner: -
--

ALTER TABLE ONLY mcl_template_type ALTER COLUMN template_type_id SET DEFAULT nextval('mcl_template_type_template_type_id_seq'::regclass);


--
-- Name: var_id; Type: DEFAULT; Schema: public; Owner: -
--

ALTER TABLE ONLY mcl_var ALTER COLUMN var_id SET DEFAULT nextval('mcl_var_var_id_seq'::regclass);


--
-- Name: cron_job_id; Type: DEFAULT; Schema: public; Owner: -
--

ALTER TABLE ONLY mdlu_cron_job ALTER COLUMN cron_job_id SET DEFAULT nextval('mdlu_cron_job_cron_job_id_seq'::regclass);


--
-- Name: database_id; Type: DEFAULT; Schema: public; Owner: -
--

ALTER TABLE ONLY mdlu_database ALTER COLUMN database_id SET DEFAULT nextval('mdlu_database_database_id_seq'::regclass);


--
-- Name: database_type_id; Type: DEFAULT; Schema: public; Owner: -
--

ALTER TABLE ONLY mdlu_database_type ALTER COLUMN database_type_id SET DEFAULT nextval('mdlu_database_type_database_type_id_seq'::regclass);


--
-- Name: log_id; Type: DEFAULT; Schema: public; Owner: -
--

ALTER TABLE ONLY mdlu_log ALTER COLUMN log_id SET DEFAULT nextval('mdlu_log_log_id_seq'::regclass);


--
-- Name: mlid; Type: DEFAULT; Schema: public; Owner: -
--

ALTER TABLE ONLY menu_links ALTER COLUMN mlid SET DEFAULT nextval('menu_links_mlid_seq'::regclass);


--
-- Name: nid; Type: DEFAULT; Schema: public; Owner: -
--

ALTER TABLE ONLY node ALTER COLUMN nid SET DEFAULT nextval('node_nid_seq'::regclass);


--
-- Name: vid; Type: DEFAULT; Schema: public; Owner: -
--

ALTER TABLE ONLY node_revision ALTER COLUMN vid SET DEFAULT nextval('node_revision_vid_seq'::regclass);


--
-- Name: item_id; Type: DEFAULT; Schema: public; Owner: -
--

ALTER TABLE ONLY queue ALTER COLUMN item_id SET DEFAULT nextval('queue_item_id_seq'::regclass);


--
-- Name: rid; Type: DEFAULT; Schema: public; Owner: -
--

ALTER TABLE ONLY role ALTER COLUMN rid SET DEFAULT nextval('role_rid_seq'::regclass);


--
-- Name: value; Type: DEFAULT; Schema: public; Owner: -
--

ALTER TABLE ONLY sequences ALTER COLUMN value SET DEFAULT nextval('sequences_value_seq'::regclass);


--
-- Name: tid; Type: DEFAULT; Schema: public; Owner: -
--

ALTER TABLE ONLY taxonomy_term_data ALTER COLUMN tid SET DEFAULT nextval('taxonomy_term_data_tid_seq'::regclass);


--
-- Name: vid; Type: DEFAULT; Schema: public; Owner: -
--

ALTER TABLE ONLY taxonomy_vocabulary ALTER COLUMN vid SET DEFAULT nextval('taxonomy_vocabulary_vid_seq'::regclass);


--
-- Name: id; Type: DEFAULT; Schema: public; Owner: -
--

ALTER TABLE ONLY tmp ALTER COLUMN id SET DEFAULT nextval('tmp_id_seq'::regclass);


--
-- Name: table_id; Type: DEFAULT; Schema: public; Owner: -
--

ALTER TABLE ONLY tripal_custom_tables ALTER COLUMN table_id SET DEFAULT nextval('tripal_custom_tables_table_id_seq'::regclass);


--
-- Name: cv_default_id; Type: DEFAULT; Schema: public; Owner: -
--

ALTER TABLE ONLY tripal_cv_defaults ALTER COLUMN cv_default_id SET DEFAULT nextval('tripal_cv_defaults_cv_default_id_seq'::regclass);


--
-- Name: obo_id; Type: DEFAULT; Schema: public; Owner: -
--

ALTER TABLE ONLY tripal_cv_obo ALTER COLUMN obo_id SET DEFAULT nextval('tripal_cv_obo_obo_id_seq'::regclass);


--
-- Name: job_id; Type: DEFAULT; Schema: public; Owner: -
--

ALTER TABLE ONLY tripal_jobs ALTER COLUMN job_id SET DEFAULT nextval('tripal_jobs_job_id_seq'::regclass);


--
-- Name: mview_id; Type: DEFAULT; Schema: public; Owner: -
--

ALTER TABLE ONLY tripal_mviews ALTER COLUMN mview_id SET DEFAULT nextval('tripal_mviews_mview_id_seq'::regclass);


--
-- Name: node_variable_id; Type: DEFAULT; Schema: public; Owner: -
--

ALTER TABLE ONLY tripal_node_variables ALTER COLUMN node_variable_id SET DEFAULT nextval('tripal_node_variables_node_variable_id_seq'::regclass);


--
-- Name: toc_item_id; Type: DEFAULT; Schema: public; Owner: -
--

ALTER TABLE ONLY tripal_toc ALTER COLUMN toc_item_id SET DEFAULT nextval('tripal_toc_toc_item_id_seq'::regclass);


--
-- Name: tripal_format_id; Type: DEFAULT; Schema: public; Owner: -
--

ALTER TABLE ONLY tripal_token_formats ALTER COLUMN tripal_format_id SET DEFAULT nextval('tripal_token_formats_tripal_format_id_seq'::regclass);


--
-- Name: variable_id; Type: DEFAULT; Schema: public; Owner: -
--

ALTER TABLE ONLY tripal_variables ALTER COLUMN variable_id SET DEFAULT nextval('tripal_variables_variable_id_seq'::regclass);


--
-- Name: setup_id; Type: DEFAULT; Schema: public; Owner: -
--

ALTER TABLE ONLY tripal_views ALTER COLUMN setup_id SET DEFAULT nextval('tripal_views_setup_id_seq'::regclass);


--
-- Name: handler_id; Type: DEFAULT; Schema: public; Owner: -
--

ALTER TABLE ONLY tripal_views_handlers ALTER COLUMN handler_id SET DEFAULT nextval('tripal_views_handlers_handler_id_seq'::regclass);


--
-- Name: view_join_id; Type: DEFAULT; Schema: public; Owner: -
--

ALTER TABLE ONLY tripal_views_join ALTER COLUMN view_join_id SET DEFAULT nextval('tripal_views_join_view_join_id_seq'::regclass);


--
-- Name: pid; Type: DEFAULT; Schema: public; Owner: -
--

ALTER TABLE ONLY url_alias ALTER COLUMN pid SET DEFAULT nextval('url_alias_pid_seq'::regclass);


--
-- Name: urid; Type: DEFAULT; Schema: public; Owner: -
--

ALTER TABLE ONLY user_restrictions ALTER COLUMN urid SET DEFAULT nextval('user_restrictions_urid_seq'::regclass);


--
-- Name: vid; Type: DEFAULT; Schema: public; Owner: -
--

ALTER TABLE ONLY views_view ALTER COLUMN vid SET DEFAULT nextval('views_view_vid_seq'::regclass);


--
-- Name: wid; Type: DEFAULT; Schema: public; Owner: -
--

ALTER TABLE ONLY watchdog ALTER COLUMN wid SET DEFAULT nextval('watchdog_wid_seq'::regclass);


--
-- Name: actions_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY actions
    ADD CONSTRAINT actions_pkey PRIMARY KEY (aid);


--
-- Name: authmap_authname_key; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY authmap
    ADD CONSTRAINT authmap_authname_key UNIQUE (authname);


--
-- Name: authmap_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY authmap
    ADD CONSTRAINT authmap_pkey PRIMARY KEY (aid);


--
-- Name: batch_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY batch
    ADD CONSTRAINT batch_pkey PRIMARY KEY (bid);


--
-- Name: bims_archive_type_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY bims_archive_type
    ADD CONSTRAINT bims_archive_type_pkey PRIMARY KEY (archive_type_id);


--
-- Name: bims_crop_organism_ukey_bims_crop_organism_crop_id_organism_id_; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY bims_crop_organism
    ADD CONSTRAINT bims_crop_organism_ukey_bims_crop_organism_crop_id_organism_id_ UNIQUE (crop_id, organism_id);


--
-- Name: bims_crop_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY bims_crop
    ADD CONSTRAINT bims_crop_pkey PRIMARY KEY (crop_id);


--
-- Name: bims_file_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY bims_file
    ADD CONSTRAINT bims_file_pkey PRIMARY KEY (file_id);


--
-- Name: bims_instruction_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY bims_instruction
    ADD CONSTRAINT bims_instruction_pkey PRIMARY KEY (instruction_id);


--
-- Name: bims_list_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY bims_list
    ADD CONSTRAINT bims_list_pkey PRIMARY KEY (list_id);


--
-- Name: bims_mview_cross_stats_node_id_nd_experiment_id_cvterm_id_key; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY bims_mview_cross_stats
    ADD CONSTRAINT bims_mview_cross_stats_node_id_nd_experiment_id_cvterm_id_key UNIQUE (node_id, nd_experiment_id, cvterm_id);


--
-- Name: bims_mview_descriptor_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY bims_mview_descriptor
    ADD CONSTRAINT bims_mview_descriptor_pkey PRIMARY KEY (cvterm_id);


--
-- Name: bims_mview_phenotype_stats_node_id_cvterm_id_key; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY bims_mview_phenotype_stats
    ADD CONSTRAINT bims_mview_phenotype_stats_node_id_cvterm_id_key UNIQUE (node_id, cvterm_id);


--
-- Name: bims_mview_stock_stats_ukey_bims_mview_stock_stats_node_id_stoc; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY bims_mview_stock_stats
    ADD CONSTRAINT bims_mview_stock_stats_ukey_bims_mview_stock_stats_node_id_stoc UNIQUE (node_id, stock_id, cvterm_id);


--
-- Name: bims_node_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY bims_node
    ADD CONSTRAINT bims_node_pkey PRIMARY KEY (node_id);


--
-- Name: bims_node_relationship_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY bims_node_relationship
    ADD CONSTRAINT bims_node_relationship_pkey PRIMARY KEY (relationship_id);


--
-- Name: bims_node_relationship_ukey_bims_node_relationship_parent_id_ch; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY bims_node_relationship
    ADD CONSTRAINT bims_node_relationship_ukey_bims_node_relationship_parent_id_ch UNIQUE (parent_id, child_id);


--
-- Name: bims_node_ukey_bims_node_name_root_id_owner_id_key; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY bims_node
    ADD CONSTRAINT bims_node_ukey_bims_node_name_root_id_owner_id_key UNIQUE (name, root_id, owner_id);


--
-- Name: bims_program_member_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY bims_program_member
    ADD CONSTRAINT bims_program_member_pkey PRIMARY KEY (program_member_id);


--
-- Name: bims_program_member_ukey_bims_program_member_program_id_user_id; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY bims_program_member
    ADD CONSTRAINT bims_program_member_ukey_bims_program_member_program_id_user_id UNIQUE (program_id, user_id);


--
-- Name: bims_user_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY bims_user
    ADD CONSTRAINT bims_user_pkey PRIMARY KEY (user_id);


--
-- Name: block_custom_info_key; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY block_custom
    ADD CONSTRAINT block_custom_info_key UNIQUE (info);


--
-- Name: block_custom_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY block_custom
    ADD CONSTRAINT block_custom_pkey PRIMARY KEY (bid);


--
-- Name: block_node_type_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY block_node_type
    ADD CONSTRAINT block_node_type_pkey PRIMARY KEY (module, delta, type);


--
-- Name: block_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY block
    ADD CONSTRAINT block_pkey PRIMARY KEY (bid);


--
-- Name: block_role_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY block_role
    ADD CONSTRAINT block_role_pkey PRIMARY KEY (module, delta, rid);


--
-- Name: block_tmd_key; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY block
    ADD CONSTRAINT block_tmd_key UNIQUE (theme, module, delta);


--
-- Name: blocked_ips_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY blocked_ips
    ADD CONSTRAINT blocked_ips_pkey PRIMARY KEY (iid);


--
-- Name: book_nid_key; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY book
    ADD CONSTRAINT book_nid_key UNIQUE (nid);


--
-- Name: book_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY book
    ADD CONSTRAINT book_pkey PRIMARY KEY (mlid);


--
-- Name: cache_block_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY cache_block
    ADD CONSTRAINT cache_block_pkey PRIMARY KEY (cid);


--
-- Name: cache_bootstrap_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY cache_bootstrap
    ADD CONSTRAINT cache_bootstrap_pkey PRIMARY KEY (cid);


--
-- Name: cache_field_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY cache_field
    ADD CONSTRAINT cache_field_pkey PRIMARY KEY (cid);


--
-- Name: cache_filter_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY cache_filter
    ADD CONSTRAINT cache_filter_pkey PRIMARY KEY (cid);


--
-- Name: cache_form_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY cache_form
    ADD CONSTRAINT cache_form_pkey PRIMARY KEY (cid);


--
-- Name: cache_image_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY cache_image
    ADD CONSTRAINT cache_image_pkey PRIMARY KEY (cid);


--
-- Name: cache_menu_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY cache_menu
    ADD CONSTRAINT cache_menu_pkey PRIMARY KEY (cid);


--
-- Name: cache_page_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY cache_page
    ADD CONSTRAINT cache_page_pkey PRIMARY KEY (cid);


--
-- Name: cache_path_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY cache_path
    ADD CONSTRAINT cache_path_pkey PRIMARY KEY (cid);


--
-- Name: cache_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY cache
    ADD CONSTRAINT cache_pkey PRIMARY KEY (cid);


--
-- Name: cache_update_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY cache_update
    ADD CONSTRAINT cache_update_pkey PRIMARY KEY (cid);


--
-- Name: cache_views_data_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY cache_views_data
    ADD CONSTRAINT cache_views_data_pkey PRIMARY KEY (cid);


--
-- Name: cache_views_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY cache_views
    ADD CONSTRAINT cache_views_pkey PRIMARY KEY (cid);


--
-- Name: comment_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY comment
    ADD CONSTRAINT comment_pkey PRIMARY KEY (cid);


--
-- Name: contact_category_key; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY contact
    ADD CONSTRAINT contact_category_key UNIQUE (category);


--
-- Name: contact_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY contact
    ADD CONSTRAINT contact_pkey PRIMARY KEY (cid);


--
-- Name: ctools_css_cache_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY ctools_css_cache
    ADD CONSTRAINT ctools_css_cache_pkey PRIMARY KEY (cid);


--
-- Name: ctools_object_cache_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY ctools_object_cache
    ADD CONSTRAINT ctools_object_cache_pkey PRIMARY KEY (sid, obj, name);


--
-- Name: date_format_locale_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY date_format_locale
    ADD CONSTRAINT date_format_locale_pkey PRIMARY KEY (type, language);


--
-- Name: date_format_type_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY date_format_type
    ADD CONSTRAINT date_format_type_pkey PRIMARY KEY (type);


--
-- Name: date_formats_formats_key; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY date_formats
    ADD CONSTRAINT date_formats_formats_key UNIQUE (format, type);


--
-- Name: date_formats_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY date_formats
    ADD CONSTRAINT date_formats_pkey PRIMARY KEY (dfid);


--
-- Name: do_group_data_overview_name_key; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY do_group
    ADD CONSTRAINT do_group_data_overview_name_key UNIQUE (name);


--
-- Name: do_group_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY do_group
    ADD CONSTRAINT do_group_pkey PRIMARY KEY (group_id);


--
-- Name: do_node_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY do_node
    ADD CONSTRAINT do_node_pkey PRIMARY KEY (node_id);


--
-- Name: do_node_relationship_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY do_node_relationship
    ADD CONSTRAINT do_node_relationship_pkey PRIMARY KEY (relationship_id);


--
-- Name: do_node_relationship_ukey_do_node_relationship_parent_id_child_; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY do_node_relationship
    ADD CONSTRAINT do_node_relationship_ukey_do_node_relationship_parent_id_child_ UNIQUE (parent_id, child_id);


--
-- Name: do_overview_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY do_overview
    ADD CONSTRAINT do_overview_pkey PRIMARY KEY (overview_id);


--
-- Name: do_user_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY do_user
    ADD CONSTRAINT do_user_pkey PRIMARY KEY (user_id);


--
-- Name: field_config_instance_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY field_config_instance
    ADD CONSTRAINT field_config_instance_pkey PRIMARY KEY (id);


--
-- Name: field_config_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY field_config
    ADD CONSTRAINT field_config_pkey PRIMARY KEY (id);


--
-- Name: field_data_body_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY field_data_body
    ADD CONSTRAINT field_data_body_pkey PRIMARY KEY (entity_type, entity_id, deleted, delta, language);


--
-- Name: field_data_comment_body_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY field_data_comment_body
    ADD CONSTRAINT field_data_comment_body_pkey PRIMARY KEY (entity_type, entity_id, deleted, delta, language);


--
-- Name: field_data_field_first_anme_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY field_data_field_first_anme
    ADD CONSTRAINT field_data_field_first_anme_pkey PRIMARY KEY (entity_type, entity_id, deleted, delta, language);


--
-- Name: field_data_field_image_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY field_data_field_image
    ADD CONSTRAINT field_data_field_image_pkey PRIMARY KEY (entity_type, entity_id, deleted, delta, language);


--
-- Name: field_data_field_tags_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY field_data_field_tags
    ADD CONSTRAINT field_data_field_tags_pkey PRIMARY KEY (entity_type, entity_id, deleted, delta, language);


--
-- Name: field_revision_body_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY field_revision_body
    ADD CONSTRAINT field_revision_body_pkey PRIMARY KEY (entity_type, entity_id, revision_id, deleted, delta, language);


--
-- Name: field_revision_comment_body_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY field_revision_comment_body
    ADD CONSTRAINT field_revision_comment_body_pkey PRIMARY KEY (entity_type, entity_id, revision_id, deleted, delta, language);


--
-- Name: field_revision_field_first_anme_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY field_revision_field_first_anme
    ADD CONSTRAINT field_revision_field_first_anme_pkey PRIMARY KEY (entity_type, entity_id, revision_id, deleted, delta, language);


--
-- Name: field_revision_field_image_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY field_revision_field_image
    ADD CONSTRAINT field_revision_field_image_pkey PRIMARY KEY (entity_type, entity_id, revision_id, deleted, delta, language);


--
-- Name: field_revision_field_tags_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY field_revision_field_tags
    ADD CONSTRAINT field_revision_field_tags_pkey PRIMARY KEY (entity_type, entity_id, revision_id, deleted, delta, language);


--
-- Name: file_managed_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY file_managed
    ADD CONSTRAINT file_managed_pkey PRIMARY KEY (fid);


--
-- Name: file_managed_uri_key; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY file_managed
    ADD CONSTRAINT file_managed_uri_key UNIQUE (uri);


--
-- Name: file_usage_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY file_usage
    ADD CONSTRAINT file_usage_pkey PRIMARY KEY (fid, type, id, module);


--
-- Name: filter_format_name_key; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY filter_format
    ADD CONSTRAINT filter_format_name_key UNIQUE (name);


--
-- Name: filter_format_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY filter_format
    ADD CONSTRAINT filter_format_pkey PRIMARY KEY (format);


--
-- Name: filter_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY filter
    ADD CONSTRAINT filter_pkey PRIMARY KEY (format, name);


--
-- Name: flood_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY flood
    ADD CONSTRAINT flood_pkey PRIMARY KEY (fid);


--
-- Name: gensas_category_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY gensas_category
    ADD CONSTRAINT gensas_category_pkey PRIMARY KEY (category_id);


--
-- Name: gensas_db_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY gensas_db
    ADD CONSTRAINT gensas_db_pkey PRIMARY KEY (db_id);


--
-- Name: gensas_db_ukey_gensas_db_db_name_key; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY gensas_db
    ADD CONSTRAINT gensas_db_ukey_gensas_db_db_name_key UNIQUE (db_name);


--
-- Name: gensas_expire_ukey_gensas_expire_uq1_key; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY gensas_expire
    ADD CONSTRAINT gensas_expire_ukey_gensas_expire_uq1_key UNIQUE (task_id);


--
-- Name: gensas_gff3_job_id_id_key; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY gensas_gff3
    ADD CONSTRAINT gensas_gff3_job_id_id_key UNIQUE (job_id, id);


--
-- Name: gensas_group_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY gensas_group
    ADD CONSTRAINT gensas_group_pkey PRIMARY KEY (group_id);


--
-- Name: gensas_group_seq_ukey_gensas_group_seq_group_id_group_seq_id_ke; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY gensas_group_seq
    ADD CONSTRAINT gensas_group_seq_ukey_gensas_group_seq_group_id_group_seq_id_ke UNIQUE (group_id, seq_id);


--
-- Name: gensas_group_task_ukey_gensas_group_task_group_id_task_id_key; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY gensas_group_task
    ADD CONSTRAINT gensas_group_task_ukey_gensas_group_task_group_id_task_id_key UNIQUE (group_id, task_id);


--
-- Name: gensas_group_user_ukey_gensas_group_user_group_id_user_id_key; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY gensas_group_user
    ADD CONSTRAINT gensas_group_user_ukey_gensas_group_user_group_id_user_id_key UNIQUE (group_id, user_id);


--
-- Name: gensas_job_files_ukey_gensas_job_files_job_id_file_id_key; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY gensas_job_files
    ADD CONSTRAINT gensas_job_files_ukey_gensas_job_files_job_id_file_id_key UNIQUE (job_id, file_id);


--
-- Name: gensas_job_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY gensas_job
    ADD CONSTRAINT gensas_job_pkey PRIMARY KEY (job_id);


--
-- Name: gensas_job_resource_ukey_gensas_job_resource_resource_id_id_key; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY gensas_job_resource
    ADD CONSTRAINT gensas_job_resource_ukey_gensas_job_resource_resource_id_id_key UNIQUE (resource_id, job_id);


--
-- Name: gensas_job_stats_job_id_type_key; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY gensas_job_stats
    ADD CONSTRAINT gensas_job_stats_job_id_type_key UNIQUE (job_id, type);


--
-- Name: gensas_library_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY gensas_library
    ADD CONSTRAINT gensas_library_pkey PRIMARY KEY (library_id);


--
-- Name: gensas_library_type_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY gensas_library_type
    ADD CONSTRAINT gensas_library_type_pkey PRIMARY KEY (library_type_id);


--
-- Name: gensas_library_type_ukey_gensas_library_type_key; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY gensas_library_type
    ADD CONSTRAINT gensas_library_type_ukey_gensas_library_type_key UNIQUE (type);


--
-- Name: gensas_library_ukey_gensas_library_label_key; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY gensas_library
    ADD CONSTRAINT gensas_library_ukey_gensas_library_label_key UNIQUE (name);


--
-- Name: gensas_param_group_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY gensas_param_group
    ADD CONSTRAINT gensas_param_group_pkey PRIMARY KEY (param_group_id);


--
-- Name: gensas_param_group_ukey_param_group_name_tool_id_key; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY gensas_param_group
    ADD CONSTRAINT gensas_param_group_ukey_param_group_name_tool_id_key UNIQUE (name, tool_id);


--
-- Name: gensas_project_type_ukey_gensas_project_type_project_type_key; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY gensas_project_type
    ADD CONSTRAINT gensas_project_type_ukey_gensas_project_type_project_type_key UNIQUE (type);


--
-- Name: gensas_publish_ukey_gensas_publish_job_id_version_key; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY gensas_publish
    ADD CONSTRAINT gensas_publish_ukey_gensas_publish_job_id_version_key UNIQUE (job_id, version);


--
-- Name: gensas_resource_library_ukey_gensas_resource_library_resource_i; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY gensas_resource_library
    ADD CONSTRAINT gensas_resource_library_ukey_gensas_resource_library_resource_i UNIQUE (resource_id, library_id);


--
-- Name: gensas_resource_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY gensas_resource
    ADD CONSTRAINT gensas_resource_pkey PRIMARY KEY (resource_id);


--
-- Name: gensas_resource_submit_ukey_gensas_resource_library_resource_id; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY gensas_resource_submit
    ADD CONSTRAINT gensas_resource_submit_ukey_gensas_resource_library_resource_id UNIQUE (resource_id, hostname);


--
-- Name: gensas_resource_tool_ukey_gensas_resource_tool_resource_id_tool; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY gensas_resource_tool
    ADD CONSTRAINT gensas_resource_tool_ukey_gensas_resource_tool_resource_id_tool UNIQUE (resource_id, tool_id);


--
-- Name: gensas_resource_ukey_gensas_resource_name_key; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY gensas_resource
    ADD CONSTRAINT gensas_resource_ukey_gensas_resource_name_key UNIQUE (name);


--
-- Name: gensas_resource_ukey_gensas_resource_rank_key; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY gensas_resource
    ADD CONSTRAINT gensas_resource_ukey_gensas_resource_rank_key UNIQUE (rank);


--
-- Name: gensas_seq_group_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY gensas_seq_group
    ADD CONSTRAINT gensas_seq_group_pkey PRIMARY KEY (seq_group_id);


--
-- Name: gensas_seq_group_ukey_gensas_seq_group_user_id_name_key; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY gensas_seq_group
    ADD CONSTRAINT gensas_seq_group_ukey_gensas_seq_group_user_id_name_key UNIQUE (user_id, name);


--
-- Name: gensas_seq_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY gensas_seq
    ADD CONSTRAINT gensas_seq_pkey PRIMARY KEY (seq_id);


--
-- Name: gensas_seq_stats_job_id_seq_id_type_key; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY gensas_seq_stats
    ADD CONSTRAINT gensas_seq_stats_job_id_seq_id_type_key UNIQUE (job_id, seq_id, type);


--
-- Name: gensas_task_files_ukey_gensas_task_files_task_id_file_id_key; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY gensas_task_files
    ADD CONSTRAINT gensas_task_files_ukey_gensas_task_files_task_id_file_id_key UNIQUE (task_id, file_id);


--
-- Name: gensas_task_job_ukey_gensas_task_job_task_id_job_id_key; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY gensas_task_job
    ADD CONSTRAINT gensas_task_job_ukey_gensas_task_job_task_id_job_id_key UNIQUE (task_id, job_id);


--
-- Name: gensas_task_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY gensas_task
    ADD CONSTRAINT gensas_task_pkey PRIMARY KEY (task_id);


--
-- Name: gensas_task_seq_group_ukey_gensas_task_seq_group_task_id_seq_gr; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY gensas_task_seq_group
    ADD CONSTRAINT gensas_task_seq_group_ukey_gensas_task_seq_group_task_id_seq_gr UNIQUE (task_id, seq_group_id);


--
-- Name: gensas_task_user_ukey_gensas_task_user_task_id_user_id_key; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY gensas_task_user
    ADD CONSTRAINT gensas_task_user_ukey_gensas_task_user_task_id_user_id_key UNIQUE (task_id, user_id);


--
-- Name: gensas_tool_param_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY gensas_tool_param
    ADD CONSTRAINT gensas_tool_param_pkey PRIMARY KEY (tool_param_id);


--
-- Name: gensas_tool_param_type_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY gensas_tool_param_type
    ADD CONSTRAINT gensas_tool_param_type_pkey PRIMARY KEY (tool_param_type_id);


--
-- Name: gensas_tool_param_type_ukey_gensas_tool_param_type_key; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY gensas_tool_param_type
    ADD CONSTRAINT gensas_tool_param_type_ukey_gensas_tool_param_type_key UNIQUE (type);


--
-- Name: gensas_tool_param_ukey_gensas_tool_param_param_elem_id_tool_id_; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY gensas_tool_param
    ADD CONSTRAINT gensas_tool_param_ukey_gensas_tool_param_param_elem_id_tool_id_ UNIQUE (param_elem_id, tool_id);


--
-- Name: gensas_tool_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY gensas_tool
    ADD CONSTRAINT gensas_tool_pkey PRIMARY KEY (tool_id);


--
-- Name: gensas_tool_type_ukey_gensas_tool_type_tid_ptid_key; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY gensas_tool_type
    ADD CONSTRAINT gensas_tool_type_ukey_gensas_tool_type_tid_ptid_key UNIQUE (tool_id, project_type_id);


--
-- Name: gensas_usage_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY gensas_usage
    ADD CONSTRAINT gensas_usage_pkey PRIMARY KEY (user_id);


--
-- Name: gensas_user_tool_ukey_gensas_user_tool_user_id_tool_id_key; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY gensas_user_tool
    ADD CONSTRAINT gensas_user_tool_ukey_gensas_user_tool_user_id_tool_id_key UNIQUE (user_id, tool_id);


--
-- Name: gensas_userprop_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY gensas_userprop
    ADD CONSTRAINT gensas_userprop_pkey PRIMARY KEY (user_id);


--
-- Name: history_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY history
    ADD CONSTRAINT history_pkey PRIMARY KEY (uid, nid);


--
-- Name: image_effects_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY image_effects
    ADD CONSTRAINT image_effects_pkey PRIMARY KEY (ieid);


--
-- Name: image_styles_name_key; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY image_styles
    ADD CONSTRAINT image_styles_name_key UNIQUE (name);


--
-- Name: image_styles_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY image_styles
    ADD CONSTRAINT image_styles_pkey PRIMARY KEY (isid);


--
-- Name: masquerade_users_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY masquerade_users
    ADD CONSTRAINT masquerade_users_pkey PRIMARY KEY (uid_from, uid_to);


--
-- Name: mcl_data_valid_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY mcl_data_valid
    ADD CONSTRAINT mcl_data_valid_pkey PRIMARY KEY (data_valid_id);


--
-- Name: mcl_data_valid_type_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY mcl_data_valid_type
    ADD CONSTRAINT mcl_data_valid_type_pkey PRIMARY KEY (data_valid_type_id);


--
-- Name: mcl_data_valid_type_ukey_mcl_data_valid_type_type_key; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY mcl_data_valid_type
    ADD CONSTRAINT mcl_data_valid_type_ukey_mcl_data_valid_type_type_key UNIQUE (type);


--
-- Name: mcl_data_valid_ukey_mcl_data_valid_data_valid_type_id_name_key; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY mcl_data_valid
    ADD CONSTRAINT mcl_data_valid_ukey_mcl_data_valid_data_valid_type_id_name_key UNIQUE (data_valid_type_id, name);


--
-- Name: mcl_file_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY mcl_file
    ADD CONSTRAINT mcl_file_pkey PRIMARY KEY (file_id);


--
-- Name: mcl_job_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY mcl_job
    ADD CONSTRAINT mcl_job_pkey PRIMARY KEY (job_id);


--
-- Name: mcl_template_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY mcl_template
    ADD CONSTRAINT mcl_template_pkey PRIMARY KEY (template_id);


--
-- Name: mcl_template_type_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY mcl_template_type
    ADD CONSTRAINT mcl_template_type_pkey PRIMARY KEY (template_type_id);


--
-- Name: mcl_template_type_ukey_mcl_template_type_key; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY mcl_template_type
    ADD CONSTRAINT mcl_template_type_ukey_mcl_template_type_key UNIQUE (type);


--
-- Name: mcl_template_ukey_mcl_template_template_key; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY mcl_template
    ADD CONSTRAINT mcl_template_ukey_mcl_template_template_key UNIQUE (template);


--
-- Name: mcl_user_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY mcl_user
    ADD CONSTRAINT mcl_user_pkey PRIMARY KEY (user_id);


--
-- Name: mcl_var_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY mcl_var
    ADD CONSTRAINT mcl_var_pkey PRIMARY KEY (var_id);


--
-- Name: mcl_var_ukey_mcl_var_name_key; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY mcl_var
    ADD CONSTRAINT mcl_var_ukey_mcl_var_name_key UNIQUE (name);


--
-- Name: mdlu_cron_job_name_key; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY mdlu_cron_job
    ADD CONSTRAINT mdlu_cron_job_name_key UNIQUE (name);


--
-- Name: mdlu_cron_job_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY mdlu_cron_job
    ADD CONSTRAINT mdlu_cron_job_pkey PRIMARY KEY (cron_job_id);


--
-- Name: mdlu_database_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY mdlu_database
    ADD CONSTRAINT mdlu_database_pkey PRIMARY KEY (database_id);


--
-- Name: mdlu_database_type_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY mdlu_database_type
    ADD CONSTRAINT mdlu_database_type_pkey PRIMARY KEY (database_type_id);


--
-- Name: mdlu_log_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY mdlu_log
    ADD CONSTRAINT mdlu_log_pkey PRIMARY KEY (log_id);


--
-- Name: menu_custom_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY menu_custom
    ADD CONSTRAINT menu_custom_pkey PRIMARY KEY (menu_name);


--
-- Name: menu_links_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY menu_links
    ADD CONSTRAINT menu_links_pkey PRIMARY KEY (mlid);


--
-- Name: menu_router_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY menu_router
    ADD CONSTRAINT menu_router_pkey PRIMARY KEY (path);


--
-- Name: node_access_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY node_access
    ADD CONSTRAINT node_access_pkey PRIMARY KEY (nid, gid, realm);


--
-- Name: node_comment_statistics_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY node_comment_statistics
    ADD CONSTRAINT node_comment_statistics_pkey PRIMARY KEY (nid);


--
-- Name: node_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY node
    ADD CONSTRAINT node_pkey PRIMARY KEY (nid);


--
-- Name: node_revision_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY node_revision
    ADD CONSTRAINT node_revision_pkey PRIMARY KEY (vid);


--
-- Name: node_type_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY node_type
    ADD CONSTRAINT node_type_pkey PRIMARY KEY (type);


--
-- Name: node_vid_key; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY node
    ADD CONSTRAINT node_vid_key UNIQUE (vid);


--
-- Name: queue_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY queue
    ADD CONSTRAINT queue_pkey PRIMARY KEY (item_id);


--
-- Name: rdf_mapping_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY rdf_mapping
    ADD CONSTRAINT rdf_mapping_pkey PRIMARY KEY (type, bundle);


--
-- Name: registry_file_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY registry_file
    ADD CONSTRAINT registry_file_pkey PRIMARY KEY (filename);


--
-- Name: registry_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY registry
    ADD CONSTRAINT registry_pkey PRIMARY KEY (name, type);


--
-- Name: role_name_key; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY role
    ADD CONSTRAINT role_name_key UNIQUE (name);


--
-- Name: role_permission_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY role_permission
    ADD CONSTRAINT role_permission_pkey PRIMARY KEY (rid, permission);


--
-- Name: role_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY role
    ADD CONSTRAINT role_pkey PRIMARY KEY (rid);


--
-- Name: search_dataset_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY search_dataset
    ADD CONSTRAINT search_dataset_pkey PRIMARY KEY (sid, type);


--
-- Name: search_index_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY search_index
    ADD CONSTRAINT search_index_pkey PRIMARY KEY (word, sid, type);


--
-- Name: search_node_links_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY search_node_links
    ADD CONSTRAINT search_node_links_pkey PRIMARY KEY (sid, type, nid);


--
-- Name: search_total_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY search_total
    ADD CONSTRAINT search_total_pkey PRIMARY KEY (word);


--
-- Name: semaphore_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY semaphore
    ADD CONSTRAINT semaphore_pkey PRIMARY KEY (name);


--
-- Name: sequences_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY sequences
    ADD CONSTRAINT sequences_pkey PRIMARY KEY (value);


--
-- Name: sessions_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY sessions
    ADD CONSTRAINT sessions_pkey PRIMARY KEY (sid, ssid);


--
-- Name: shortcut_set_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY shortcut_set
    ADD CONSTRAINT shortcut_set_pkey PRIMARY KEY (set_name);


--
-- Name: shortcut_set_users_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY shortcut_set_users
    ADD CONSTRAINT shortcut_set_users_pkey PRIMARY KEY (uid);


--
-- Name: system_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY system
    ADD CONSTRAINT system_pkey PRIMARY KEY (filename);


--
-- Name: taxonomy_term_data_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY taxonomy_term_data
    ADD CONSTRAINT taxonomy_term_data_pkey PRIMARY KEY (tid);


--
-- Name: taxonomy_term_hierarchy_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY taxonomy_term_hierarchy
    ADD CONSTRAINT taxonomy_term_hierarchy_pkey PRIMARY KEY (tid, parent);


--
-- Name: taxonomy_vocabulary_machine_name_key; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY taxonomy_vocabulary
    ADD CONSTRAINT taxonomy_vocabulary_machine_name_key UNIQUE (machine_name);


--
-- Name: taxonomy_vocabulary_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY taxonomy_vocabulary
    ADD CONSTRAINT taxonomy_vocabulary_pkey PRIMARY KEY (vid);


--
-- Name: tripal_custom_tables_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY tripal_custom_tables
    ADD CONSTRAINT tripal_custom_tables_pkey PRIMARY KEY (table_id);


--
-- Name: tripal_cv_defaults_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY tripal_cv_defaults
    ADD CONSTRAINT tripal_cv_defaults_pkey PRIMARY KEY (cv_default_id);


--
-- Name: tripal_cv_defaults_tripal_cv_defaults_unq1_key; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY tripal_cv_defaults
    ADD CONSTRAINT tripal_cv_defaults_tripal_cv_defaults_unq1_key UNIQUE (table_name, field_name, cv_id);


--
-- Name: tripal_cv_obo_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY tripal_cv_obo
    ADD CONSTRAINT tripal_cv_obo_pkey PRIMARY KEY (obo_id);


--
-- Name: tripal_jobs_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY tripal_jobs
    ADD CONSTRAINT tripal_jobs_pkey PRIMARY KEY (job_id);


--
-- Name: tripal_mviews_mv_name_key; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY tripal_mviews
    ADD CONSTRAINT tripal_mviews_mv_name_key UNIQUE (name);


--
-- Name: tripal_mviews_mv_table_key; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY tripal_mviews
    ADD CONSTRAINT tripal_mviews_mv_table_key UNIQUE (mv_table);


--
-- Name: tripal_mviews_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY tripal_mviews
    ADD CONSTRAINT tripal_mviews_pkey PRIMARY KEY (mview_id);


--
-- Name: tripal_node_variables_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY tripal_node_variables
    ADD CONSTRAINT tripal_node_variables_pkey PRIMARY KEY (node_variable_id);


--
-- Name: tripal_node_variables_tripal_node_variables_c1_key; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY tripal_node_variables
    ADD CONSTRAINT tripal_node_variables_tripal_node_variables_c1_key UNIQUE (nid, variable_id, rank);


--
-- Name: tripal_toc_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY tripal_toc
    ADD CONSTRAINT tripal_toc_pkey PRIMARY KEY (toc_item_id);


--
-- Name: tripal_toc_tripal_toc_uq1_key; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY tripal_toc
    ADD CONSTRAINT tripal_toc_tripal_toc_uq1_key UNIQUE (node_type, key, nid);


--
-- Name: tripal_token_formats_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY tripal_token_formats
    ADD CONSTRAINT tripal_token_formats_pkey PRIMARY KEY (tripal_format_id);


--
-- Name: tripal_token_formats_type_application_key; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY tripal_token_formats
    ADD CONSTRAINT tripal_token_formats_type_application_key UNIQUE (content_type, application);


--
-- Name: tripal_variables_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY tripal_variables
    ADD CONSTRAINT tripal_variables_pkey PRIMARY KEY (variable_id);


--
-- Name: tripal_variables_tripal_variables_c1_key; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY tripal_variables
    ADD CONSTRAINT tripal_variables_tripal_variables_c1_key UNIQUE (name);


--
-- Name: tripal_views_field_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY tripal_views_field
    ADD CONSTRAINT tripal_views_field_pkey PRIMARY KEY (setup_id, column_name);


--
-- Name: tripal_views_handlers_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY tripal_views_handlers
    ADD CONSTRAINT tripal_views_handlers_pkey PRIMARY KEY (handler_id);


--
-- Name: tripal_views_join_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY tripal_views_join
    ADD CONSTRAINT tripal_views_join_pkey PRIMARY KEY (view_join_id);


--
-- Name: tripal_views_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY tripal_views
    ADD CONSTRAINT tripal_views_pkey PRIMARY KEY (setup_id);


--
-- Name: url_alias_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY url_alias
    ADD CONSTRAINT url_alias_pkey PRIMARY KEY (pid);


--
-- Name: user_restrictions_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY user_restrictions
    ADD CONSTRAINT user_restrictions_pkey PRIMARY KEY (urid);


--
-- Name: users_name_key; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY users
    ADD CONSTRAINT users_name_key UNIQUE (name);


--
-- Name: users_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY users
    ADD CONSTRAINT users_pkey PRIMARY KEY (uid);


--
-- Name: users_roles_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY users_roles
    ADD CONSTRAINT users_roles_pkey PRIMARY KEY (uid, rid);


--
-- Name: variable_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY variable
    ADD CONSTRAINT variable_pkey PRIMARY KEY (name);


--
-- Name: views_display_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY views_display
    ADD CONSTRAINT views_display_pkey PRIMARY KEY (vid, id);


--
-- Name: views_view_name_key; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY views_view
    ADD CONSTRAINT views_view_name_key UNIQUE (name);


--
-- Name: views_view_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY views_view
    ADD CONSTRAINT views_view_pkey PRIMARY KEY (vid);


--
-- Name: watchdog_pkey; Type: CONSTRAINT; Schema: public; Owner: -; Tablespace: 
--

ALTER TABLE ONLY watchdog
    ADD CONSTRAINT watchdog_pkey PRIMARY KEY (wid);


--
-- Name: authmap_uid_module_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX authmap_uid_module_idx ON authmap USING btree (uid, module);


--
-- Name: batch_token_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX batch_token_idx ON batch USING btree (token);


--
-- Name: block_list_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX block_list_idx ON block USING btree (theme, status, region, weight, module);


--
-- Name: block_node_type_type_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX block_node_type_type_idx ON block_node_type USING btree (type);


--
-- Name: block_role_rid_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX block_role_rid_idx ON block_role USING btree (rid);


--
-- Name: blocked_ips_blocked_ip_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX blocked_ips_blocked_ip_idx ON blocked_ips USING btree (ip);


--
-- Name: book_bid_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX book_bid_idx ON book USING btree (bid);


--
-- Name: cache_block_expire_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX cache_block_expire_idx ON cache_block USING btree (expire);


--
-- Name: cache_bootstrap_expire_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX cache_bootstrap_expire_idx ON cache_bootstrap USING btree (expire);


--
-- Name: cache_expire_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX cache_expire_idx ON cache USING btree (expire);


--
-- Name: cache_field_expire_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX cache_field_expire_idx ON cache_field USING btree (expire);


--
-- Name: cache_filter_expire_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX cache_filter_expire_idx ON cache_filter USING btree (expire);


--
-- Name: cache_form_expire_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX cache_form_expire_idx ON cache_form USING btree (expire);


--
-- Name: cache_image_expire_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX cache_image_expire_idx ON cache_image USING btree (expire);


--
-- Name: cache_menu_expire_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX cache_menu_expire_idx ON cache_menu USING btree (expire);


--
-- Name: cache_page_expire_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX cache_page_expire_idx ON cache_page USING btree (expire);


--
-- Name: cache_path_expire_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX cache_path_expire_idx ON cache_path USING btree (expire);


--
-- Name: cache_update_expire_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX cache_update_expire_idx ON cache_update USING btree (expire);


--
-- Name: cache_views_data_expire_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX cache_views_data_expire_idx ON cache_views_data USING btree (expire);


--
-- Name: cache_views_expire_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX cache_views_expire_idx ON cache_views USING btree (expire);


--
-- Name: comment_comment_created_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX comment_comment_created_idx ON comment USING btree (created);


--
-- Name: comment_comment_nid_language_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX comment_comment_nid_language_idx ON comment USING btree (nid, language);


--
-- Name: comment_comment_num_new_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX comment_comment_num_new_idx ON comment USING btree (nid, status, created, cid, thread);


--
-- Name: comment_comment_status_pid_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX comment_comment_status_pid_idx ON comment USING btree (pid, status);


--
-- Name: comment_comment_uid_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX comment_comment_uid_idx ON comment USING btree (uid);


--
-- Name: contact_list_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX contact_list_idx ON contact USING btree (weight, category);


--
-- Name: ctools_object_cache_updated_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX ctools_object_cache_updated_idx ON ctools_object_cache USING btree (updated);


--
-- Name: date_format_type_title_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX date_format_type_title_idx ON date_format_type USING btree (title);


--
-- Name: field_config_active_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX field_config_active_idx ON field_config USING btree (active);


--
-- Name: field_config_deleted_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX field_config_deleted_idx ON field_config USING btree (deleted);


--
-- Name: field_config_field_name_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX field_config_field_name_idx ON field_config USING btree (field_name);


--
-- Name: field_config_instance_deleted_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX field_config_instance_deleted_idx ON field_config_instance USING btree (deleted);


--
-- Name: field_config_instance_field_name_bundle_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX field_config_instance_field_name_bundle_idx ON field_config_instance USING btree (field_name, entity_type, bundle);


--
-- Name: field_config_module_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX field_config_module_idx ON field_config USING btree (module);


--
-- Name: field_config_storage_active_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX field_config_storage_active_idx ON field_config USING btree (storage_active);


--
-- Name: field_config_storage_module_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX field_config_storage_module_idx ON field_config USING btree (storage_module);


--
-- Name: field_config_storage_type_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX field_config_storage_type_idx ON field_config USING btree (storage_type);


--
-- Name: field_config_type_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX field_config_type_idx ON field_config USING btree (type);


--
-- Name: field_data_body_body_format_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX field_data_body_body_format_idx ON field_data_body USING btree (body_format);


--
-- Name: field_data_body_bundle_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX field_data_body_bundle_idx ON field_data_body USING btree (bundle);


--
-- Name: field_data_body_deleted_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX field_data_body_deleted_idx ON field_data_body USING btree (deleted);


--
-- Name: field_data_body_entity_id_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX field_data_body_entity_id_idx ON field_data_body USING btree (entity_id);


--
-- Name: field_data_body_entity_type_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX field_data_body_entity_type_idx ON field_data_body USING btree (entity_type);


--
-- Name: field_data_body_language_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX field_data_body_language_idx ON field_data_body USING btree (language);


--
-- Name: field_data_body_revision_id_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX field_data_body_revision_id_idx ON field_data_body USING btree (revision_id);


--
-- Name: field_data_comment_body_bundle_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX field_data_comment_body_bundle_idx ON field_data_comment_body USING btree (bundle);


--
-- Name: field_data_comment_body_comment_body_format_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX field_data_comment_body_comment_body_format_idx ON field_data_comment_body USING btree (comment_body_format);


--
-- Name: field_data_comment_body_deleted_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX field_data_comment_body_deleted_idx ON field_data_comment_body USING btree (deleted);


--
-- Name: field_data_comment_body_entity_id_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX field_data_comment_body_entity_id_idx ON field_data_comment_body USING btree (entity_id);


--
-- Name: field_data_comment_body_entity_type_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX field_data_comment_body_entity_type_idx ON field_data_comment_body USING btree (entity_type);


--
-- Name: field_data_comment_body_language_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX field_data_comment_body_language_idx ON field_data_comment_body USING btree (language);


--
-- Name: field_data_comment_body_revision_id_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX field_data_comment_body_revision_id_idx ON field_data_comment_body USING btree (revision_id);


--
-- Name: field_data_field_first_anme_bundle_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX field_data_field_first_anme_bundle_idx ON field_data_field_first_anme USING btree (bundle);


--
-- Name: field_data_field_first_anme_deleted_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX field_data_field_first_anme_deleted_idx ON field_data_field_first_anme USING btree (deleted);


--
-- Name: field_data_field_first_anme_entity_id_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX field_data_field_first_anme_entity_id_idx ON field_data_field_first_anme USING btree (entity_id);


--
-- Name: field_data_field_first_anme_entity_type_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX field_data_field_first_anme_entity_type_idx ON field_data_field_first_anme USING btree (entity_type);


--
-- Name: field_data_field_first_anme_field_first_anme_format_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX field_data_field_first_anme_field_first_anme_format_idx ON field_data_field_first_anme USING btree (field_first_anme_format);


--
-- Name: field_data_field_first_anme_language_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX field_data_field_first_anme_language_idx ON field_data_field_first_anme USING btree (language);


--
-- Name: field_data_field_first_anme_revision_id_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX field_data_field_first_anme_revision_id_idx ON field_data_field_first_anme USING btree (revision_id);


--
-- Name: field_data_field_image_bundle_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX field_data_field_image_bundle_idx ON field_data_field_image USING btree (bundle);


--
-- Name: field_data_field_image_deleted_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX field_data_field_image_deleted_idx ON field_data_field_image USING btree (deleted);


--
-- Name: field_data_field_image_entity_id_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX field_data_field_image_entity_id_idx ON field_data_field_image USING btree (entity_id);


--
-- Name: field_data_field_image_entity_type_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX field_data_field_image_entity_type_idx ON field_data_field_image USING btree (entity_type);


--
-- Name: field_data_field_image_field_image_fid_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX field_data_field_image_field_image_fid_idx ON field_data_field_image USING btree (field_image_fid);


--
-- Name: field_data_field_image_language_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX field_data_field_image_language_idx ON field_data_field_image USING btree (language);


--
-- Name: field_data_field_image_revision_id_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX field_data_field_image_revision_id_idx ON field_data_field_image USING btree (revision_id);


--
-- Name: field_data_field_tags_bundle_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX field_data_field_tags_bundle_idx ON field_data_field_tags USING btree (bundle);


--
-- Name: field_data_field_tags_deleted_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX field_data_field_tags_deleted_idx ON field_data_field_tags USING btree (deleted);


--
-- Name: field_data_field_tags_entity_id_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX field_data_field_tags_entity_id_idx ON field_data_field_tags USING btree (entity_id);


--
-- Name: field_data_field_tags_entity_type_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX field_data_field_tags_entity_type_idx ON field_data_field_tags USING btree (entity_type);


--
-- Name: field_data_field_tags_field_tags_tid_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX field_data_field_tags_field_tags_tid_idx ON field_data_field_tags USING btree (field_tags_tid);


--
-- Name: field_data_field_tags_language_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX field_data_field_tags_language_idx ON field_data_field_tags USING btree (language);


--
-- Name: field_data_field_tags_revision_id_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX field_data_field_tags_revision_id_idx ON field_data_field_tags USING btree (revision_id);


--
-- Name: field_revision_body_body_format_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX field_revision_body_body_format_idx ON field_revision_body USING btree (body_format);


--
-- Name: field_revision_body_bundle_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX field_revision_body_bundle_idx ON field_revision_body USING btree (bundle);


--
-- Name: field_revision_body_deleted_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX field_revision_body_deleted_idx ON field_revision_body USING btree (deleted);


--
-- Name: field_revision_body_entity_id_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX field_revision_body_entity_id_idx ON field_revision_body USING btree (entity_id);


--
-- Name: field_revision_body_entity_type_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX field_revision_body_entity_type_idx ON field_revision_body USING btree (entity_type);


--
-- Name: field_revision_body_language_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX field_revision_body_language_idx ON field_revision_body USING btree (language);


--
-- Name: field_revision_body_revision_id_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX field_revision_body_revision_id_idx ON field_revision_body USING btree (revision_id);


--
-- Name: field_revision_comment_body_bundle_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX field_revision_comment_body_bundle_idx ON field_revision_comment_body USING btree (bundle);


--
-- Name: field_revision_comment_body_comment_body_format_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX field_revision_comment_body_comment_body_format_idx ON field_revision_comment_body USING btree (comment_body_format);


--
-- Name: field_revision_comment_body_deleted_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX field_revision_comment_body_deleted_idx ON field_revision_comment_body USING btree (deleted);


--
-- Name: field_revision_comment_body_entity_id_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX field_revision_comment_body_entity_id_idx ON field_revision_comment_body USING btree (entity_id);


--
-- Name: field_revision_comment_body_entity_type_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX field_revision_comment_body_entity_type_idx ON field_revision_comment_body USING btree (entity_type);


--
-- Name: field_revision_comment_body_language_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX field_revision_comment_body_language_idx ON field_revision_comment_body USING btree (language);


--
-- Name: field_revision_comment_body_revision_id_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX field_revision_comment_body_revision_id_idx ON field_revision_comment_body USING btree (revision_id);


--
-- Name: field_revision_field_first_anme_bundle_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX field_revision_field_first_anme_bundle_idx ON field_revision_field_first_anme USING btree (bundle);


--
-- Name: field_revision_field_first_anme_deleted_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX field_revision_field_first_anme_deleted_idx ON field_revision_field_first_anme USING btree (deleted);


--
-- Name: field_revision_field_first_anme_entity_id_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX field_revision_field_first_anme_entity_id_idx ON field_revision_field_first_anme USING btree (entity_id);


--
-- Name: field_revision_field_first_anme_entity_type_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX field_revision_field_first_anme_entity_type_idx ON field_revision_field_first_anme USING btree (entity_type);


--
-- Name: field_revision_field_first_anme_field_first_anme_format_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX field_revision_field_first_anme_field_first_anme_format_idx ON field_revision_field_first_anme USING btree (field_first_anme_format);


--
-- Name: field_revision_field_first_anme_language_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX field_revision_field_first_anme_language_idx ON field_revision_field_first_anme USING btree (language);


--
-- Name: field_revision_field_first_anme_revision_id_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX field_revision_field_first_anme_revision_id_idx ON field_revision_field_first_anme USING btree (revision_id);


--
-- Name: field_revision_field_image_bundle_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX field_revision_field_image_bundle_idx ON field_revision_field_image USING btree (bundle);


--
-- Name: field_revision_field_image_deleted_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX field_revision_field_image_deleted_idx ON field_revision_field_image USING btree (deleted);


--
-- Name: field_revision_field_image_entity_id_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX field_revision_field_image_entity_id_idx ON field_revision_field_image USING btree (entity_id);


--
-- Name: field_revision_field_image_entity_type_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX field_revision_field_image_entity_type_idx ON field_revision_field_image USING btree (entity_type);


--
-- Name: field_revision_field_image_field_image_fid_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX field_revision_field_image_field_image_fid_idx ON field_revision_field_image USING btree (field_image_fid);


--
-- Name: field_revision_field_image_language_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX field_revision_field_image_language_idx ON field_revision_field_image USING btree (language);


--
-- Name: field_revision_field_image_revision_id_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX field_revision_field_image_revision_id_idx ON field_revision_field_image USING btree (revision_id);


--
-- Name: field_revision_field_tags_bundle_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX field_revision_field_tags_bundle_idx ON field_revision_field_tags USING btree (bundle);


--
-- Name: field_revision_field_tags_deleted_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX field_revision_field_tags_deleted_idx ON field_revision_field_tags USING btree (deleted);


--
-- Name: field_revision_field_tags_entity_id_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX field_revision_field_tags_entity_id_idx ON field_revision_field_tags USING btree (entity_id);


--
-- Name: field_revision_field_tags_entity_type_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX field_revision_field_tags_entity_type_idx ON field_revision_field_tags USING btree (entity_type);


--
-- Name: field_revision_field_tags_field_tags_tid_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX field_revision_field_tags_field_tags_tid_idx ON field_revision_field_tags USING btree (field_tags_tid);


--
-- Name: field_revision_field_tags_language_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX field_revision_field_tags_language_idx ON field_revision_field_tags USING btree (language);


--
-- Name: field_revision_field_tags_revision_id_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX field_revision_field_tags_revision_id_idx ON field_revision_field_tags USING btree (revision_id);


--
-- Name: file_managed_status_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX file_managed_status_idx ON file_managed USING btree (status);


--
-- Name: file_managed_timestamp_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX file_managed_timestamp_idx ON file_managed USING btree ("timestamp");


--
-- Name: file_managed_uid_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX file_managed_uid_idx ON file_managed USING btree (uid);


--
-- Name: file_usage_fid_count_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX file_usage_fid_count_idx ON file_usage USING btree (fid, count);


--
-- Name: file_usage_fid_module_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX file_usage_fid_module_idx ON file_usage USING btree (fid, module);


--
-- Name: file_usage_type_id_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX file_usage_type_id_idx ON file_usage USING btree (type, id);


--
-- Name: filter_format_status_weight_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX filter_format_status_weight_idx ON filter_format USING btree (status, weight);


--
-- Name: filter_list_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX filter_list_idx ON filter USING btree (weight, module, name);


--
-- Name: flood_allow_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX flood_allow_idx ON flood USING btree (event, identifier, "timestamp");


--
-- Name: flood_purge_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX flood_purge_idx ON flood USING btree (expiration);


--
-- Name: gensas_expire_idx1_gensas_expire_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX gensas_expire_idx1_gensas_expire_idx ON gensas_expire USING btree (task_id);


--
-- Name: gensas_gff3_landmark_job_id_id_key; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX gensas_gff3_landmark_job_id_id_key ON gensas_gff3 USING btree (job_id, id, landmark);


--
-- Name: gensas_job_files_idx_gensas_job_files_file_id_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX gensas_job_files_idx_gensas_job_files_file_id_idx ON gensas_job_files USING btree (file_id);


--
-- Name: gensas_job_resource_idx_gensas_job_resource_job_id_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX gensas_job_resource_idx_gensas_job_resource_job_id_idx ON gensas_job_resource USING btree (job_id);


--
-- Name: gensas_job_resource_idx_gensas_job_resource_resource_id_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX gensas_job_resource_idx_gensas_job_resource_resource_id_idx ON gensas_job_resource USING btree (resource_id);


--
-- Name: gensas_job_stats_idx_gensas_job_stats_job_id_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX gensas_job_stats_idx_gensas_job_stats_job_id_idx ON gensas_job_stats USING btree (job_id);


--
-- Name: gensas_project_type_idx_gensas_project_type_project_type_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX gensas_project_type_idx_gensas_project_type_project_type_idx ON gensas_project_type USING btree (type);


--
-- Name: gensas_publish_idx_gensas_publish_job_id_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX gensas_publish_idx_gensas_publish_job_id_idx ON gensas_publish USING btree (job_id);


--
-- Name: gensas_resource_idx_gensas_resource_name_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX gensas_resource_idx_gensas_resource_name_idx ON gensas_resource USING btree (name);


--
-- Name: gensas_resource_library_idx_gensas_resource_library_library_id_; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX gensas_resource_library_idx_gensas_resource_library_library_id_ ON gensas_resource_library USING btree (library_id);


--
-- Name: gensas_resource_library_idx_gensas_resource_library_resource_id; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX gensas_resource_library_idx_gensas_resource_library_resource_id ON gensas_resource_library USING btree (resource_id);


--
-- Name: gensas_resource_submit_idx_gensas_resource_submit_hostname_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX gensas_resource_submit_idx_gensas_resource_submit_hostname_idx ON gensas_resource_submit USING btree (hostname);


--
-- Name: gensas_resource_submit_idx_gensas_resource_submit_resource_id_i; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX gensas_resource_submit_idx_gensas_resource_submit_resource_id_i ON gensas_resource_submit USING btree (resource_id);


--
-- Name: gensas_resource_tool_idx_gensas_resource_tool_resource_id_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX gensas_resource_tool_idx_gensas_resource_tool_resource_id_idx ON gensas_resource_tool USING btree (resource_id);


--
-- Name: gensas_resource_tool_idx_gensas_resource_tool_tool_id_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX gensas_resource_tool_idx_gensas_resource_tool_tool_id_idx ON gensas_resource_tool USING btree (tool_id);


--
-- Name: gensas_seq_idx_gensas_seq_name_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX gensas_seq_idx_gensas_seq_name_idx ON gensas_seq USING btree (name);


--
-- Name: gensas_seq_idx_gensas_seq_seq_group_id_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX gensas_seq_idx_gensas_seq_seq_group_id_idx ON gensas_seq USING btree (seq_group_id);


--
-- Name: gensas_seq_stats_idx_gensas_job_stats_job_id_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX gensas_seq_stats_idx_gensas_job_stats_job_id_idx ON gensas_seq_stats USING btree (job_id);


--
-- Name: gensas_seq_stats_idx_gensas_job_stats_seq_id_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX gensas_seq_stats_idx_gensas_job_stats_seq_id_idx ON gensas_seq_stats USING btree (seq_id);


--
-- Name: gensas_task_files_idx_gensas_task_files_file_id_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX gensas_task_files_idx_gensas_task_files_file_id_idx ON gensas_task_files USING btree (file_id);


--
-- Name: gensas_task_files_idx_gensas_task_files_task_id_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX gensas_task_files_idx_gensas_task_files_task_id_idx ON gensas_task_files USING btree (task_id);


--
-- Name: gensas_task_job_idx_gensas_task_job_job_id_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX gensas_task_job_idx_gensas_task_job_job_id_idx ON gensas_task_job USING btree (job_id);


--
-- Name: gensas_task_job_idx_gensas_task_job_task_id_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX gensas_task_job_idx_gensas_task_job_task_id_idx ON gensas_task_job USING btree (task_id);


--
-- Name: gensas_task_seq_group_idx_gensas_task_seq_group_seq_id_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX gensas_task_seq_group_idx_gensas_task_seq_group_seq_id_idx ON gensas_task_seq_group USING btree (seq_group_id);


--
-- Name: gensas_task_seq_group_idx_gensas_task_seq_group_task_id_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX gensas_task_seq_group_idx_gensas_task_seq_group_task_id_idx ON gensas_task_seq_group USING btree (task_id);


--
-- Name: gensas_tool_type_idx_gensas_tool_type_project_type_id_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX gensas_tool_type_idx_gensas_tool_type_project_type_id_idx ON gensas_tool_type USING btree (project_type_id);


--
-- Name: gensas_tool_type_idx_gensas_tool_type_tool_id_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX gensas_tool_type_idx_gensas_tool_type_tool_id_idx ON gensas_tool_type USING btree (tool_id);


--
-- Name: history_nid_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX history_nid_idx ON history USING btree (nid);


--
-- Name: image_effects_isid_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX image_effects_isid_idx ON image_effects USING btree (isid);


--
-- Name: image_effects_weight_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX image_effects_weight_idx ON image_effects USING btree (weight);


--
-- Name: masquerade_sid_2_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX masquerade_sid_2_idx ON masquerade USING btree (sid, uid_as);


--
-- Name: masquerade_sid_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX masquerade_sid_idx ON masquerade USING btree (sid, uid_from);


--
-- Name: menu_links_menu_parents_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX menu_links_menu_parents_idx ON menu_links USING btree (menu_name, p1, p2, p3, p4, p5, p6, p7, p8, p9);


--
-- Name: menu_links_menu_plid_expand_child_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX menu_links_menu_plid_expand_child_idx ON menu_links USING btree (menu_name, plid, expanded, has_children);


--
-- Name: menu_links_path_menu_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX menu_links_path_menu_idx ON menu_links USING btree (substr((link_path)::text, 1, 128), menu_name);


--
-- Name: menu_links_router_path_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX menu_links_router_path_idx ON menu_links USING btree (substr((router_path)::text, 1, 128));


--
-- Name: menu_router_fit_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX menu_router_fit_idx ON menu_router USING btree (fit);


--
-- Name: menu_router_tab_parent_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX menu_router_tab_parent_idx ON menu_router USING btree (substr((tab_parent)::text, 1, 64), weight, title);


--
-- Name: menu_router_tab_root_weight_title_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX menu_router_tab_root_weight_title_idx ON menu_router USING btree (substr((tab_root)::text, 1, 64), weight, title);


--
-- Name: node_comment_statistics_comment_count_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX node_comment_statistics_comment_count_idx ON node_comment_statistics USING btree (comment_count);


--
-- Name: node_comment_statistics_last_comment_uid_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX node_comment_statistics_last_comment_uid_idx ON node_comment_statistics USING btree (last_comment_uid);


--
-- Name: node_comment_statistics_node_comment_timestamp_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX node_comment_statistics_node_comment_timestamp_idx ON node_comment_statistics USING btree (last_comment_timestamp);


--
-- Name: node_language_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX node_language_idx ON node USING btree (language);


--
-- Name: node_node_changed_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX node_node_changed_idx ON node USING btree (changed);


--
-- Name: node_node_created_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX node_node_created_idx ON node USING btree (created);


--
-- Name: node_node_frontpage_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX node_node_frontpage_idx ON node USING btree (promote, status, sticky, created);


--
-- Name: node_node_status_type_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX node_node_status_type_idx ON node USING btree (status, type, nid);


--
-- Name: node_node_title_type_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX node_node_title_type_idx ON node USING btree (title, substr((type)::text, 1, 4));


--
-- Name: node_node_type_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX node_node_type_idx ON node USING btree (substr((type)::text, 1, 4));


--
-- Name: node_revision_nid_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX node_revision_nid_idx ON node_revision USING btree (nid);


--
-- Name: node_revision_uid_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX node_revision_uid_idx ON node_revision USING btree (uid);


--
-- Name: node_tnid_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX node_tnid_idx ON node USING btree (tnid);


--
-- Name: node_translate_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX node_translate_idx ON node USING btree (translate);


--
-- Name: node_uid_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX node_uid_idx ON node USING btree (uid);


--
-- Name: queue_expire_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX queue_expire_idx ON queue USING btree (expire);


--
-- Name: queue_name_created_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX queue_name_created_idx ON queue USING btree (name, created);


--
-- Name: registry_hook_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX registry_hook_idx ON registry USING btree (type, weight, module);


--
-- Name: role_name_weight_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX role_name_weight_idx ON role USING btree (name, weight);


--
-- Name: role_permission_permission_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX role_permission_permission_idx ON role_permission USING btree (permission);


--
-- Name: search_index_sid_type_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX search_index_sid_type_idx ON search_index USING btree (sid, type);


--
-- Name: search_node_links_nid_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX search_node_links_nid_idx ON search_node_links USING btree (nid);


--
-- Name: semaphore_expire_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX semaphore_expire_idx ON semaphore USING btree (expire);


--
-- Name: semaphore_value_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX semaphore_value_idx ON semaphore USING btree (value);


--
-- Name: sessions_ssid_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX sessions_ssid_idx ON sessions USING btree (ssid);


--
-- Name: sessions_timestamp_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX sessions_timestamp_idx ON sessions USING btree ("timestamp");


--
-- Name: sessions_uid_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX sessions_uid_idx ON sessions USING btree (uid);


--
-- Name: shortcut_set_users_set_name_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX shortcut_set_users_set_name_idx ON shortcut_set_users USING btree (set_name);


--
-- Name: system_system_list_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX system_system_list_idx ON system USING btree (status, bootstrap, type, weight, name);


--
-- Name: system_type_name_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX system_type_name_idx ON system USING btree (type, name);


--
-- Name: taxonomy_index_nid_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX taxonomy_index_nid_idx ON taxonomy_index USING btree (nid);


--
-- Name: taxonomy_index_term_node_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX taxonomy_index_term_node_idx ON taxonomy_index USING btree (tid, sticky, created);


--
-- Name: taxonomy_term_data_name_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX taxonomy_term_data_name_idx ON taxonomy_term_data USING btree (name);


--
-- Name: taxonomy_term_data_taxonomy_tree_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX taxonomy_term_data_taxonomy_tree_idx ON taxonomy_term_data USING btree (vid, weight, name);


--
-- Name: taxonomy_term_data_vid_name_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX taxonomy_term_data_vid_name_idx ON taxonomy_term_data USING btree (vid, name);


--
-- Name: taxonomy_term_hierarchy_parent_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX taxonomy_term_hierarchy_parent_idx ON taxonomy_term_hierarchy USING btree (parent);


--
-- Name: taxonomy_vocabulary_list_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX taxonomy_vocabulary_list_idx ON taxonomy_vocabulary USING btree (weight, name);


--
-- Name: tripal_custom_tables_table_id_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX tripal_custom_tables_table_id_idx ON tripal_custom_tables USING btree (table_id);


--
-- Name: tripal_cv_defaults_tripal_cv_defaults_idx1_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX tripal_cv_defaults_tripal_cv_defaults_idx1_idx ON tripal_cv_defaults USING btree (table_name, field_name);


--
-- Name: tripal_cv_obo_tripal_cv_obo_idx1_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX tripal_cv_obo_tripal_cv_obo_idx1_idx ON tripal_cv_obo USING btree (obo_id);


--
-- Name: tripal_jobs_job_id_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX tripal_jobs_job_id_idx ON tripal_jobs USING btree (job_id);


--
-- Name: tripal_jobs_job_name_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX tripal_jobs_job_name_idx ON tripal_jobs USING btree (job_name);


--
-- Name: tripal_mviews_mview_id_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX tripal_mviews_mview_id_idx ON tripal_mviews USING btree (mview_id);


--
-- Name: tripal_node_variables_tripal_node_variables_idx1_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX tripal_node_variables_tripal_node_variables_idx1_idx ON tripal_node_variables USING btree (variable_id);


--
-- Name: tripal_toc_tripal_toc_idx1_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX tripal_toc_tripal_toc_idx1_idx ON tripal_toc USING btree (node_type, key);


--
-- Name: tripal_toc_tripal_toc_idx2_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX tripal_toc_tripal_toc_idx2_idx ON tripal_toc USING btree (node_type, key, nid);


--
-- Name: tripal_variables_tripal_variable_names_idx1_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX tripal_variables_tripal_variable_names_idx1_idx ON tripal_variables USING btree (variable_id);


--
-- Name: tripal_views_priority_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX tripal_views_priority_idx ON tripal_views USING btree (table_name, priority);


--
-- Name: url_alias_alias_language_pid_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX url_alias_alias_language_pid_idx ON url_alias USING btree (alias, language, pid);


--
-- Name: url_alias_source_language_pid_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX url_alias_source_language_pid_idx ON url_alias USING btree (source, language, pid);


--
-- Name: user_restrictions_expire_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX user_restrictions_expire_idx ON user_restrictions USING btree (expire);


--
-- Name: user_restrictions_status_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX user_restrictions_status_idx ON user_restrictions USING btree (status);


--
-- Name: user_restrictions_subtype_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX user_restrictions_subtype_idx ON user_restrictions USING btree (substr((subtype)::text, 1, 32));


--
-- Name: user_restrictions_type_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX user_restrictions_type_idx ON user_restrictions USING btree (substr((type)::text, 1, 32));


--
-- Name: users_access_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX users_access_idx ON users USING btree (access);


--
-- Name: users_created_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX users_created_idx ON users USING btree (created);


--
-- Name: users_mail_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX users_mail_idx ON users USING btree (mail);


--
-- Name: users_picture_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX users_picture_idx ON users USING btree (picture);


--
-- Name: users_roles_rid_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX users_roles_rid_idx ON users_roles USING btree (rid);


--
-- Name: views_display_vid_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX views_display_vid_idx ON views_display USING btree (vid, "position");


--
-- Name: watchdog_severity_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX watchdog_severity_idx ON watchdog USING btree (severity);


--
-- Name: watchdog_type_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX watchdog_type_idx ON watchdog USING btree (type);


--
-- Name: watchdog_uid_idx; Type: INDEX; Schema: public; Owner: -; Tablespace: 
--

CREATE INDEX watchdog_uid_idx ON watchdog USING btree (uid);


--
-- Name: tripal_custom_tables_fk1; Type: FK CONSTRAINT; Schema: public; Owner: -
--

ALTER TABLE ONLY tripal_custom_tables
    ADD CONSTRAINT tripal_custom_tables_fk1 FOREIGN KEY (mview_id) REFERENCES tripal_mviews(mview_id) ON DELETE CASCADE DEFERRABLE INITIALLY DEFERRED;


--
-- Name: public; Type: ACL; Schema: -; Owner: -
--

REVOKE ALL ON SCHEMA public FROM PUBLIC;
REVOKE ALL ON SCHEMA public FROM postgres;
GRANT ALL ON SCHEMA public TO postgres;
GRANT ALL ON SCHEMA public TO PUBLIC;


--
-- PostgreSQL database dump complete
--

