
DROP TABLE objednavka_zabavna;
DROP TABLE polozka_objednavky;
DROP TABLE objednavka;
DROP TABLE druh_pizzy;
DROP TABLE pizzerie_otevreno;
DROP TABLE pizzerie;
DROP TABLE vlastnik;
DROP TABLE bavic_dostupny;
DROP TABLE bavic_umi;
DROP TABLE bavic;
DROP TABLE druh_zabavy;
DROP TABLE hladovec;
DROP TABLE zakaznik;

CREATE TABLE zakaznik (
id INT PRIMARY KEY,
jmeno VARCHAR(255) NOT NULL,
adresa VARCHAR(255) NOT NULL
);

CREATE TABLE hladovec (
id INT PRIMARY KEY REFERENCES zakaznik ON DELETE CASCADE
);

CREATE TABLE druh_zabavy (
nazev VARCHAR(255) PRIMARY KEY
);

CREATE TABLE bavic (
id INT PRIMARY KEY REFERENCES zakaznik ON DELETE CASCADE,
scena VARCHAR(511) NOT NULL,
cv VARCHAR(511) NOT NULL,
cena_pulhodina NUMERIC(10,2) NOT NULL CHECK (cena_pulhodina > 0)
);

CREATE TABLE bavic_umi (
bavic INT REFERENCES bavic ON DELETE CASCADE,
zabava VARCHAR(255) REFERENCES druh_zabavy ON DELETE CASCADE,
PRIMARY KEY (bavic, zabava)
);

CREATE TABLE bavic_dostupny (
bavic INT REFERENCES bavic ON DELETE CASCADE,
den CHAR(2) CHECK (den IN ('PO', 'UT', 'ST', 'CT', 'PA', 'SO', 'NE')),
hodin_od INT NOT NULL CHECK (hodin_od BETWEEN 0 AND 23),
hodin_do INT NOT NULL CHECK (hodin_do BETWEEN 0 AND 23),
PRIMARY KEY (bavic, den),
CHECK (hodin_do > hodin_od)
);

CREATE TABLE vlastnik (
id INT PRIMARY KEY REFERENCES zakaznik ON DELETE CASCADE
);

CREATE TABLE pizzerie (
nazev VARCHAR(255),
adresa VARCHAR(255),
vlastnik INT REFERENCES vlastnik ON DELETE CASCADE,
PRIMARY KEY (nazev, adresa)
);

CREATE TABLE pizzerie_otevreno (
pizzerie_nazev VARCHAR(255),
pizzerie_adresa VARCHAR(255),
den CHAR(2) CHECK (den IN ('PO', 'UT', 'ST', 'CT', 'PA', 'SO', 'NE')),
hodin_od INT NOT NULL CHECK (hodin_od BETWEEN 0 AND 23),
hodin_do INT NOT NULL CHECK (hodin_do BETWEEN 0 AND 23),
PRIMARY KEY (pizzerie_nazev, pizzerie_adresa, den),
FOREIGN KEY (pizzerie_nazev, pizzerie_adresa)
REFERENCES pizzerie ON DELETE CASCADE,
CHECK (hodin_do > hodin_od)
);

CREATE TABLE druh_pizzy (
pizzerie_nazev VARCHAR(255),
pizzerie_adresa VARCHAR(255),
jmeno VARCHAR(255),
cena NUMERIC(10,2) NOT NULL CHECK (cena >= 0),
krusta VARCHAR(255) NOT NULL,
PRIMARY KEY (pizzerie_nazev, pizzerie_adresa, jmeno),
FOREIGN KEY (pizzerie_nazev, pizzerie_adresa)
REFERENCES pizzerie ON DELETE CASCADE
);

CREATE TABLE objednavka (
id INT PRIMARY KEY,
datum_a_cas_objednani DATE DEFAULT CURRENT_TIMESTAMP NOT NULL,
datum_a_cas_doruceni DATE NOT NULL,
zakaznik INT NOT NULL REFERENCES hladovec
);

CREATE TABLE polozka_objednavky (
objednavka INT REFERENCES objednavka,
pizzerie_nazev VARCHAR(255),
pizzerie_adresa VARCHAR(255),
pizza VARCHAR(255),
pocet_kusu INT NOT NULL CHECK (pocet_kusu > 0),
PRIMARY KEY (objednavka, pizzerie_nazev, pizzerie_adresa, pizza),
FOREIGN KEY (pizzerie_nazev, pizzerie_adresa, pizza) REFERENCES druh_pizzy
);

CREATE TABLE objednavka_zabavna (
id INT PRIMARY KEY REFERENCES objednavka,
delka_minut FLOAT NOT NULL CHECK (delka_minut > 1),
zabava VARCHAR(255) NOT NULL REFERENCES druh_zabavy
);

