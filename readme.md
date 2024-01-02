# Cudoku
Sudoku je jedna z klasických her, kterou většina lidí zná z denního tisku. Dočkala se i moderních verzí v podobě různých mobilních aplikací a podobně. Zde je krátký souhrn pravidel hry:
1) Tabulka sestavá z 9 sloupců a 9 řádků
2) V tabulce je vyznačených dále devět "čtverců” nebo také "bloků” o velikosti 3*3 buňky
3) Do buněk smíme dosazovat čísla 1-9
4) Cílem je vyplnit celou tabulku tak, aby v každém sloupci, řádku i bloku byla všechna čísla 1-9, a to každé právě jednou.
5) Existuje speciální verze sudoku, kde obdobné pravidlo platí také pro obě diagonály, tím se zde zabývat nebudeme.
6) Sudoku je zadáno předvyplněním některých buněk. "Dobře zadané" sudoku má právě jedno validní řešení

Cíle, které jsem si pro tuto práci vytičil:
1) Hratelné sudoku
2) Ověření správnosti
3) Nalezení chyby
4) Automatické řešení hry
5) Poskytování nápověd uživateli
6) Generování nových zadání

## Analýza problému
Je třeba hru ukládat do paměti i souboru, označit, jaký je původ vyplněných buňěk (zdali byly přednastavené nebo je vyplnil uživatel). Dále je třeba herní pole vytisknout do konzole a nějakým způsobem vyznačit kursor. Příkazy, které bude třeba implementovat jsou:
- Načtení
- Uložení
- Pohyb kursoru
- Nastavení hodnoty buňky

Tím bude splněn cíl číslo 1. Pro splnění cílů 2,3,5 a 6 je radno předem realizovat cíl 5. To je implementace:
- Zjištění zbývajících možností v buňce
- Počítání polí s možnostmi v každé skupině (sloupce, řádky, čtverce)
- Ověření, že je pole zcela vyřešené

## Popis řešení
Kód je dělen do souborů podle dílčích funkčních jednotek. Soubory jsou opatřeny příslušnými header .h soubory s deklaracemi. Všechny ```#include``` statementy jsou též v .h. Osobně mi přijde přehledné psát do jednoho souboru vždy kód podporující jednu strukturu nebo koncept. Testovací prostředí bylo virtualizované, OS **Debian** Bookworm na architektuře AMD x86. Jako kompilátor jsem využíval **GCC**, jehož vstupy byly automaticky generovány skrz CMake a zpracovány Make. Program je kompatibilní i s optimalizační strategií O3 v GCC, zlepšení proti výchozímu programu je cca 10%. V mém řešení jsem se snažil vyvážit rychlost se spotřebou paměti.

Základem je uložení sudoku v operační paměti. Zvolil jsem přístup, který sice spotřebuje více paměti, ale umožňuje pak jednodušší a rychlejší práci se hrou. Jsou definovány struktury Cell a Playfield. K ukládání hodnot buněk používám
```c
typedef usigned int Options;
```
Zvolil jsem použití jednotlivých bitů pro různé možnosti, k čemuž se vrátím později, každopádně LSB je 1, ```1<<8``` je devítka. Buňky jako takové jsou uloženy ve struktuře
```c
typedef struct {
    char x;
    char y;
    Options options;
    char solveBased;
} Cell;
```

Char je použit pro x a y protože jejich maximální hodnota je 8 (řádky, sloupce 0-8). solveBased by mohl být enum, jelikož ukládá pouze údaje o tom, kterým algoritmem byla buňka vyřešena, ale osobně nemám enum rád, je zbytečně velký a "upovídaný". Každá buňka tedy "ví" kde je, jakou má hodnotu (resp. potenciální hodnotu), jestli byla nastavena uživatelem (potažmo řešícím algoritmem) nebo jestli byla přednastavena v zadání.

Struktura Playfield
```c
typedef struct {
    Cell* rows[9][9];
    Cell* cols[9][9];
    Cell* blocks[9][9];
    Cell* cells[9*9];
    char solvedCells;
} Playfield;
```

pak drží jednotlivé buňky a počet vyřešených buněk. Také obsahuje trojici dvourozměrných polí odkazů na jednotlivé buňky, jedno pro sloupce, řádky a čtverce. Tato pole jsou vždy jednorázově naplněna při tvorbě herního pole. Slouží jako "index" pro rychlé vyhledávání buněk. Pole ```cells[9*9]``` drží také tytéž buňky a není nutné, ale usnadňuje práci při smyčkování přes všechny buňky - *uvažoval jsem o jeho odstranění a pravděpodobně to bude jedna z budoucích optimalizací*.

Funkce ```main``` na začátku inicializuje herní pole ze souboru. Pro soubory jsem zvolil příponu **.cudoku** (sudoku v céčku). Jedná se v podstatě o CSV, které drží x-ové a y-ové souřadnice buněk, jejich hodnoty (NIKOLIV možnosti, nevyřešené buňky neukládáme) a jejich výchozí stav ad. ```Cell.solveBased```. Pole je načítáno ze složky ```./saves/```, proto není žádoucí zadávat cestu k souboru ani příponu, postačí jméno "levelu". Ve výchozím stavu je připraveno 7 herních polí:
-	preset[1-4]
-	evil[1-2]
-	empty

