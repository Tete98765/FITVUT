DROP MATERIALIZED VIEW pohlad;
DROP INDEX index_poctu_mrtvych;

DROP TABLE zdravotne_zaznamy;
DROP TABLE vybeh;
DROP TABLE klietka;
DROP TABLE pavilon;
DROP TABLE umiestnenie;
DROP TABLE zivocich;
DROP TABLE kategoria;
DROP TABLE osetrovatel;
DROP TABLE chovatel;
DROP TABLE zamestnanec;

CREATE TABLE zamestnanec (
rod_cislo INT PRIMARY KEY,
cele_meno VARCHAR(255) NOT NULL,
adresa VARCHAR(255) NOT NULL,
mesto VARCHAR(255) NOT NULL,
tel_cislo INT NOT NULL,
datum_nastupu DATE NOT NULL
);

--SELECT * FROM zamestnanec;

ALTER TABLE zamestnanec ADD CONSTRAINT kontrola_rodne_cislo CHECK ( (length(rod_cislo) > 8) AND ((length(rod_cislo) < 11) OR (MOD(rod_cislo, 11)=0 AND (length(rod_cislo) < 11))) );


CREATE TABLE chovatel (
rod_cislo INT PRIMARY KEY REFERENCES zamestnanec ON DELETE CASCADE,
opravnenia VARCHAR(511) NOT NULL
);

--SELECT * FROM chovatel;

CREATE TABLE osetrovatel (
rod_cislo INT PRIMARY KEY REFERENCES zamestnanec ON DELETE CASCADE,
opravnenia VARCHAR(511) NOT NULL
);

--SELECT * FROM osetrovatel;
CREATE TABLE kategoria (
    ID_kategoria INTEGER PRIMARY KEY,
    trieda VARCHAR(255) NOT NULL,
    rad VARCHAR(255) NOT NULL,
    celad VARCHAR(255) NOT NULL,
    rod VARCHAR(255) NOT NULL,
    druh VARCHAR(255) NOT NULL
);

CREATE TABLE zivocich (
ID_zivocicha INT PRIMARY KEY,
datum_narodenia DATE NOT NULL,
datum_umrtia DATE DEFAULT NULL,
meno VARCHAR(255) NOT NULL,
pohlavie VARCHAR(255) CHECK(pohlavie IN('M', 'Z')),
cas_krmenia VARCHAR(255) CHECK(cas_krmenia IN('rano', 'vecer')),
strava VARCHAR(255) NOT NULL,
chovatel INT REFERENCES chovatel ON DELETE CASCADE,
kategoria INT REFERENCES kategoria ON DELETE CASCADE
);

CREATE TABLE umiestnenie (
    pavilon_klietka INT PRIMARY KEY,
    typ VARCHAR(255) NOT NULL,
    kapacita INT NOT NULL,
    u_zivocich INT REFERENCES zivocich ON DELETE CASCADE
);

CREATE TABLE pavilon (
    ID_pavilonu INT PRIMARY KEY,
    typ VARCHAR(255),
    kapacita INT,
    osetrovatel INT REFERENCES osetrovatel ON DELETE CASCADE
);

CREATE TABLE klietka (
    ID_kliekty INT PRIMARY KEY REFERENCES umiestnenie ON DELETE CASCADE,
    umiestnenie INT NOT NULL,
    FOREIGN KEY (umiestnenie) REFERENCES pavilon ON DELETE CASCADE
);


CREATE TABLE vybeh (
    ID_vybehu INT PRIMARY KEY REFERENCES umiestnenie ON DELETE CASCADE,
    osetrovatel INT REFERENCES osetrovatel ON DELETE CASCADE
);

CREATE TABLE zdravotne_zaznamy (
    ID_zaznamu INT PRIMARY KEY,
    datum DATE NOT NULL,
    sprava VARCHAR(255) NOT NULL,
    vyska INT NOT NULL,
    vaha INT NOT NULL,
    diagnoza VARCHAR(255),
    liecba VARCHAR(255),
    chovatel INT REFERENCES chovatel ON DELETE CASCADE,
    id_zivocich INT NOT NULL,
    FOREIGN KEY (id_zivocich) REFERENCES zivocich ON DELETE CASCADE
);

