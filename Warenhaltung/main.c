#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HALLE_20 18000 //vorhandene Anzahl Positions IDs für 20cm hohe Fächer
#define HALLE_40 54000
#define PORTA_20 34800
#define PORTA_40 6000
#define PORTA_80 7800

// Maximale Anzahl von Artikeln
#define MAX_ARTIKEL 400

int menue();
int neuen_artikel_anlegen();
int artikel_bearbeiten();
int artikel_erfassen();
int artikel_einlagern_nach_nummer(int eingabeNummer, int lager);
int artikel_einlagern_nach_name(char eingabeName[], int lager);
int vorhandene_artikel_ansehen();
int lager_aktualisieren(int);
void bs_loeschen();


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

struct PositionsID {
    int id;
    int resthöhe;
    int positions_id_voll;
};


int belegte_id_halle_20[18000] = [2000001, 2000002, 2000003, ..., 2000245];

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

struct ArtikelTyp {
    char name[100];
    int art_nummer;
    double preis;
    double hoehe;
    double breite;
    double tiefe;
    int lager;
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

/*
struct Artikel {
    int artikel_typ_nummer = 12345
    int inventarnummer = 9785
    int positions_id = [3, 4]
};

*/

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
    scanf("%s", artikeltyp.name);

    // Überprüfen, ob der Artikelname bereits existiert
    for (int i = 0; i < anzahl_artikel; i++) {
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
    for (int i = 0; i < anzahl_artikel; i++) {
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
        printf("\nDer Artikel wurde erfolgreich in Halle an der Saale eingelagert.\nDruecken Sie Enter, um fortzufahren.\n");
        break;
    case PORTA_WESTFALICA:
        // Artikel ist nicht verderblich, Lagerort entsprechend festlegen und verarbeiten
        porta_lager.artikel_liste[porta_lager.anzahl_artikel] = artikeltyp;
        porta_lager.anzahl_artikel++;
        artikeltyp.lager = 2;
        printf("\nDer Artikel wurde erfolgreich in Porta Westfalica eingelagert.\nDruecken Sie Enter, um fortzufahren.\n");
        break;
    default:
        printf("\nUngueltige Eingabe. Bitte waehlen Sie eine der angegebenen Optionen.");
        break;
    }

    while (getchar() != '\n');
    getchar();

    bs_loeschen();

    // anzeigen des erstellten Artikels so umbauen, dass User Inputs richtig ausgegeben werden
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
    int weitere_aenderungen = 0;

    bs_loeschen();
    printf("Artikel bearbeiten");
    printf("\n");
    printf("\nMoechten Sie den Artikel anhand der Artikelnummer (1) oder des Namens (2) suchen? ");
    scanf("%d", &auswahl);

    switch (auswahl) {
    case 1:
        printf("\nBitte geben Sie die Artikelnummer ein: ");
        scanf("%d", &suchnummer);
        for (int i = 0; i < anzahl_artikel; i++) {
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
                        scanf("%s", artikel_liste[i].name);
                        // Überprüfen, ob der neue Name bereits existiert
                        for (int j = 0; j < anzahl_artikel; j++) {
                            if (j != i && strcmp(artikel_liste[j].name, artikel_liste[i].name) == 0) {
                                printf("\nFehler: Ein Artikel mit diesem Namen existiert bereits. Druecken Sie Enter um zum Menue zurueck zu kommen!\n");
                                strcpy(artikel_liste[i].name, artikel_liste[j].name); // Ursprünglichen Namen wiederherstellen
                                while (getchar() != '\n');
                                getchar();
                                return 0; // Rückkehr, da Fehler aufgetreten ist
                            }
                        }
                        break;
                    case 2:
                        printf("\nBitte geben Sie die neue Artikelnummer ein: ");
                        scanf("%d", &artikel_liste[i].art_nummer);
                        for (int j = 0; j < anzahl_artikel; j++) {
                            if (j != i && artikel_liste[j].art_nummer == artikel_liste[i].art_nummer) {
                                printf("\nFehler: Ein Artikel mit dieser Artikelnummer existiert bereits. Druecken Sie Enter um zum Menue zurueck zu kommen!\n");
                                strcpy(artikel_liste[i].art_nummer, artikel_liste[j].art_nummer); // Ursprünglichen Namen wiederherstellen
                                while (getchar() != '\n');
                                getchar();
                                return 0; // Rückkehr, da Fehler aufgetreten ist
                            }
                        }
                        break;
                    case 3:
                        printf("\nBitte geben Sie den neuen Preis ein: ");
                        scanf("%lf", &artikel_liste[i].preis);
                        break;
                    case 4:
                        printf("\nBitte geben Sie die neue Hoehe ein: ");
                        scanf("%lf", &artikel_liste[i].hoehe);
                        break;
                    case 5:
                        printf("\nBitte geben Sie die neue Breite ein: ");
                        scanf("%lf", &artikel_liste[i].breite);
                        break;
                    case 6:
                        printf("\nBitte geben Sie die neue Tiefe ein: ");
                        scanf("%lf", &artikel_liste[i].tiefe);
                        break;
                    default:
                        printf("\nUngueltige Auswahl.");
                        break;
                    }
                    printf("\nArtikel Name: %s", artikel_liste[i].name);
                    printf("\nArtikel Nummer: %d", artikel_liste[i].art_nummer);
                    printf("\nArtikel Preis: %.2lf", artikel_liste[i].preis);
                    printf("\nArtikel Hoehe (in cm): %.2lf", artikel_liste[i].hoehe);
                    printf("\nArtikel Breite (in cm): %.2lf", artikel_liste[i].breite);
                    printf("\nArtikel Tiefe (in cm): %.2lf", artikel_liste[i].tiefe);
                    printf("\n");

                    printf("\nMoechten Sie weitere Details bearbeiten? (1 = Ja, 0 = Nein): ");
                    scanf("%d", &weitere_aenderungen);
                } while (weitere_aenderungen);

                lager_aktualisieren(i);

                printf("\nSind Sie sicher, dass Sie diese Aenderungen speichern moechten? (1 = Ja, 0 = Nein): ");
                scanf("%d", &auswahl);
                if (auswahl == 1) {
                    printf("\nDie Aenderungen wurden erfolgreich gespeichert.");
                }
                else {
                    printf("\nDie Aenderungen wurden abgebrochen.");
                }

                gefunden = 1;
                break;
            }
        }
        if (!gefunden) {
            printf("\nArtikel mit dieser Nummer wurde nicht gefunden.");
        }
        break;
    case 2:
        printf("\nBitte geben Sie den Artikelnamen ein: ");
        scanf("%s", suchname);
        for (int i = 0; i < anzahl_artikel; i++) {
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
                        scanf("%s", artikel_liste[i].name);
                        // Überprüfen, ob der neue Name bereits existiert
                        for (int j = 0; j < anzahl_artikel; j++) {
                            if (j != i && strcmp(artikel_liste[j].name, artikel_liste[i].name) == 0) {
                                printf("\nFehler: Ein Artikel mit diesem Namen existiert bereits. Druecken Sie Enter um zum Menue zurueck zu kommen!\n");
                                strcpy(artikel_liste[i].name, artikel_liste[j].name); // Ursprünglichen Namen wiederherstellen
                                while (getchar() != '\n');
                                getchar();
                                return 0; // Rückkehr, da Fehler aufgetreten ist
                            }
                        }
                        break;
                    case 2:
                        printf("\nBitte geben Sie die neue Artikelnummer ein: ");
                        scanf("%d", &artikel_liste[i].art_nummer);
                        for (int j = 0; j < anzahl_artikel; j++) {
                            if (j != i && artikel_liste[j].art_nummer == artikel_liste[i].art_nummer) {
                                printf("\nFehler: Ein Artikel mit dieser Artikelnummer existiert bereits. Druecken Sie Enter um zum Menue zurueck zu kommen!\n");
                                strcpy(artikel_liste[i].art_nummer, artikel_liste[j].art_nummer); // Ursprünglichen Namen wiederherstellen
                                while (getchar() != '\n');
                                getchar();
                                return 0; // Rückkehr, da Fehler aufgetreten ist
                            }
                        }
                        break;
                    case 3:
                        printf("\nBitte geben Sie den neuen Preis ein: ");
                        scanf("%lf", &artikel_liste[i].preis);
                        break;
                    case 4:
                        printf("\nBitte geben Sie die neue Hoehe ein: ");
                        scanf("%lf", &artikel_liste[i].hoehe);
                        break;
                    case 5:
                        printf("\nBitte geben Sie die neue Breite ein: ");
                        scanf("%lf", &artikel_liste[i].breite);
                        break;
                    case 6:
                        printf("\nBitte geben Sie die neue Tiefe ein: ");
                        scanf("%lf", &artikel_liste[i].tiefe);
                        break;
                    default:
                        printf("\nUngueltige Auswahl.");
                        break;
                    }
                    printf("\nArtikel Name: %s", artikel_liste[i].name);
                    printf("\nArtikel Nummer: %d", artikel_liste[i].art_nummer);
                    printf("\nArtikel Preis: %.2lf", artikel_liste[i].preis);
                    printf("\nArtikel Hoehe (in cm): %.2lf", artikel_liste[i].hoehe);
                    printf("\nArtikel Breite (in cm): %.2lf", artikel_liste[i].breite);
                    printf("\nArtikel Tiefe (in cm): %.2lf", artikel_liste[i].tiefe);
                    printf("\n");

                    printf("\nMoechten Sie weitere Details bearbeiten? (1 = Ja, 0 = Nein): ");
                    scanf("%d", &weitere_aenderungen);
                } while (weitere_aenderungen);

                lager_aktualisieren(i);

                printf("\nSind Sie sicher, dass Sie diese Aenderungen speichern moechten? (1 = Ja, 0 = Nein): ");
                scanf("%d", &auswahl);
                if (auswahl == 1) {
                    printf("\nDie Aenderungen wurden erfolgreich gespeichert.");
                }
                else {
                    printf("\nDie Aenderungen wurden abgebrochen.");
                }

                gefunden = 1;
                break;
            }
        }
        if (!gefunden) {
            printf("\nArtikel mit diesem Namen wurde nicht gefunden.");
        }
        break;
    default:
        printf("\nUngueltige Auswahl.");
        break;
    }

    return 0;
}

