# IPK - Projekt 1
**Dátum:** 8.3.2022

Cieľom projektu bolo vytvoriť server, ktorý bude komunikovať prostredníctvom HTTP protokolu a poskytovať rôzne informácie o systéme. Komunikácia so serverom je možná pomocou webového prehliadača alebo pomocou nástrojov wget a curl.

### **Preklad a spustenie aplikácie**
Preklad sa vykonáva pomocou priloženého súboru makefile
```
$ make
```
Výsledok po úspešnom preklade v konzoly 

```
gcc projekt1.c -o hinosvc
```
Server môžme následne spustiť
```
./hinosvc port
```
Príklad spustenia
```
./hinosvc 12345 &
```
Z webového prehliadača alebo programu, ktorý vie zasielať HTTP požiadavky zašleme požiadavok GET/curl
```
GET http://server:port/požiadavok
curl http://server:port/požiadavok
``` 

| Požiadavka   | Odpoveď                                | 
| ---------    | -------------------------------------- |
| `\hostname`  | Doménové meno počítača počítače        |
| `\cpu-name`  | Názov processoru                       |
| `\load`      | Aktuálne informácie o záťaži           |

Príklad použitia
```
GET http://localhost:12345/hostname
curl http://localhost:12345/hostname
``` 
Upratanie binárnych súborov
```
make clean
``` 