------------------------------------------------------------------------------------------------------------------------------------------------
--vytvorenie trigeru pre automaticku inkrementaciuu 
DROP SEQUENCE zivocich_seq;
DROP SEQUENCE kategoria_seq;
DROP SEQUENCE pavilon_seq;
DROP SEQUENCE zaznamy_seq;
--id zvierata
CREATE SEQUENCE zivocich_seq START WITH 1 INCREMENT BY 1;

CREATE OR REPLACE TRIGGER zivocich_autoinc
BEFORE INSERT ON zivocich
FOR EACH ROW
BEGIN
    :new.ID_zivocicha := zivocich_seq.nextval;
END;
/

--id kategorie
CREATE SEQUENCE kategoria_seq START WITH 11 INCREMENT BY 1;

CREATE OR REPLACE TRIGGER kategoria_autoinc
BEFORE INSERT ON kategoria
FOR EACH ROW
BEGIN
    :new.ID_kategoria := kategoria_seq.nextval;
END;
/

--id pavilonu
CREATE SEQUENCE pavilon_seq START WITH 75 INCREMENT BY 1;

CREATE OR REPLACE TRIGGER pavilon_autoinc
BEFORE INSERT ON pavilon
FOR EACH ROW
BEGIN
    :new.ID_pavilonu := pavilon_seq.nextval;
END;
/

--id zaznamu
CREATE SEQUENCE zaznamy_seq START WITH 56 INCREMENT BY 1;

CREATE OR REPLACE TRIGGER zaznamy_autoinc
BEFORE INSERT ON zdravotne_zaznamy
FOR EACH ROW
BEGIN
    :new.ID_zaznamu := zaznamy_seq.nextval;
END;
/

--trigger pre kontrolu ci nebol datum umrtia nastaveny skor ako datum narodenia alebo ci nebol datum narodenia alebo umrtia zapisany ako datum, ktory este neexistuje
CREATE OR REPLACE TRIGGER kontrola_umrtia
BEFORE INSERT OR UPDATE ON zivocich
FOR EACH ROW
BEGIN
    IF :new.datum_narodenia > :new.datum_umrtia THEN
        Raise_Application_Error(-20322, 'Nespravne zadany datum umrtia');
    END IF;
    IF :new.datum_narodenia > current_date THEN
        Raise_Application_Error(-20322, 'Nespravne zadany datum narodenia - datum este neexistuje');
    END IF;
    IF :new.datum_umrtia > current_date THEN
        Raise_Application_Error(-20322, 'Nespravne zadany datum umrtia - datum este neexistuje');
    END IF;
END;
/
------------------------------------------------------------------------------------------------------------------------------------------------

INSERT INTO zamestnanec VALUES (9610219168, 'Jan Novak', 'Sukova 553/2, 602 00', 'Brno-mesto', 0907079443,  TO_DATE('01/02/2010','MM/DD/YYYY')); --chovatel
INSERT INTO zamestnanec VALUES (9755128790, 'Foro Mrkvicka', 'Petrov 10, 602 00', 'Brno-mesto', 090456987, TO_DATE('01/01/2022','MM/DD/YYYY'));
INSERT INTO zamestnanec VALUES (7506162037, 'Richard Justik', 'Mrova 31, 602 00', 'Brno', 0907012343, TO_DATE('01/01/2015','MM/DD/YYYY')); --chovatel
INSERT INTO zamestnanec VALUES (8657231121, 'Anna Nova', 'Josefska 21, 602 00', 'Brno', 0901179198, TO_DATE('01/01/2022','MM/DD/YYYY')); --chovatel
INSERT INTO zamestnanec VALUES (9011283490, 'Matus Hrman', 'Fisova 553/2, 602 00', 'Brno', 0907075643, TO_DATE('01/01/2022','MM/DD/YYYY'));
INSERT INTO zamestnanec VALUES (9004117782, 'Alfa', 'Fchsova 553/2, 602 00', 'Brno', 09, TO_DATE('01/01/2022','MM/DD/YYYY'));

INSERT INTO chovatel VALUES (9610219168, 'pristup ku zvieratam v pavilone/vybehu cicavcov');
INSERT INTO chovatel VALUES (7506162037, 'pristup ku zvieratam v pavilone/vybehu chordatov');
INSERT INTO chovatel VALUES (8657231121, 'pristup ku zvieratam v pavilone motylov');

