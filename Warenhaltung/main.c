#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define HALLE_20 18000 //vorhandene Anzahl Positions IDs für 20cm hohe Fächer
#define HALLE_40 54000
#define PORTA_20 34800
#define PORTA_40 6000
#define PORTA_80 7800


// StartIDs zum Zaehlen; Halle = Lager 1; PW = Lager 2
#define START_ID_HALLE_20CM 12000001
#define START_ID_HALLE_40CM 14000001
#define START_ID_PORTA_20CM 22000001
#define START_ID_PORTA_40CM 2400001
#define START_ID_PORTA_80CM 2400001

// Maximale Anzahl von Artikeln
#define MAX_ARTIKEL 400

int menue();
int neuen_artikel_anlegen();
int artikel_bearbeiten();
int details_waehlen_bearbeitung(struct ArtikelTyp *artikel);
int artikel_erfassen();
int artikel_einlagern_nach_nummer(int eingabeNummer, int lager, struct Artikel artikel);
// artikel_einlagern_nach_name(char eingabeName[], int lager);
int berechne_belegte_ids_Halle(struct Artikel artikel);
int berechne_belegte_ids_Porta(struct Artikel artikel);
void print_zugeordnete_ids(struct Artikel artikel);
int vorhandene_artikel_ansehen();
int lager_aktualisieren(struct ArtikelTyp *artikel);
void bs_loeschen();
void strtrim(char* str);    //Leerzeichen am Anfang und Ende einer Eingabe ignorieren/wegschneiden

int inventarnummerZaehler = 1000;
int belegte_id_halle_20[HALLE_20]; // Array für belegte IDs im Halle-Lager mit 20cm Höhe
int belegte_id_halle_40[HALLE_40]; // Array für belegte IDs im Halle-Lager mit 40cm Höhe
int belegte_id_porta_20[PORTA_20]; // Array für belegte IDs im Porta-Lager mit 20cm Höhe
int belegte_id_porta_40[PORTA_40]; // Array für belegte IDs im Porta-Lager mit 40cm Höhe
int belegte_id_porta_80[PORTA_80]; // Array für belegte IDs im Porta-Lager mit 80cm Höhe

//int belegte_id_halle_20[];
/* Beispiel

// id_20 => 2018000
// id_40 => 4054000
// id_80 => 807800

// resthöhe
    // Minimale Höhe für einen Artikel festlegen?
       // (minimal 5cm und Maximal 80cm)
       // (Wenn ein Fach eine Resthöhe von z.B. 5cm hat, dann wird es als "voll" angesehen)

// Beispiel
// Artikel (20x30x120)
// (Höhe, Breite, Tiefe)
*/
struct PositionsID {
    int id;
    int resthöhe;
    int positions_id_voll;
    int artikelnummer; // Artikelnummer an dieser Position
};

struct ArtikelTyp {
    char name[100];
    int art_nummer;
    double preis;
    double hoehe;
    double breite;
    double tiefe;
    int lager;
};

struct Artikel {
    struct ArtikelTyp typ;
    int inventarnummer;
    struct PositionsID* positions; // Pointer auf PositionsID
    int num_positions;  // Anzahl PosIDs
};


//int belegte_id_halle_20[18000] = [2000001, 2000002, 2000003, ..., 2000245];
/*
if (hoehe <= 20) {
    int id = 2000001;
    max_anzahl = HALLE_20 + 2000000;
    anzahl_belegter_ids = breite / 10;          //Anzahl IDs, die Artikel belegt
        for (int id; id <= max_anzahl; id++) {
            if id in belegte_id_halle_20{
                break;
            }
            else
                if artikel_name == aktueller_artikel_name
                    if PositionsID[id].resthöhe >= artikel_höhe
                        // einlagern
                    else
                        // nächste ID
                else
                    // nächste ID
            else // ID noch nicht belegt
                //artikel einlagern und resthöhe berechnen
    }
else if ((hoehe > 20) && (hoehe <= 40))
    int id = 4000001;
    max_anzahl = HALLE_40 + 4000000;
    anzahl_belegter_ids = breite / 10;
}


*/

//Enumeration, gibt HALLE Wert 1 und PW Wert 2
enum LagerTyp {
    HALLE = 1,  //verderblich
    PORTA_WESTFALICA = 2    //nicht verderblich
};