INSERT INTO zakaznik VALUES (12, 'Jan Novák', 'Sukova 553/2, 602 00 Brno-mìsto');
INSERT INTO zakaznik VALUES (13, 'Fero Mrkvièa', 'Fišova 35, 602 00, Brno-mìsto');
INSERT INTO zakaznik VALUES (14, 'Anna Hnedá', 'Plotní 50, 602 00, Brno-mìsto');
INSERT INTO zakaznik VALUES (15, 'Jana Zelená', 'Rudišova 13, 602 00, Brno-mìsto');

--SELECT * FROM zakaznik;

INSERT INTO hladovec VALUES (12);
INSERT INTO hladovec VALUES (13);
INSERT INTO hladovec VALUES (14);
INSERT INTO hladovec VALUES (15);

--SELECT * FROM hladovec;

INSERT INTO druh_zabavy VALUES ('šašovská zábava');
INSERT INTO druh_zabavy VALUES ('princezovská zábava');
INSERT INTO druh_zabavy VALUES ('dospelácka zábava'); 
INSERT INTO druh_zabavy VALUES ('hudobná zábava'); 

--SELECT * FROM druh_zabavy;

INSERT INTO bavic VALUES (12, 'bla', 'blabla', 100);
INSERT INTO bavic VALUES (13, 'bla', 'blabla', 130);
INSERT INTO bavic VALUES (14, 'bla', 'blabla', 110);
INSERT INTO bavic VALUES (15, 'bla', 'blabla', 150);

--SELECT * FROM bavic;

INSERT INTO bavic_umi VALUES (12, 'šašovská zábava');
INSERT INTO bavic_umi VALUES (13, 'princezovská zábava');
INSERT INTO bavic_umi VALUES (14, 'dospelácka zábava');
INSERT INTO bavic_umi VALUES (15, 'hudobná zábava');

--SELECT * FROM bavic_umi;

INSERT INTO bavic_dostupny VALUES (12, 'PO', 10, 20);
INSERT INTO bavic_dostupny VALUES (12, 'UT', 10, 20);
INSERT INTO bavic_dostupny VALUES (12, 'ST', 10, 20);
INSERT INTO bavic_dostupny VALUES (13, 'PA', 9, 23);
INSERT INTO bavic_dostupny VALUES (13, 'SO', 9, 23);
INSERT INTO bavic_dostupny VALUES (13, 'NE', 9, 23);
INSERT INTO bavic_dostupny VALUES (14, 'ST', 9, 23);
INSERT INTO bavic_dostupny VALUES (14, 'CT', 9, 23);
INSERT INTO bavic_dostupny VALUES (14, 'PA', 17, 23);
INSERT INTO bavic_dostupny VALUES (15, 'NE', 10, 22);
INSERT INTO bavic_dostupny VALUES (15, 'PO', 10, 22);
INSERT INTO bavic_dostupny VALUES (15, 'UT', 10, 22);

--SELECT * FROM bavic_dostupny;

INSERT INTO vlastnik VALUES (13);
INSERT INTO vlastnik VALUES (14);
INSERT INTO vlastnik VALUES (15);

INSERT INTO pizzerie VALUES ('Ortica Pizza', 'Palackého tø. 1785', 13);
INSERT INTO pizzerie VALUES ('Pizzeria Al Capone', '6, Hrnèíøska 573', 14);
INSERT INTO pizzerie VALUES ('La Speranza', 'Lidická1871/32', 15);

--SELECT * FROM pizzerie;

INSERT INTO pizzerie_otevreno VALUES ('Ortica Pizza', 'Palackého tø. 1785', 'PO', 8, 22);
INSERT INTO pizzerie_otevreno VALUES ('Ortica Pizza', 'Palackého tø. 1785', 'UT', 8, 22);
INSERT INTO pizzerie_otevreno VALUES ('Ortica Pizza', 'Palackého tø. 1785', 'ST', 8, 22);
INSERT INTO pizzerie_otevreno VALUES ('Ortica Pizza', 'Palackého tø. 1785', 'CT', 8, 22);
INSERT INTO pizzerie_otevreno VALUES ('Ortica Pizza', 'Palackého tø. 1785', 'PA', 8, 22);
INSERT INTO pizzerie_otevreno VALUES ('Ortica Pizza', 'Palackého tø. 1785', 'SO', 12, 20);
INSERT INTO pizzerie_otevreno VALUES ('Ortica Pizza', 'Palackého tø. 1785', 'NE', 12, 20);