INSERT INTO osetrovatel VALUES (9755128790, 'pristup ku krmivam, pristup do ambulancie, pristup do vybehov/pavilonov');
INSERT INTO osetrovatel VALUES (9011283490, 'pristup ku krmivam, pristup do skladu, pristup do vybehov/pavilonov');

INSERT INTO kategoria (trieda, rad, celad, rod, druh) VALUES ('cicavce', 'masozravce', 'psovite', 'vlk', 'vlk dravz');
INSERT INTO kategoria (trieda, rad, celad, rod, druh) VALUES ('cicavce', 'masozravce', 'mackovite', 'puma', 'puma americka');
INSERT INTO kategoria (trieda, rad, celad, rod, druh) VALUES ('cicavce', 'parnokopytniky', 'turovite', 'jak', 'jak divy');
INSERT INTO kategoria (trieda, rad, celad, rod, druh) VALUES ('chordaty', 'vtaky', 'hrabavce', 'bazantovite', 'bazant obycajny');
INSERT INTO kategoria (trieda, rad, celad, rod, druh) VALUES ('chordaty', 'vtaky', 'pstrosy', 'kazuarovite', 'emu');
INSERT INTO kategoria (trieda, rad, celad, rod, druh) VALUES ('chordaty', 'lucoplutvovce', 'srukotvare', 'stukovite', 'stuka');
INSERT INTO kategoria (trieda, rad, celad, rod, druh) VALUES ('clankonozce', 'hmyz', 'motyle', 'priadkovite', 'priadka');

INSERT INTO zivocich (datum_narodenia, meno, pohlavie, cas_krmenia, strava, chovatel, kategoria) VALUES (TO_DATE('01/02/2020','MM/DD/YYYY'), 'Gonzalez', 'M', 'rano', 'maso', 9610219168, 11);
INSERT INTO zivocich (datum_narodenia, meno, pohlavie, cas_krmenia, strava, chovatel, kategoria) VALUES (TO_DATE('04/03/2018','MM/DD/YYYY'), 'Akela', 'Z', 'vecer', 'maso', 7506162037, 12);
INSERT INTO zivocich (datum_narodenia, meno, pohlavie, cas_krmenia, strava, chovatel, kategoria) VALUES (TO_DATE('11/11/2001','MM/DD/YYYY'), 'Raksha', 'Z', 'rano', 'seno, ovos', 9610219168, 13);
INSERT INTO zivocich (datum_narodenia, meno, pohlavie, cas_krmenia, strava, chovatel, kategoria) VALUES (TO_DATE('01/15/2020','MM/DD/YYYY'), 'Maugrin', 'M', 'rano', 'cerviky', 7506162037, 14);
INSERT INTO zivocich (datum_narodenia, meno, pohlavie, cas_krmenia, strava, chovatel, kategoria) VALUES (TO_DATE('11/05/2019','MM/DD/YYYY'), 'Oscar', 'M', 'rano', 'zrno, ovos', 7506162037, 15);
INSERT INTO zivocich (datum_narodenia, meno, pohlavie, cas_krmenia, strava, chovatel, kategoria) VALUES (TO_DATE('01/03/2021','MM/DD/YYYY'), 'Fatima', 'Z', 'rano', 'zubrienky, rybky', 7506162037, 16);
INSERT INTO zivocich (datum_narodenia, meno, pohlavie, cas_krmenia, strava, chovatel, kategoria) VALUES (TO_DATE('03/02/2022','MM/DD/YYYY'), 'Aurora', 'Z', 'rano', 'makke ovocie', 8657231121, 17);
INSERT INTO zivocich (datum_narodenia, datum_umrtia, meno, pohlavie, cas_krmenia, strava, chovatel, kategoria) VALUES (TO_DATE('03/02/2021','MM/DD/YYYY'), TO_DATE('01/02/2022','MM/DD/YYYY'), 'Bilko', 'M', 'rano', 'makke ovocie', 8657231121, 17);

INSERT INTO umiestnenie VALUES(61, 'vybeh cicavcov', 20, 1);
INSERT INTO umiestnenie VALUES(62, 'vybeh cicavcov', 20, 2);
INSERT INTO umiestnenie VALUES(63, 'vybeh cicavcov', 20, 3);
INSERT INTO umiestnenie VALUES(64, 'vybeh chordatov', 10, 4);
INSERT INTO umiestnenie VALUES(65, 'vybeh chordatov', 10, 5);
INSERT INTO umiestnenie VALUES(66, 'vybeh chordatov', 10, 6);
INSERT INTO umiestnenie VALUES(67, 'vybeh chordatov', 10, 7);