struct Lager {
    struct ArtikelTyp artikel_liste[MAX_ARTIKEL];
    int anzahl_artikel;
};

struct Lager halle_lager;
struct Lager porta_lager;

// Liste für bereits angelegte Artikel
struct ArtikelTyp artikel_liste[MAX_ARTIKEL];

// Anzahl der bereits vorhandenen Artikel
int anzahl_artikel = 0;





// Hier geht der richtige Code los

int main(void) {
    int auswahl;

    // Initialisierung der Lager
    halle_lager.anzahl_artikel = 0;
    porta_lager.anzahl_artikel = 0;

    while (1) {
        menue();

        scanf("%d", &auswahl);

        switch (auswahl) {
        case 1:
            neuen_artikel_anlegen();
            break;
        case 2:
            artikel_bearbeiten();
            break;
        case 4:
            artikel_erfassen();
            break;
        case 7:
            vorhandene_artikel_ansehen();
            break;
        default:
            printf("\nFalsche Eingabe. Waehle eine der oben aufgelisteten Moeglichkeiten!\n");
        } // Ende des switch
    } // Ende der while-Schleife

    return 0;
}

int menue() { //hier wird das Menue ausgegeben
    bs_loeschen();
    printf("  Halle\t\t\tPorta Westfalica");
    printf("\n  %d Artikel\t\t%d Artikel", halle_lager.anzahl_artikel, porta_lager.anzahl_artikel); 
    printf("\n  Belegung:\t\tBelegung:");
    printf("\n  20cm: 40 %%            20cm: 5 %%");
    printf("\n  40cm: 14 %%            40cm: 51 %%");
    printf("\n  80cm: 79 %%");
    printf("\n");
    printf("\nWarenausgang");
    printf("\nHalle: 5 Artikel      Porta Westfalica: 6 Artikel");
    printf("\n");
    printf("\n(1)   Neuen Artikel anlegen");
    printf("\n(2)   Artikel bearbeiten");
    printf("\n(3)   Artikel loeschen");
    printf("\n(4)   Artikel erfassen");
    printf("\n(5)   Artikel entfernen");
    printf("\n(6)   Artikel umraeumen");
    printf("\n(7)   Alle Artikel ansehen");
    printf("\n(8)   Lagerbestand zufaellig befuellen");
    printf("\n(9)   Zufaellige Bestellung erzeugen");
    printf("\n(10)  Manuelle Bestellung erzeugen");
    printf("\n(11)  Erfasste Bestellungen versenden");
    printf("\n(12)  Erfasste Bestellungen abbrechen");
    printf("\n");
    printf("\nWaehle eine Auswahlmoeglichkeit:");
    printf("\n");

    return 0;
}