// Funktion zum Anzeigen der vorhandenen Artikel und gleichzeitigen Aktualisierung des Lagerbestands
int vorhandene_artikel_ansehen() {
    int lagerwahl = 0;

    bs_loeschen();
    printf("Welches Lager moechten Sie anzeigen?");
    printf("\n(1) HALLE Lager");
    printf("\n(2) PORTA WESTFALICA Lager");
    printf("\n");

    scanf("%d", &lagerwahl);

    switch (lagerwahl) {
    case HALLE:
        printf("\nArtikel im HALLE Lager:\n\n");
        for (int i = 0; i < halle_lager.anzahl_artikel; i++) {
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
        for (int i = 0; i < porta_lager.anzahl_artikel; i++) {
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
int lager_aktualisieren(int i) {
    for (int j = 0; j < anzahl_artikel; j++) {
        if (artikel_liste[j].art_nummer == artikel_liste[i].art_nummer) {
            halle_lager.artikel_liste[i] = artikel_liste[j];
            break;
        }
    }

    for (int j = 0; j < anzahl_artikel; j++) {
        if (artikel_liste[j].art_nummer == artikel_liste[i].art_nummer) {
            porta_lager.artikel_liste[i] = artikel_liste[j];
            break;
        }
    }
}

void bs_loeschen(void)
{
    system("CLS");   // MS-DOS-Kommando
}


// Funktion zum Suchen, Anzeigen und einlagern (dazu ists noch nicht gekommen) des Artikels
int artikel_erfassen() {
    int gefunden = 0;
    int eingabeNummer;
    char eingabeName[100];

    do {
        bs_loeschen();

        printf("Geben Sie die Artikelnummer oder den Artikelnamen ein: ");
        if (scanf("%d", &eingabeNummer) == 1) { // Eingabe als Nummer
            for (int i = 0; i < sizeof(artikel_liste) / sizeof(artikel_liste[0]); i++) {
                if (artikel_liste[i].art_nummer == eingabeNummer) {
                    gefunden = 1;
                    printf("\nArtikel gefunden:\n");
                    printf("Name: %s\n", artikel_liste[i].name);
                    printf("Artikelnummer: %d\n", artikel_liste[i].art_nummer);
                    printf("Preis (in EUR): %.2lf\n", artikel_liste[i].preis);
                    printf("Hoehe (in cm): %.2lf\n", artikel_liste[i].hoehe);
                    printf("Breite (in cm): %.2lf\n", artikel_liste[i].breite);
                    printf("Tiefe (in cm): %.2lf\n", artikel_liste[i].tiefe);
                    printf("Lager: %d\n", artikel_liste[i].lager);
                    char antwort;
                    printf("\nMoechten Sie diesen Artikel einlagern? (j/n): ");
                    scanf(" %c", &antwort);
                    if (antwort == 'j' || antwort == 'J') {
                        artikel_einlagern_nach_nummer(eingabeNummer, artikel_liste[i].lager);
                    }
                    break;
                }
            }
        }
        else { // Eingabe als Name
            scanf("%s", eingabeName);
            for (int i = 0; i < sizeof(artikel_liste) / sizeof(artikel_liste[0]); i++) {
                if (strcmp(artikel_liste[i].name, eingabeName) == 0) {
                    gefunden = 1;
                    printf("\nArtikel gefunden:\n");
                    printf("Name: %s\n", artikel_liste[i].name);
                    printf("Nummer: %d\n", artikel_liste[i].art_nummer);
                    printf("Preis (in EUR): %.2lf\n", artikel_liste[i].preis);
                    printf("Hoehe (in cm): %.2lf\n", artikel_liste[i].hoehe);
                    printf("Breite (in cm): %.2lf\n", artikel_liste[i].breite);
                    printf("Tiefe (in cm): %.2lf\n", artikel_liste[i].tiefe);
                    printf("Lager: %d\n", artikel_liste[i].lager);
                    char antwort;
                    printf("\nMoechten Sie diesen Artikel einlagern? (j/n): ");
                    scanf(" %c", &antwort);
                    if (antwort == 'j' || antwort == 'J') {
                        artikel_einlagern_nach_name(eingabeName, artikel_liste[i].lager);
                    }
                    break;
                }
            }
        }

        if (!gefunden) {
            printf("\nArtikel nicht gefunden. Druecken Sie Enter, um einen neuen Artikel zu suchen!\n");
            while (getchar() != '\n');
            getchar();
        }
        else {
            char wahl;
            printf("Moechten Sie zum Menue zurueckkehren (0) oder nach einem anderen Artikel suchen (1) ?: ");
            scanf(" %c", &wahl);
            if (wahl == '0') {
                printf("Zurueck zum Menü...\n");
                break;
            }
            else {
                printf("Suche nach einem anderen Artikel...\n");
            }
        }
    } while (1);

    return 0;
}

int artikel_einlagern_nach_nummer(int eingabeNummer, int lager) {
    // Überprüfen des Lagers und Berechnungen, vielleicht als separate Funktionen nochmal
    if (lager == 1) {
        // Aktionen für Lager 1 (HALLE)
        printf("Artikel mit der Nummer %d wird im Halle Lager eingelagert.\n", eingabeNummer);
    }
    else if (lager == 2) {
        // Aktionen für Lager 2 (PW)
        printf("Artikel mit der Nummer %d wird im Porta Westfalica Lager eingelagert.\n", eingabeNummer);
    }
    else {
        // Fehlermeldung 
        printf("Unbekanntes Lager für Artikel mit der Nummer %d.\n", eingabeNummer);
    }

    return 0;
}

int artikel_einlagern_nach_name(char eingabeName[], int lager) {
    // Überprüfen des Lagers und dann Berechnungen
    if (lager == 1) {
        // Aktionen für Lager 1 (HALLE)
        printf("Artikel mit dem Namen %s wird im Halle Lager eingelagert.\n", eingabeName);
    }
    else if (lager == 2) {
        // Aktionen für Lager 2 (PW) einzeln, weil es noch 80cm gibt
        printf("Artikel mit dem Namen %s wird im Porta Westfalica Lager eingelagert.\n", eingabeName);
    }
    else {
        // Fehlermeldung, mehr oder weniger sinnlos
        printf("Unbekanntes Lager fuer Artikel mit dem Namen %s.\n", eingabeName);
    }

    return 0;
}