INSERT INTO vybeh VALUES(63, 9755128790);
INSERT INTO vybeh VALUES(64, 9011283490);
INSERT INTO vybeh VALUES(65, 9011283490);

INSERT INTO pavilon (typ, kapacita, osetrovatel) VALUES ('pavilon seliem', 50, 9011283490);
INSERT INTO pavilon (typ, kapacita, osetrovatel) VALUES ('pavilon vodnyho sveta', 50, 9755128790);
INSERT INTO pavilon (typ, kapacita, osetrovatel) VALUES ('pavilon motylov', 50, 9011283490);

INSERT INTO klietka VALUES (61, 75);
INSERT INTO klietka VALUES (62, 75);
INSERT INTO klietka VALUES (66, 76);
INSERT INTO klietka VALUES (67, 77);

INSERT INTO zdravotne_zaznamy (datum, sprava, vyska, vaha, diagnoza, liecba, chovatel, ID_zivocich) VALUES (TO_DATE('01/04/2022','MM/DD/YYYY'), 'zaludocne problemy', 85, 75, 'prevrateny zaludok', 'dieta', 9610219168, 1);
INSERT INTO zdravotne_zaznamy (datum, sprava, vyska, vaha, diagnoza, liecba, chovatel, ID_zivocich) VALUES (TO_DATE('01/03/2022','MM/DD/YYYY'), 'poranena lapka', 120, 100, 'aaaaaa', 'hojiva masticka', 7506162037, 2);
INSERT INTO zdravotne_zaznamy (datum, sprava, vyska, vaha, diagnoza, liecba, chovatel, ID_zivocich) VALUES (TO_DATE('01/02/2022','MM/DD/YYYY'), 'natrhnute kridlo', 1, 5, 'blabla', 'kludovy rezim', 8657231121, 7);
-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
--O ake zvierata sa stara Richard Justik?
SELECT meno 
FROM
    zamestnanec za,
    chovatel cho,
    zivocich z
WHERE za.rod_cislo = cho.rod_cislo
AND cho.rod_cislo = z.chovatel
AND za.cele_meno = 'Richard Justik';
    
--Kolko zvierat je umiestnenych vo vybehu chordatov
SELECT COUNT(u_zivocich) as zivocichy
FROM 
    zivocich z,
    umiestnenie u
WHERE z.ID_zivocicha = u.u_zivocich
AND u.typ = 'vybeh chordatov';
    
--Ktory osetrovatelia sa staraju o pavilon seliem?
SELECT DISTINCT cele_meno 
FROM
    zamestnanec za,
    osetrovatel o,
    pavilon p
WHERE  za.rod_cislo = o.rod_cislo
AND o.rod_cislo = p.osetrovatel;

--Ktore zvierata patria do cicavcov a su krmene masom
SELECT DISTINCT meno
FROM
    zivocich z,
    kategoria k
WHERE k.trieda = 'cicavce' and z.strava = 'maso';

--O ktore zvierata sa staraju osetrovatelia ktori nastupili po roku 2012
SELECT DISTINCT za.cele_meno,z.meno
FROM 
    zamestnanec za,
    chovatel cho,
    zivocich z
where datum_nastupu >= TO_DATE('01/01/2012','MM/DD/YYYY')
and za.rod_cislo = cho.rod_cislo
and cho.rod_cislo = z.chovatel
Group by za.cele_meno, z.meno;    

--Ktore zvierata nemaju ziadne zdravotne zaznamy
SELECT DISTINCT meno
FROM zivocich  natural join zdravotne_zaznamy
WHERE zivocich.ID_zivocicha NOT IN (SELECT ID_zivocicha
    FROM
    zivocich NATURAL JOIN zdravotne_zaznamy
    WHERE zivocich.ID_zivocicha = zdravotne_zaznamy.id_zivocich);

--O ktore zvierata sa staraju osetrovatelia ktori su priamo z Brna
SELECT DISTINCT z.meno
FROM 
    zamestnanec za,
    chovatel cho,
    zivocich z