int neuen_artikel_anlegen() {
    struct ArtikelTyp artikeltyp;
    int janein = 0;
    int lagerwahl = 0;
    int wahl = 0;
    int i;

    bs_loeschen();
    printf("Neuen Artikel anlegen?");
    printf("\n(1) Ja");
    printf("\n(0) Nein");
    printf("\n");

    scanf("%d", &janein);

    if (janein == 0) {
        return 0;
    }

    bs_loeschen();
    printf("Neuen Artikel anlegen");

    printf("\n");
    printf("\nArtikelname (max. 100 Zeichen): ");
    // Leere den Eingabepuffer nach der vorherigen Eingabe
    while (getchar() != '\n');  // verhindert, dass leere Zeile eingelesen wird
    fgets(artikeltyp.name, sizeof(artikeltyp.name), stdin); // kann Leerzeichen/ganze Zeile einlesen
    // Entferne das Zeilenumbruchzeichen am Ende der Eingabe
    artikeltyp.name[strcspn(artikeltyp.name, "\n")] = '\0';

    strtrim(artikeltyp.name);   //löscht Leerzeichen hinter/vor Name
    // Überprüfen, ob der Artikelname bereits existiert
    for (i = 0; i < anzahl_artikel; i++) {
        if (strcmp(artikeltyp.name, artikel_liste[i].name) == 0) {
            printf("\nFehler: Ein Artikel mit diesem Namen existiert bereits. Druecke Enter, um zurueck in das Menue zu kommen.\n");
            while (getchar() != '\n');
            getchar();
            return 0; // Rückkehr, da Fehler aufgetreten ist
        }
    }

    printf("\nArtikel Nummer: ");
    scanf("%d", &artikeltyp.art_nummer);

    // Überprüfen, ob die Artikelnummer bereits existiert
    for (i = 0; i < anzahl_artikel; i++) {
        if (artikeltyp.art_nummer == artikel_liste[i].art_nummer) {
            printf("\nFehler: Artikel mit dieser Nummer existiert bereits. Druecke Enter, um zurueck in das Menue zu kommen.\n");
            while (getchar() != '\n');
            getchar();
            return 0; // Rückkehr, da Fehler aufgetreten ist
        }
    }


    printf("\nPreis (in EUR): ");
    scanf("%lf", &artikeltyp.preis);

    printf("\nHoehe (in cm): ");
    scanf("%lf", &artikeltyp.hoehe);

    printf("\nBreite (in cm): ");
    scanf("%lf", &artikeltyp.breite);

    printf("\nTiefe (in cm): ");
    scanf("%lf", &artikeltyp.tiefe);


    printf("\nWeisen Sie dem Artikel nun noch zu, ob er verderblich ist oder nicht.\nVerderblich: (1)\nNicht verderblich: (2)\n");
    // Eingabe für die Art des Artikels (verderblich oder nicht)
    scanf("%d", &lagerwahl);

    switch (lagerwahl) {
    case HALLE:
        // Artikel ist verderblich, Lagerort entsprechend festlegen und verarbeiten
        halle_lager.artikel_liste[halle_lager.anzahl_artikel] = artikeltyp;
        halle_lager.anzahl_artikel++;
        artikeltyp.lager = 1;
        printf("\nDer Artikel wurde erfolgreich in Halle an der Saale angelegt.\nDruecken Sie Enter, um fortzufahren.\n");
        break;
    case PORTA_WESTFALICA:
        // Artikel ist nicht verderblich, Lagerort entsprechend festlegen und verarbeiten
        porta_lager.artikel_liste[porta_lager.anzahl_artikel] = artikeltyp;
        porta_lager.anzahl_artikel++;
        artikeltyp.lager = 2;
        printf("\nDer Artikel wurde erfolgreich in Porta Westfalica angelegt.\nDruecken Sie Enter, um fortzufahren.\n");
        break;
    default:
        printf("\nUngueltige Eingabe. Bitte waehlen Sie eine der angegebenen Optionen.");
        break;
    }

    while (getchar() != '\n');
    getchar();

    bs_loeschen();

    // Anzeigen des erstellten Artikels so umbauen, dass User Inputs richtig ausgegeben werden
    // Vielleicht auch in artikel_anzeigen() Funktion bauen?
    printf("Artikel wurde erfolgreich angelegt. Druecke Enter, um zum Menue zu kommen!");
    printf("\n");
    printf("\nArtikel Name: %s", artikeltyp.name);
    printf("\nArtikel Nummer: %d", artikeltyp.art_nummer);
    printf("\nArtikel Preis: %.2lf", artikeltyp.preis);
    printf("\nArtikel Hoehe (in cm): %.2lf", artikeltyp.hoehe);
    printf("\nArtikel Breite (in cm): %.2lf", artikeltyp.breite);
    printf("\nArtikel Tiefe (in cm): %.2lf", artikeltyp.tiefe);
    // Lagerort anzeigen lassen

    // Hinzufügen des neuen Artikels zur Liste
    artikel_liste[anzahl_artikel] = artikeltyp;
    anzahl_artikel++;

    while (getchar() != '\n');
    getchar();

    return 0;
}