INSERT INTO pizzerie_otevreno VALUES ('Pizzeria Al Capone', '6, Hrnèíøska 573', 'PO', 11, 22);
INSERT INTO pizzerie_otevreno VALUES ('Pizzeria Al Capone', '6, Hrnèíøska 573', 'UT', 11, 22);
INSERT INTO pizzerie_otevreno VALUES ('Pizzeria Al Capone', '6, Hrnèíøska 573', 'ST', 11, 22);
INSERT INTO pizzerie_otevreno VALUES ('Pizzeria Al Capone', '6, Hrnèíøska 573', 'CT', 11, 22);
INSERT INTO pizzerie_otevreno VALUES ('Pizzeria Al Capone', '6, Hrnèíøska 573', 'PA', 11, 23);
INSERT INTO pizzerie_otevreno VALUES ('Pizzeria Al Capone', '6, Hrnèíøska 573', 'SO', 11, 18);
INSERT INTO pizzerie_otevreno VALUES ('Pizzeria Al Capone', '6, Hrnèíøska 573', 'NE', 11, 18);
 
INSERT INTO pizzerie_otevreno VALUES ('La Speranza', 'Lidická1871/32', 'PO', 10, 20);
INSERT INTO pizzerie_otevreno VALUES ('La Speranza', 'Lidická1871/32', 'UT', 10, 20);
INSERT INTO pizzerie_otevreno VALUES ('La Speranza', 'Lidická1871/32', 'ST', 10, 20);
INSERT INTO pizzerie_otevreno VALUES ('La Speranza', 'Lidická1871/32', 'CT', 10, 20);
INSERT INTO pizzerie_otevreno VALUES ('La Speranza', 'Lidická1871/32', 'PA', 10, 21);
INSERT INTO pizzerie_otevreno VALUES ('La Speranza', 'Lidická1871/32', 'SO', 13, 20);
INSERT INTO pizzerie_otevreno VALUES ('La Speranza', 'Lidická1871/32', 'NE', 13, 20);

--SELECT * FROM pizzerie_otevreno;

INSERT INTO druh_pizzy VALUES ('Ortica Pizza', 'Palackého tø. 1785', 'Antivegetariana pizza', 219, 'èesnek'); 
INSERT INTO druh_pizzy VALUES ('Ortica Pizza', 'Palackého tø. 1785', 'Diablo pizza', 209, 'èesnek'); 
INSERT INTO druh_pizzy VALUES ('Ortica Pizza', 'Palackého tø. 1785', 'Funghi pizza', 149, 'èesnek'); 
INSERT INTO druh_pizzy VALUES ('Ortica Pizza', 'Palackého tø. 1785', 'Margarita pizza', 149, 'èesnek'); 
INSERT INTO druh_pizzy VALUES ('Ortica Pizza', 'Palackého tø. 1785', 'Salami pizza', 179, 'èesnek'); 

INSERT INTO druh_pizzy VALUES ('Pizzeria Al Capone', '6, Hrnèíøska 573', 'Antivegetariana Pizza', 219, 'èesnek'); 
INSERT INTO druh_pizzy VALUES ('Pizzeria Al Capone', '6, Hrnèíøska 573', 'Diablo pizza', 209, 'èesnek'); 
INSERT INTO druh_pizzy VALUES ('Pizzeria Al Capone', '6, Hrnèíøska 573', 'Funghi pizza', 149, 'èesnek'); 
INSERT INTO druh_pizzy VALUES ('Pizzeria Al Capone', '6, Hrnèíøska 573', 'Margarita pizza', 149, 'èesnek'); 
INSERT INTO druh_pizzy VALUES ('Pizzeria Al Capone', '6, Hrnèíøska 573', 'Salami pizza', 179, 'èesnek'); 
 
INSERT INTO druh_pizzy VALUES ('La Speranza', 'Lidická1871/32', 'Antivegetariana Pizza', 219, 'èesnek'); 
INSERT INTO druh_pizzy VALUES ('La Speranza', 'Lidická1871/32', 'Diablo pizza', 209, 'èesnek'); 
INSERT INTO druh_pizzy VALUES ('La Speranza', 'Lidická1871/32', 'Funghi pizza', 149, 'èesnek'); 
INSERT INTO druh_pizzy VALUES ('La Speranza', 'Lidická1871/32', 'Margarita pizza', 149, 'èesnek'); 
INSERT INTO druh_pizzy VALUES ('La Speranza', 'Lidická1871/32', 'Salami pizza', 179, 'èesnek'); 

--SELECT * FROM druh_pizzy;

INSERT INTO objednavka VALUES (1, '2022-03-25 18:20:00', '2022-03-25 18:20:00', 14); 
INSERT INTO objednavka VALUES (2, '2022-03-25 18:20:00', '2022-03-25 18:20:00', 13); 
INSERT INTO objednavka VALUES (3, '2022-03-25 18:20:00', '2022-03-25 18:20:00', 12);
INSERT INTO objednavka VALUES (4, '2022-03-25 15:20:00', '2022-03-25 16:15:00', 13);
INSERT INTO objednavka VALUES (5, '2022-03-25 15:20:00', '2022-03-25 16:15:00', 13);
INSERT INTO objednavka VALUES (6, '2022-03-25 18:20:00', '2022-03-25 18:20:00', 12);