WHERE za.rod_cislo = cho.rod_cislo AND cho.rod_cislo = z.chovatel and EXISTS(SELECT * FROM  zamestnanec za WHERE za.rod_cislo = cho.rod_cislo AND za.mesto = 'Brno');
---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
--procedury
--procedura, ktora SKONTROLUJE CI JE TELEFONNE CISLO V DOBROM FORMATE
CREATE OR REPLACE PROCEDURE cislo AS
CURSOR cislo_zamestnanca IS SELECT * FROM zamestnanec;
pocet INTEGER;
z zamestnanec%ROWTYPE;
BEGIN
    pocet := 0;
    dbms_output.put_line('Zamestnanci so zlym telefonnym cislom: ');
	OPEN cislo_zamestnanca;
	LOOP
		FETCH cislo_zamestnanca INTO z ;
		EXIT WHEN cislo_zamestnanca%NOTFOUND;
		IF LENGTH(to_char(z.tel_cislo)) != 10 THEN
			dbms_output.put_line('Zamestnanec: ' || z.cele_meno || ' '|| ' cislo:' || z.tel_cislo);
			pocet := pocet +1;
		END IF;
	END LOOP;
	dbms_output.put_line('Celkom: ' || pocet);
	CLOSE cislo_zamestnanca;
EXCEPTION
    WHEN OTHERS THEN
        RAISE_APPLICATION_ERROR(-20005, 'chyba procedury');
END;
/

--procedura, ktora vypise meno a kategoriu prveho zivocicha
CREATE OR REPLACE PROCEDURE prvy_zivocich AS
    prvy zivocich.ID_zivocicha%type := 1;
    z_meno zivocich.meno%type;
    z_rod kategoria.rod%type;
BEGIN
    SELECT z.meno, k.rod INTO z_meno, z_rod
    FROM zivocich z LEFT JOIN kategoria k ON z.kategoria = k.ID_kategoria
    WHERE z.ID_zivocicha = prvy;
    dbms_output.put_line('Meno prveho zivocicha: ' || z_meno);
    dbms_output.put_line('Rod prveho zivociha: ' || z_rod);
EXCEPTION
    WHEN no_data_found THEN
        dbms_output.put_line('Zivocich s ID 1 nie je v databazy');
END;
/

--spustenie procedur
EXECUTE cislo;
EXECUTE prvy_zivocich;
---------------------------------------------------------------------------------------------------------------------------------------------------------------------------
--index a example plan

--kolko zvierat umrelo 
CREATE INDEX index_poctu_mrtvych ON zivocich(datum_umrtia);

EXPLAIN PLAN FOR
    SELECT COUNT(*) AS pocet_umrti
    FROM zivocich z
    WHERE z.datum_umrtia IS NOT NULL;
SELECT * FROM TABLE(DBMS_XPLAN.DISPLAY());

--kolko zvierat je umiestnenych v ktorom vybehu
EXPLAIN PLAN FOR
    SELECT u.u_zivocich, COUNT(*) AS pocet_zvierat
    FROM zivocich z,
        umiestnenie u,
        vybeh v
    WHERE z.ID_zivocicha = u. u_zivocich and u.pavilon_klietka = v.ID_vybehu
    GROUP BY  u.u_zivocich;
SELECT * FROM TABLE(DBMS_XPLAN.DISPLAY());

----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
---pridelenie pristupovych prav pre druheho clena tymu
--tabulky
GRANT ALL ON zamestnanec TO xcsalo00;
GRANT ALL ON vybeh TO xcsalo00;
GRANT ALL ON klietka TO xcsalo00;
GRANT ALL ON pavilon TO xcsalo00;
GRANT ALL ON umiestnenie TO xcsalo00;
GRANT ALL ON zivocich TO xcsalo00;
GRANT ALL ON kategoria TO xcsalo00;
GRANT ALL ON osetrovatel TO xcsalo00;
GRANT ALL ON chovatel TO xcsalo00;
GRANT ALL ON zdravotne_zaznamy TO xcsalo00;

--procedury
GRANT EXECUTE ON cislo TO xcsalo00;
GRANT EXECUTE ON prvy_zivocich TO xcsalo00;

--pohlad pre druheho clena tymu
CREATE MATERIALIZED VIEW pohlad 
AS SELECT DISTINCT meno
FROM
    zivocich z,
    kategoria k
WHERE z.kategoria = k.ID_kategoria and k.trieda = 'chordaty';

GRANT ALL ON pohlad TO xcsalo00;

COMMIT;
SELECT * FROM pohlad; 