int artikel_bearbeiten() {
    int auswahl;
    char suchname[100];
    int suchnummer;
    int gefunden = 0;
    int i;

    bs_loeschen();
    printf("Artikel bearbeiten");
    printf("\n");
    printf("\nMoechten Sie den Artikel anhand der Artikelnummer (1) oder des Namens (2) suchen? ");
    scanf("%d", &auswahl);

    switch (auswahl) {
    case 1:
        printf("\nBitte geben Sie die Artikelnummer ein: ");
        scanf("%d", &suchnummer);
        for (i = 0; i < anzahl_artikel; i++) {
            if (artikel_liste[i].art_nummer == suchnummer) {
                // Artikel gefunden
                printf("\nArtikel gefunden:\n");
                printf("\nArtikel Name: %s", artikel_liste[i].name);
                printf("\nArtikel Nummer: %d", artikel_liste[i].art_nummer);
                printf("\nArtikel Preis: %.2lf", artikel_liste[i].preis);
                printf("\nArtikel Hoehe (in cm): %.2lf", artikel_liste[i].hoehe);
                printf("\nArtikel Breite (in cm): %.2lf", artikel_liste[i].breite);
                printf("\nArtikel Tiefe (in cm): %.2lf", artikel_liste[i].tiefe);
                printf("\n");

                // Code für Änderung Details des Artikels
                details_waehlen_bearbeitung(&artikel_liste[i]); // Details waehlen ausgelagert, um richtiges Speichern zu ermoeglichen und fuer Uebersichtlichkeit
                gefunden = 1;
                break;
            }
        }
        if (!gefunden) {
            printf("\nEin Artikel mit dieser Nummer wurde nicht gefunden.\nDruecken Sie Enter, um zum Menue zurueckzukehren!"); //vielleicht zu Auswahl Name oder Nummer suchen zurueckkehren??
            while (getchar() != '\n');
            getchar();
        }
        break;
    case 2:
        printf("\nBitte geben Sie den Artikelnamen ein: ");
        getchar(); // Leertaste nach vorheriger Eingabe abfangen
        fgets(suchname, sizeof(suchname), stdin); // Einlesen des Namens mit Leerzeichen
        suchname[strcspn(suchname, "\n")] = 0; // Entfernen des Zeilenumbruchs
        strtrim(suchname);
        for (i = 0; i < anzahl_artikel; i++) {
            if (strcmp(artikel_liste[i].name, suchname) == 0) {
                // Artikel gefunden
                printf("\nArtikel gefunden:\n");
                printf("\nArtikel Name: %s", artikel_liste[i].name);
                printf("\nArtikel Nummer: %d", artikel_liste[i].art_nummer);
                printf("\nArtikel Preis: %.2lf", artikel_liste[i].preis);
                printf("\nArtikel Hoehe (in cm): %.2lf", artikel_liste[i].hoehe);
                printf("\nArtikel Breite (in cm): %.2lf", artikel_liste[i].breite);
                printf("\nArtikel Tiefe (in cm): %.2lf", artikel_liste[i].tiefe);
                printf("\n");

                // Code für die Änderung Details Artikel
                details_waehlen_bearbeitung(&artikel_liste[i]); 
                gefunden = 1;
                break;
            }
        }
        if (!gefunden) {
            printf("\nEin Artikel mit diesem Namen wurde nicht gefunden.\nDruecken Sie Enter, um zum Menue zurueckzukehren!");  //vielleicht zu Auswahl Name oder Nummer suchen zurueckkehren??
            while (getchar() != '\n');
            getchar();
        }
        break;
    default:
        printf("\nUngueltige Auswahl.");
        break;
    }

    return 0;
}