Pro začátek "od nuly" lze načíst soubor ```empty.cudoku```, který neobsahuje žádné buňky. Presety 1 a 2 jsou řešitelné logicky (bez hádání a rekurse), trojka spustí dlouhou rekursi (cca 100k kroků), 4 vyžaduje dva rekursivní kroky a pole označená evil pochází z internetu a jsou údajně nejtěžšími sudoku, která jsou "dobře zadaná". 

Do souboru lze uložit příkazem ```save``` a následným zadáním jména. 

Dále lze uložit jako zadání, kdy se všechna pole, bez ohledu na jejich původ uloží jako přednastavené buňky. Tedy ```Cell.solveBased``` je ignorováno a všude je uložena hodnota ```'p'``` tedy "preset".

Tabulka jako taková se tiskne do konzole pomocí "grafických" boxdrawing ASCII znaků a různých modifikátorů/escape sekvencí pro tučné písmo, vyznačení kurzoru, vymazání terminálu a podobně.

**Stručná dokumentace ke každé funkci i proměné je uveda v komentářích přímo v kódu. Markup je kompatibilní s Intellisense.**

### Solver
Součástí mého řešení je také komponeta pro řešení zadaného sudoku. Na této komponentě jsem strávil solidně nejvíce času a ovlivnila většinu ostatních strukturálních rozhodnutí v celém programu. Implementuje tři řešící metody, které dohramady dokážou vyřešit každé *dobře zadané* sudoku. Jedná se o funkce: 
- ```removeOptions```: Funkce všechny buňky očistí od možností, které v nich nemohou logicky existovat (například v řádku, který už obsahuje číslo 3 nelze předpokládat další buňku s trojkou)
- ```onlyInReg```: Vyhledává buňky, které jsou v dané skupině jedinné s potenciálem držet nějaké číslo. Například když pouze jedna buňka ve sloupci může obsahovat číslo 3, pak ji obsahovat musí
- ```recourse```: Vybere buňku s nejnižším počtem možností a zkusí je postupně dosazovat. To se opakuje, dokud není celé pole vyřešené

Odstraňování a vyhledávání možností je primární důvod proč hodnotu buněk ukládám jako bitfield v ```Cell.options```. Například odstranění možnosti ```n``` je pak extrémně rychlé:
```c
exampleCell.options &= ~(1 << (n-1)); // Odstraní n-tou možnost
```
podobně je rychlé ověření, zdali je buňka již vyřešená, protože potom musí ```Cell.options``` být mocnina dvou:
```c
!(cell.options & (cell.options-1));
```

Kdyby program měl sudoku pouze řešit a nemusel být hratelnout hrou, dala by se struktura buněk ztenčit o ```Cell.solveBased```, tato proměnná původně vznikla jako čistě diagnostický údaj.

### Měření rychlosti solveru

Také jsem ze zajímavosti napsal kratičký script na evaluaci rychlosti mého řešení:
```bash
time ./time.sh
```
Script se pokusí 1000x vyřešit zadání ```preset4``` a vrátí jak dlouho to trvalo. Ve virtuálním vývojovém prostředí to trvalo cca 10,5s teda cca 10,5ms pokaždé a to včetně načítání dat z disku a inicializace herního pole - tedy včetně budování indexů.

## Popis ovládání
Kompilovaná a optimalizovaná verze programu je ```./build/cudoku```
Build je nejlepší vyvolat následovně:
```bash
mkdir build
cd build
cmake -D CMAKE_BUILD_TYPE=Release ..
make
```

Po spuštění programu bez parametrů zadejte název hry, kterou chcete načíst. ```empty``` načte prázdné hřiště. Poté jsou dostupné následující příkazy:
- ```load```: Načte pole ze souboru
- ```save```: Uloží pole do souboru
- ```save!```: Uloží pole do souboru jako zadání
- ```exit```: ukončí program
- ```set [v]```: Nastaví právě vybranou buňku na hodnotu ```[v]```, pro ```[v] == 0``` buňku vymaže
- ```D```/```U```/```L```/```R```: Posune kursor dolů/nahoru/vlevo/vpravo
- ```w```/```a```/```s```/```d```: Posune kursor nahoru/vlevo/dolů/vpravo
- ```solve```: Pokusí se vyřešit pole
    - Pokud pole lze vyřešit, zobrazí jeho řešenou podobu
    - Pokud pole nelze vyřešit, zobrazí senam nevyřešených buněk
    - U polí s chybou může dojít na velmi dlouhou rekursi, která vyzkouší všechny možnosti a program pak prohlásí pole za neřešitelné
    - Jako výchozí pro dořešení pole se pokládá aktuální stav, včetně uživatelem vyplněných buňěk
- ```choises```: nápověda v podobně všech čísel, která lze do dané buňky dosadit
- ```check```: vytiskne seznam chybně vyplňěných buněk

Podrobnější popis ovládání a návod k jednotlivým příkazům lze zobrazit příkazem ./build/cudoku help