--SELECT * FROM objednavka;

INSERT INTO polozka_objednavky VALUES (1, 'La Speranza', 'Lidická1871/32', 'Antivegetariana pizza', 1);
INSERT INTO polozka_objednavky VALUES (2, 'Pizzeria Al Capone', '6, Hrnèíøska 573', 'Diablo pizza', 2);
INSERT INTO polozka_objednavky VALUES (2, 'Pizzeria Al Capone', '6, Hrnèíøska 573', 'Margarita pizza', 2);
INSERT INTO polozka_objednavky VALUES (3, 'Ortica Pizza', 'Palackého tø. 1785', 'Salami pizza', 1);
INSERT INTO polozka_objednavky VALUES (4, 'Ortica Pizza', 'Palackého tø. 1785', 'Salami pizza', 1);
INSERT INTO polozka_objednavky VALUES (1, 'Ortica Pizza', 'Palackého tø. 1785', 'Salami pizza', 1);
INSERT INTO polozka_objednavky VALUES (5, 'Ortica Pizza', 'Palackého tø. 1785', 'Salami pizza', 1);
INSERT INTO polozka_objednavky VALUES (6, 'Pizzeria Al Capone', '6, Hrnèíøska 573', 'Margarita pizza', 2);

--SELECT * FROM polozka_objednavky;

INSERT INTO objednavka_zabavna VALUES (2, 60, 'šašovská zábava');
INSERT INTO objednavka_zabavna VALUES (3, 50, 'dospelácka zábava');
INSERT INTO objednavka_zabavna VALUES (4, 50, 'princezovská zábava');
INSERT INTO objednavka_zabavna VALUES (6, 50, 'princezovská zábava');
--SELECT * FROM objednavka_zabavna;


SELECT DISTINCT pizzerie_nazev, pizzerie_adresa
FROM
	zakaznik z,
	objednavka o,
	polozka_objednavky po
WHERE z.id = o.zakaznik
AND po.objednavka = o.id
AND z.jmeno = 'Jan Novák'

SELECT DISTINCT z.jmeno, po.pizzerie_nazev, po.pizzerie_adresa
FROM zakaznik z
	JOIN objednavka o ON o.zakaznik = z.id
	JOIN polozka_objednavky po ON o.id = po.objednavka
	LEFT JOIN objednavka_zabavna oz ON o.id = oz.id
WHERE oz.id IS NOT NULL 
AND (z.jmeno) NOT IN (SELECT z.jmeno FROM zakaznik z
JOIN objednavka o ON o.zakaznik = z.id
JOIN polozka_objednavky po ON o.id = po.objednavka
LEFT JOIN objednavka_zabavna oz ON o.id = oz.id
WHERE oz.id IS NULL);

/*SELECT DISTINCT z.jmeno, po.pizzerie_nazev, po.pizzerie_adresa
FROM zakaznik z
	JOIN objednavka o ON o.zakaznik = z.id
	JOIN polozka_objednavky po ON o.id = po.objednavka
	LEFT JOIN objednavka_zabavna oz ON o.id = oz.id
WHERE oz.id IS NOT NULL */

SELECT z.jmeno, CAST(SUM(COALESCE(oz.delka_minut,0)) / 60 AS NUMERIC(10,2)) AS pocet_hodin
FROM bavic
	JOIN zakaznik z ON bavic.id = z.id
	LEFT JOIN bavic_umi bu ON bavic.id = bu.bavic
	LEFT JOIN objednavka_zabavna oz ON bu.zabava = oz.zabava
GROUP BY z.jmeno;

SELECT p.pizzerie_nazev, p.pizzerie_adresa, p.den, z.jmeno
FROM bavic_dostupny
	JOIN bavic b ON bavic_dostupny.bavic = b.id
	JOIN zakaznik z ON b.id = z.id
	JOIN pizzerie_otevreno p ON bavic_dostupny.den = p.den
WHERE bavic_dostupny.hodin_do >= p.hodin_do
AND bavic_dostupny.hodin_od <= p.hodin_od;

SELECT top 1 pizzerie_nazev, pizzerie_adresa, COUNT(DISTINCT zakaznik) as zakaznici 
FROM polozka_objednavky polozka
JOIN objednavka o ON o.id = polozka.objednavka
GROUP BY pizzerie_nazev, pizzerie_adresa
ORDER BY zakaznici DESC