int details_waehlen_bearbeitung(struct ArtikelTyp* artikel) {
    int weitere_aenderungen = 1;
    int auswahl;
    struct ArtikelTyp temp_artikel = *artikel; // Temporäre Kopie des Artikels
    int i;

    do {
        printf("\nWelches Detail moechten Sie bearbeiten?");
        printf("\n(1) Name");
        printf("\n(2) Artikelnummer");
        printf("\n(3) Preis");
        printf("\n(4) Hoehe");
        printf("\n(5) Breite");
        printf("\n(6) Tiefe");
        printf("\n");

        scanf("%d", &auswahl);
        switch (auswahl) {
        case 1:
            printf("\nBitte geben Sie den neuen Namen ein: ");
            getchar(); // Leertaste nach vorheriger Eingabe abfangen
            fgets(temp_artikel.name, sizeof(temp_artikel.name), stdin); // Einlesen des Namens mit Leerzeichen
            temp_artikel.name[strcspn(temp_artikel.name, "\n")] = 0; // Entfernen des Zeilenumbruchs
            strtrim(temp_artikel.name);
            // Überprüfung, ob der Name bereits existiert
            for (i = 0; i < anzahl_artikel; i++) {
                if (strcmp(temp_artikel.name, artikel_liste[i].name) == 0 && temp_artikel.art_nummer != artikel_liste[i].art_nummer) {
                    printf("\nFehler: Der eingegebene Name existiert bereits in einem anderen Artikel.\nDruecken Sie Enter, um zum Menue zurueckzukehren!");
                    while (getchar() != '\n');
                    getchar();
                    return 1;
                }
            }
            break;
        case 2:
            printf("\nBitte geben Sie die neue Artikelnummer ein: ");
            scanf("%d", &temp_artikel.art_nummer);
            // Überprüfung, ob die Artikelnummer bereits existiert
            for (i = 0; i < anzahl_artikel; i++) {
                if (temp_artikel.art_nummer == artikel_liste[i].art_nummer && strcmp(temp_artikel.name, artikel_liste[i].name) != 0) {
                    printf("\nFehler: Die eingegebene Artikelnummer existiert bereits.\nDruecken Sie Enter, um zum Menue zurueckzukehren!");
                    while (getchar() != '\n');
                    getchar();
                    return 1;
                }
            }
            break;
        case 3:
            printf("\nBitte geben Sie den neuen Preis ein: ");
            scanf("%lf", &temp_artikel.preis);
            break;
        case 4:
            printf("\nBitte geben Sie die neue Hoehe ein: ");
            scanf("%lf", &temp_artikel.hoehe);
            break;
        case 5:
            printf("\nBitte geben Sie die neue Breite ein: ");
            scanf("%lf", &temp_artikel.breite);
            break;
        case 6:
            printf("\nBitte geben Sie die neue Tiefe ein: ");
            scanf("%lf", &temp_artikel.tiefe);
            break;
        default:
            printf("\nUngueltige Auswahl.");
            break;
        }

        printf("\nArtikel Name: %s", temp_artikel.name);
        printf("\nArtikel Nummer: %d", temp_artikel.art_nummer);
        printf("\nArtikel Preis: %.2lf", temp_artikel.preis);
        printf("\nArtikel Hoehe (in cm): %.2lf", temp_artikel.hoehe);
        printf("\nArtikel Breite (in cm): %.2lf", temp_artikel.breite);
        printf("\nArtikel Tiefe (in cm): %.2lf", temp_artikel.tiefe);
        printf("\n");

        printf("\nMoechten Sie weitere Details bearbeiten? (1 = Ja, 0 = Nein): ");
        scanf("%d", &weitere_aenderungen);
    } while (weitere_aenderungen);

    printf("\nMoechten Sie diese Aenderungen speichern? (1 = Ja, 0 = Nein): ");
    scanf("%d", &auswahl);
    if (auswahl == 1) {
        printf("\nDie Aenderungen wurden erfolgreich gespeichert.\nDruecken Sie Enter, um zum Menue zurueckzukehren!");
        while (getchar() != '\n');
        getchar();
        *artikel = temp_artikel; // Aktualisierung des Originalartikels mit den Änderungen aus der temporären Kopie
        lager_aktualisieren(artikel); // Funktion zur Aktualisierung der Änderungen im Lager 
    }

    return 0;
}

// Funktion zum Anzeigen der vorhandenen Artikel und gleichzeitigen Aktualisierung des Lagerbestands
int vorhandene_artikel_ansehen() {
    int lagerwahl = 0;
    int i;

    bs_loeschen();
    printf("Welches Lager moechten Sie anzeigen?");
    printf("\n(1) HALLE Lager");
    printf("\n(2) PORTA WESTFALICA Lager");
    printf("\n");

    scanf("%d", &lagerwahl);

    switch (lagerwahl) {
    case HALLE:
        printf("\nArtikel im HALLE Lager:\n\n");
        for (i = 0; i < halle_lager.anzahl_artikel; i++) {
            struct ArtikelTyp artikeltyp = halle_lager.artikel_liste[i];
            printf("\nArtikel Name: %s", artikeltyp.name);
            printf("\nArtikel Nummer: %d", artikeltyp.art_nummer);
            printf("\nArtikel Preis: %.2lf", artikeltyp.preis);
            printf("\nArtikel Hoehe (in cm): %.2lf", artikeltyp.hoehe);
            printf("\nArtikel Breite (in cm): %.2lf", artikeltyp.breite);
            printf("\nArtikel Tiefe (in cm): %.2lf", artikeltyp.tiefe);
            printf("\n");

            
        }
        break;
    case PORTA_WESTFALICA:
        printf("\nArtikel im PORTA WESTFALICA Lager:\n\n");
        for (i = 0; i < porta_lager.anzahl_artikel; i++) {
            struct ArtikelTyp artikeltyp = porta_lager.artikel_liste[i];
            printf("\nArtikel Name: %s", artikeltyp.name);
            printf("\nArtikel Nummer: %d", artikeltyp.art_nummer);
            printf("\nArtikel Preis: %.2lf", artikeltyp.preis);
            printf("\nArtikel Hoehe (in cm): %.2lf", artikeltyp.hoehe);
            printf("\nArtikel Breite (in cm): %.2lf", artikeltyp.breite);
            printf("\nArtikel Tiefe (in cm): %.2lf", artikeltyp.tiefe);
            printf("\n");
        }
        break;
    default:
        printf("\nUngueltige Eingabe. Bitte waehlen Sie eine der angegebenen Optionen.");
        break;
    }

    printf("\n\nDruecke Enter, um zum Hauptmenue zurueckzukehren.");
    while (getchar() != '\n');
    getchar();
}

