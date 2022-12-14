--
-- PostgreSQL database dump
--

-- Dumped from database version 9.6.20
-- Dumped by pg_dump version 9.6.20

-- Started on 2022-12-13 16:32:10 MSK

SET statement_timeout = 0;
SET lock_timeout = 0;
SET idle_in_transaction_session_timeout = 0;
SET client_encoding = 'UTF8';
SET standard_conforming_strings = on;
SELECT pg_catalog.set_config('search_path', '', false);
SET check_function_bodies = false;
SET xmloption = content;
SET client_min_messages = warning;
SET row_security = off;

--
-- TOC entry 2156 (class 1262 OID 16384)
-- Name: devices; Type: DATABASE; Schema: -; Owner: -
--

CREATE DATABASE devices WITH TEMPLATE = template0 ENCODING = 'UTF8' LC_COLLATE = 'ru_RU.UTF-8' LC_CTYPE = 'ru_RU.UTF-8';


\connect devices

SET statement_timeout = 0;
SET lock_timeout = 0;
SET idle_in_transaction_session_timeout = 0;
SET client_encoding = 'UTF8';
SET standard_conforming_strings = on;
SELECT pg_catalog.set_config('search_path', '', false);
SET check_function_bodies = false;
SET xmloption = content;
SET client_min_messages = warning;
SET row_security = off;

--
-- TOC entry 1 (class 3079 OID 12393)
-- Name: plpgsql; Type: EXTENSION; Schema: -; Owner: -
--

CREATE EXTENSION IF NOT EXISTS plpgsql WITH SCHEMA pg_catalog;


--
-- TOC entry 2158 (class 0 OID 0)
-- Dependencies: 1
-- Name: EXTENSION plpgsql; Type: COMMENT; Schema: -; Owner: -
--

COMMENT ON EXTENSION plpgsql IS 'PL/pgSQL procedural language';


--
-- TOC entry 187 (class 1259 OID 16396)
-- Name: device_id_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE public.device_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


SET default_with_oids = false;

--
-- TOC entry 188 (class 1259 OID 16398)
-- Name: device; Type: TABLE; Schema: public; Owner: -
--

CREATE TABLE public.device (
    id integer DEFAULT nextval('public.device_id_seq'::regclass) NOT NULL,
    name character varying(128)
);


--
-- TOC entry 190 (class 1259 OID 16406)
-- Name: parameter; Type: TABLE; Schema: public; Owner: -
--

CREATE TABLE public.parameter (
    id integer NOT NULL,
    name character varying(128),
    device_id integer,
    type_id integer
);


--
-- TOC entry 189 (class 1259 OID 16404)
-- Name: parameter_id_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE public.parameter_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


--
-- TOC entry 2159 (class 0 OID 0)
-- Dependencies: 189
-- Name: parameter_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: -
--

ALTER SEQUENCE public.parameter_id_seq OWNED BY public.parameter.id;


--
-- TOC entry 186 (class 1259 OID 16387)
-- Name: type; Type: TABLE; Schema: public; Owner: -
--

CREATE TABLE public.type (
    id integer NOT NULL,
    name character varying(128)
);


--
-- TOC entry 185 (class 1259 OID 16385)
-- Name: type_id_seq; Type: SEQUENCE; Schema: public; Owner: -
--

CREATE SEQUENCE public.type_id_seq
    START WITH 1
    INCREMENT BY 1
    NO MINVALUE
    NO MAXVALUE
    CACHE 1;


--
-- TOC entry 2160 (class 0 OID 0)
-- Dependencies: 185
-- Name: type_id_seq; Type: SEQUENCE OWNED BY; Schema: public; Owner: -
--

ALTER SEQUENCE public.type_id_seq OWNED BY public.type.id;


--
-- TOC entry 2019 (class 2604 OID 16409)
-- Name: parameter id; Type: DEFAULT; Schema: public; Owner: -
--

ALTER TABLE ONLY public.parameter ALTER COLUMN id SET DEFAULT nextval('public.parameter_id_seq'::regclass);


--
-- TOC entry 2017 (class 2604 OID 16390)
-- Name: type id; Type: DEFAULT; Schema: public; Owner: -
--

ALTER TABLE ONLY public.type ALTER COLUMN id SET DEFAULT nextval('public.type_id_seq'::regclass);