// Aktualisierung der Änderungen am Artikel im Lager
int lager_aktualisieren(struct ArtikelTyp *artikel) {
    int i;
    for (i = 0; i < anzahl_artikel; i++) {
        if (halle_lager.artikel_liste[i].art_nummer == artikel->art_nummer) {
            halle_lager.artikel_liste[i] = *artikel;
            break;
        }
    }

    for (i = 0; i < anzahl_artikel; i++) {
        if (porta_lager.artikel_liste[i].art_nummer == artikel->art_nummer) {
            porta_lager.artikel_liste[i] = *artikel;
            break;
        }
    }
}


void bs_loeschen(void){
    system("CLS");   // MS-DOS-Kommando
}


// Funktion zum Suchen, Anzeigen und einlagern (dazu ists noch nicht gekommen) des Artikels
int artikel_erfassen() {
    int gefunden = 0;
    int eingabeNummer;
    char eingabeName[100];
    char wahl;
    int i;


    do {
        gefunden = 0; // Zurücksetzen des gefunden-Flags

        bs_loeschen();

        printf("Geben Sie die Artikelnummer oder den Artikelnamen ein: ");
        if (scanf("%d", &eingabeNummer) == 1) { // Eingabe als Nummer
            for (i = 0; i < anzahl_artikel; i++) {
                if (artikel_liste[i].art_nummer == eingabeNummer) {
                    gefunden = 1;
                    printf("\nArtikel gefunden:");
                    printf("\nName: %s", artikel_liste[i].name);
                    printf("\nArtikelnummer: %d", artikel_liste[i].art_nummer);
                    printf("\nPreis (in EUR): %.2lf", artikel_liste[i].preis);
                    printf("\nHoehe (in cm): %.2lf", artikel_liste[i].hoehe);
                    printf("\nBreite (in cm): %.2lf", artikel_liste[i].breite);
                    printf("\nTiefe (in cm): %.2lf", artikel_liste[i].tiefe);
                    printf("\nLager: %d", artikel_liste[i].lager);
                    printf("\nMoechten Sie zum Menue zurueckkehren (0), nach einem anderen Artikel suchen (1) oder den Artikel einlagern (2)?: ");
                    scanf(" %c", &wahl);
                    if (wahl == '0') {
                        printf("\nZurueck zum Menue...");
                        return 0;
                    }
                    else if (wahl == '2') {
                        struct Artikel artikel;
                        artikel.typ = artikel_liste[i];
                        artikel.inventarnummer = generiere_inventarnummer(); // Hier wird die Inventarnummer generiert und zugewiesen

                        printf("Generierte Inventarnummer: %d\n", artikel.inventarnummer); // Hier wird die generierte Inventarnummer ausgegeben

                        artikel_einlagern_nach_nummer(eingabeNummer, artikel_liste[i].lager, artikel);
                        break;
                    }
                }
            }
        }
        else { // Eingabe als Name
            getchar(); 
            fgets(eingabeName, sizeof(eingabeName), stdin); // Einlesen mit Leerzeichen moeglich/ganze Zeile
            eingabeName[strcspn(eingabeName, "\n")] = 0; // Entfernen der Zeilenumbruchzeichen (\n)
            strtrim(eingabeName); // Leerzeichen am Anfang und Ende entfernen

            for (i = 0; i < anzahl_artikel; i++) {
                if (strcmp(artikel_liste[i].name, eingabeName) == 0) {
                    gefunden = 1;
                    printf("\nArtikel gefunden:");
                    printf("\nName: %s", artikel_liste[i].name);
                    printf("\nNummer: %d", artikel_liste[i].art_nummer);
                    printf("\nPreis (in EUR): %.2lf", artikel_liste[i].preis);
                    printf("\nHoehe (in cm): %.2lf", artikel_liste[i].hoehe);
                    printf("\nBreite (in cm): %.2lf", artikel_liste[i].breite);
                    printf("\nTiefe (in cm): %.2lf", artikel_liste[i].tiefe);
                    printf("\nLager: %d", artikel_liste[i].lager);
                    printf("\nMoechten Sie zum Menue zurueckkehren (0), nach einem anderen Artikel suchen (1) oder den Artikel einlagern (2)?: ");
                    scanf(" %c", &wahl);
                    if (wahl == '0') {
                        printf("\nZurueck zum Menue...");
                        return 0;
                    }
                    else if (wahl == '2') {
                        //artikel_einlagern_nach_name(eingabeName, artikel_liste[i].lager);
                        return 1;
                    }
                    break;
                }
            }
        }

        if (!gefunden) {
            printf("\nArtikel nicht gefunden.\n");
        }
        printf("Moechten Sie zum Menue zurueckkehren (0) oder einen neuen Artikel suchen (1) ?: ");
        scanf(" %c", &wahl);
    } while (wahl != '0');

    return 0;
}

int generiere_inventarnummer() {
    // Inkrementiere den Zähler und gib die neue Inventarnummer zurück
    return ++inventarnummerZaehler;
}

int artikel_einlagern_nach_nummer(int eingabeNummer, int lager, struct Artikel artikel) {
    // Überprüfen des Lagers und Berechnungen, vielleicht als separate Funktionen nochmal
    if (lager == 1) {
        // Aktionen für Lager 1 (HALLE)
        int belegte_ids = berechne_belegte_ids_Halle(artikel);
        if (belegte_ids != -1) {
            print_zugeordnete_ids(artikel); // Ausgabe der zugeordneten IDs
            printf("Artikel mit der Nummer %d wurde erfolgreich im Halle Lager eingelagert.\n", eingabeNummer);
        }
        else {
            printf("Artikel mit der Nummer %d konnte nicht eingelagert werden. Keine passende Position gefunden.\n", eingabeNummer);
        }
        printf("Druecken Sie Enter, um weitere Aktionen auszufuehren!\n");
        while (getchar() != '\n');
        getchar();
    }
    else if (lager == 2) {
        // Aktionen für Lager 2 (PW)
        int belegte_ids = berechne_belegte_ids_Porta(artikel, 0); // Start-Zaehlerwert
        printf("Artikel mit der Nummer %d wird im Porta Westfalica Lager eingelagert.\nDruecken Sie Enter, um weitere Aktionen auszufuehren!\n", eingabeNummer);
        while (getchar() != '\n');
        getchar();
    }
    else {
        // Fehlermeldung 
        printf("Unbekanntes Lager für Artikel mit der Nummer %d.\nDruecken Sie Enter, um weitere Aktionen auszufuehren!\n", eingabeNummer);
        while (getchar() != '\n');
        getchar();
    }

    return 1;
}

/*int artikel_einlagern_nach_name(char eingabeName[], int lager) {
    // Überprüfen des Lagers und dann Berechnungen
    if (lager == 1) {
        // Aktionen für Lager 1 (HALLE)
        printf("Artikel mit dem Namen %s wird im Halle Lager eingelagert.\nDruecken Sie Enter, um weitere Aktionen auszufuehren!\n", eingabeName);
        while (getchar() != '\n');
        getchar();
    }
    else if (lager == 2) {
        // Aktionen für Lager 2 (PW) einzeln, weil es noch 80cm gibt
        printf("Artikel mit dem Namen %s wird im Porta Westfalica Lager eingelagert.\nDruecken Sie Enter, um weitere Aktionen auszufuehren!\n", eingabeName);
        while (getchar() != '\n');
        getchar();
    }
    else {
        // Fehlermeldung, mehr oder weniger sinnlos
        printf("Unbekanntes Lager fuer Artikel mit dem Namen %s.\nDruecken Sie Enter, um weitere Aktionen auszufuehren!\n", eingabeName);
        while (getchar() != '\n');
        getchar();
    }

    return 1;
}*/