--
-- TOC entry 2148 (class 0 OID 16398)
-- Dependencies: 188
-- Data for Name: device; Type: TABLE DATA; Schema: public; Owner: -
--

INSERT INTO public.device (id, name) VALUES (1, 'Устройство 1');
INSERT INTO public.device (id, name) VALUES (2, 'Устройство X');


--
-- TOC entry 2161 (class 0 OID 0)
-- Dependencies: 187
-- Name: device_id_seq; Type: SEQUENCE SET; Schema: public; Owner: -
--

SELECT pg_catalog.setval('public.device_id_seq', 2, true);


--
-- TOC entry 2150 (class 0 OID 16406)
-- Dependencies: 190
-- Data for Name: parameter; Type: TABLE DATA; Schema: public; Owner: -
--

INSERT INTO public.parameter (id, name, device_id, type_id) VALUES (1, 'Параметр1', 1, 1);
INSERT INTO public.parameter (id, name, device_id, type_id) VALUES (2, 'Параметр2', 1, 5);
INSERT INTO public.parameter (id, name, device_id, type_id) VALUES (3, 'Параметр3', 1, 2);
INSERT INTO public.parameter (id, name, device_id, type_id) VALUES (4, 'ПараметрN', 1, 4);
INSERT INTO public.parameter (id, name, device_id, type_id) VALUES (5, 'Параметр1', 2, 3);
INSERT INTO public.parameter (id, name, device_id, type_id) VALUES (6, 'ПараметрМ', 2, 3);


--
-- TOC entry 2162 (class 0 OID 0)
-- Dependencies: 189
-- Name: parameter_id_seq; Type: SEQUENCE SET; Schema: public; Owner: -
--

SELECT pg_catalog.setval('public.parameter_id_seq', 6, true);


--
-- TOC entry 2146 (class 0 OID 16387)
-- Dependencies: 186
-- Data for Name: type; Type: TABLE DATA; Schema: public; Owner: -
--

INSERT INTO public.type (id, name) VALUES (1, 'Частота');
INSERT INTO public.type (id, name) VALUES (2, 'Период');
INSERT INTO public.type (id, name) VALUES (3, 'Потребляемый ток');
INSERT INTO public.type (id, name) VALUES (4, 'Рабочее напряжение');
INSERT INTO public.type (id, name) VALUES (5, 'Мощность');


--
-- TOC entry 2163 (class 0 OID 0)
-- Dependencies: 185
-- Name: type_id_seq; Type: SEQUENCE SET; Schema: public; Owner: -
--

SELECT pg_catalog.setval('public.type_id_seq', 5, true);


--
-- TOC entry 2023 (class 2606 OID 16403)
-- Name: device device_pk; Type: CONSTRAINT; Schema: public; Owner: -
--

ALTER TABLE ONLY public.device
    ADD CONSTRAINT device_pk PRIMARY KEY (id);


--
-- TOC entry 2025 (class 2606 OID 16411)
-- Name: parameter parameter_pk; Type: CONSTRAINT; Schema: public; Owner: -
--

ALTER TABLE ONLY public.parameter
    ADD CONSTRAINT parameter_pk PRIMARY KEY (id);


--
-- TOC entry 2021 (class 2606 OID 16392)
-- Name: type type_pk; Type: CONSTRAINT; Schema: public; Owner: -
--

ALTER TABLE ONLY public.type
    ADD CONSTRAINT type_pk PRIMARY KEY (id);


--
-- TOC entry 2027 (class 2606 OID 16427)
-- Name: parameter parameter_device_fk; Type: FK CONSTRAINT; Schema: public; Owner: -
--

ALTER TABLE ONLY public.parameter
    ADD CONSTRAINT parameter_device_fk FOREIGN KEY (device_id) REFERENCES public.device(id) ON UPDATE CASCADE ON DELETE CASCADE;


--
-- TOC entry 2026 (class 2606 OID 16412)
-- Name: parameter parameter_type_fk; Type: FK CONSTRAINT; Schema: public; Owner: -
--

ALTER TABLE ONLY public.parameter
    ADD CONSTRAINT parameter_type_fk FOREIGN KEY (type_id) REFERENCES public.type(id);


-- Completed on 2022-12-13 16:32:10 MSK

--
-- PostgreSQL database dump complete
--