int berechne_belegte_ids_Halle(struct Artikel artikel) {
    int start_id;
    int i;
    int max_anzahl_halle_20 = HALLE_20 + 12000000;
    int max_anzahl_halle_40 = HALLE_40 + 14000000;

    // Zugriff auf Daten des Artikels
    double hoehe = artikel.typ.hoehe;
    double breite = artikel.typ.breite;
    double tiefe = artikel.typ.tiefe;
    int artikelnummer = artikel.typ.art_nummer; 

    int ids_belegt = 0; // Anzahl der belegten IDs


    if (hoehe <= 20) {
        start_id = START_ID_HALLE_20CM;

        // Berechnung der Anzahl der belegten IDs basierend auf Breite und Tiefe
        ids_belegt = (int)(breite / 10); // Breite belegt breite/10 IDs
        if (tiefe > 60) {
            ids_belegt *= 2; // Tiefe > 60 belegt das doppelte an IDs
        }

        // Schleife für 20 cm Fächer
        for (start_id; start_id < max_anzahl_halle_20; start_id++) {
            int available = 1;
            for (i = 0; i < ids_belegt; i++) {
                if (start_id + i >= max_anzahl_halle_20 || artikel.positions[start_id + i].artikelnummer != 0 || artikel.positions[start_id + i].positions_id_voll == 1) {
                    available = 0;
                    break;
                }
            }

            if (available) {
                for (i = 0; i < ids_belegt; i++) {
                    artikel.positions[start_id + i].resthöhe -= hoehe;
                    if (artikel.positions[start_id + i].resthöhe <= 5) {
                        artikel.positions[start_id + i].positions_id_voll = 1;
                    }
                    artikel.positions[start_id + i].artikelnummer = artikelnummer;
                }
                return start_id; // Rückgabe der berechneten ID
            }
            else {
                start_id += i; // Weiter mit der nächsten verfügbaren ID
            }
        }
    }
    else if ((hoehe > 20) && (hoehe <= 40)) {   // oder wenn alle posIDs halle_20 voll sind ?? !
        start_id = START_ID_HALLE_40CM;

        // Schleife für 40 cm Fächer
        for (start_id; start_id < max_anzahl_halle_40; start_id++) {
            if (artikel.positions[start_id].artikelnummer != 0 || artikel.positions[start_id].positions_id_voll == 1) {
                continue; // Überspringe belegte IDs
            }

            if (artikel.positions[start_id].resthöhe >= hoehe) {
                artikel.positions[start_id].resthöhe -= hoehe;
                if (artikel.positions[start_id].resthöhe <= 5) {
                    artikel.positions[start_id].positions_id_voll = 1;
                }
                artikel.positions[start_id].artikelnummer = artikelnummer;
                return start_id; // Rückgabe der berechneten ID
            }
        }
    }

    // Rückgabe eines ungültigen Wertes, falls keine passende Position gefunden wurde
    return -1;
}

berechne_belegte_ids_Porta() {
    int start_id = 0;
    int max_anzahl_porta_20 = PORTA_20 + 22000000;
    int max_anzahl_porta_40 = PORTA_40 + 2400000;
    int max_anzahl_porta_80 = PORTA_80 + 2800000;

}

// Funktion zur Ausgabe aller zugeordneten IDs
void print_zugeordnete_ids(struct Artikel artikel) {    // aktuell werden noch falsche IDs ausgegeben, da Startwert 0 ist !!
    int i;
    printf("Zugeordnete IDs fuer Artikelnummer %d:\n", artikel.typ.art_nummer);
    for (i = 0; i < artikel.num_positions; i++) {
        printf("ID: %d\n", artikel.positions[i].id);
    }
}

void strtrim(char* str) {
    int start = 0, end;

    // Leerzeichen am Anfang entfernen
    while (isspace(str[start])) {
        start++;
    }

    // Wenn die Zeichenkette nur aus Leerzeichen besteht
    if (str[start] == '\0') {
        *str = '\0';
        return;
    }

    // Leerzeichen am Ende finden
    end = strlen(str) - 1;
    while (end >= start && isspace(str[end])) {
        end--;
    }

    // Nullterminator setzen
    str[end + 1] = '\0';

    // Zeichenkette verschieben, wenn nötig
    if (start > 0) {
        memmove(str, str + start, end - start + 2);
    }
}