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
int vorhandene_artikel_ansehen();
void bs_loeschen();



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

if (hohe <= 20) {
    int id = 2000001;
    max_anzahl = HALLE_20 + 2000000;
    anzahl_belegter_ids = breite / 10;
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
    enum LagerTyp lager;
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


// Funktion zur Aktualisierung der Lagerliste basierend auf den Artikeländerungen
void lager_aktualisieren() {
    // Durchlaufen Sie alle Artikel im Hauptartikelarray
    for (int i = 0; i < anzahl_artikel; i++) {
        // Überprüfen Sie den Lagertyp des aktuellen Artikels und aktualisieren Sie die entsprechende Lagerliste
        if (artikel_liste[i].lager == HALLE) {
            // Durchlaufen Sie die Artikel im HALLE Lager
            for (int j = 0; j < halle_lager.anzahl_artikel; j++) {
                // Überprüfen Sie, ob die Artikelnummer übereinstimmt und aktualisieren Sie die entsprechenden Informationen
                if (halle_lager.artikel_liste[j].art_nummer == artikel_liste[i].art_nummer) {
                    // Aktualisieren Sie die Artikelinformationen im HALLE Lager
                    halle_lager.artikel_liste[j] = artikel_liste[i];
                    break;
                }
            }
        }
        else if (artikel_liste[i].lager == PORTA_WESTFALICA) {
            // Durchlaufen Sie die Artikel im PORTA WESTFALICA Lager
            for (int j = 0; j < porta_lager.anzahl_artikel; j++) {
                // Überprüfen Sie, ob die Artikelnummer übereinstimmt und aktualisieren Sie die entsprechenden Informationen
                if (porta_lager.artikel_liste[j].art_nummer == artikel_liste[i].art_nummer) {
                    // Aktualisieren Sie die Artikelinformationen im PORTA WESTFALICA Lager
                    porta_lager.artikel_liste[j] = artikel_liste[i];
                    break;
                }
            }
        }
    }
}


// Funktion zur Überprüfung der Artikel
int artikel_existiert_bereits(const char* name, int art_nummer) {
    int i;
    for (i = 0; i < anzahl_artikel; i++) {
        if (strcmp(artikel_liste[i].name, name) == 0 && artikel_liste[i].art_nummer == art_nummer) {
            return 1; // Artikel bereits vorhanden
        }
    }
    return 0; // Artikel nicht vorhanden
}


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
        case 7:
            vorhandene_artikel_ansehen();
            break;
        default:
            printf("\nFalsche eingabe. Waehle eine der oben aufgelisteten Moeglichkeiten!\n");
        } // Ende des switch
    } // Ende der while-Schleife


    // Initialisierung der Lager
    halle_lager.anzahl_artikel = 0;
    porta_lager.anzahl_artikel = 0;

    return 0;
}

int menue() { //hier wird das Menue ausgegeben
    bs_loeschen();
    printf("Halle                 Porta Westfalica");
    printf("\nArtikel:              Artikel:");
    printf("\nBelegung:             Belegung:");
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
    printf("\nArtikelname (max. 100 Zeichen):");

    // input für artikelname auslesen
    scanf("%s", artikeltyp.name);

    // Überprüfen, ob der Artikelname bereits existiert
    for (int i = 0; i < anzahl_artikel; i++) {
        if (strcmp(artikeltyp.name, artikel_liste[i].name) == 0) {
            printf("\nFehler: Ein Artikel mit diesem Namen existiert bereits.\n");
            return 0; // Rückkehr, da Fehler aufgetreten ist
        }
    }

    printf("\nArtikel Nummer:");
    // input für artikel nummer auslesen
    scanf("%d", &artikeltyp.art_nummer);

    // Überprüfen, ob die Artikelnummer bereits existiert
    for (int i = 0; i < anzahl_artikel; i++) {
        if (artikeltyp.art_nummer == artikel_liste[i].art_nummer) {
            printf("\nFehler: Artikel mit dieser Nummer existiert bereits.\n");
            return 0; // Rückkehr, da Fehler aufgetreten ist
        }
    }


    printf("\nPreis (in EUR):");
    // input für artikel preis auslesen
    scanf("%lf", &artikeltyp.preis);

    printf("\nHoehe (in cm):");
    // input für hoehe auslesen
    scanf("%lf", &artikeltyp.hoehe);

    printf("\nBreite (in cm):");
    // input für breite auslesen
    scanf("%lf", &artikeltyp.breite);

    printf("\nTiefe (in cm):");
    // input für tiefe auslesen
    scanf("%lf", &artikeltyp.tiefe);


    printf("\nWeisen Sie dem Artikel nun noch zu, ob er verderblich ist oder nicht.\nVerderblich: (1)\nNicht verderblich: (2)");
    // Eingabe für die Art des Artikels (verderblich oder nicht)
    scanf("%d", &lagerwahl);

    switch (lagerwahl) {
    case HALLE:
        // Artikel ist verderblich, Lagerort entsprechend festlegen und verarbeiten
        halle_lager.artikel_liste[halle_lager.anzahl_artikel] = artikeltyp;
        halle_lager.anzahl_artikel++;
        printf("\nDer Artikel wurde erfolgreich in Halle an der Saale eingelagert.\nDruecken Sie Enter, um fortzufahren.\n");
        break;
    case PORTA_WESTFALICA:
        // Artikel ist nicht verderblich, Lagerort entsprechend festlegen und verarbeiten
        porta_lager.artikel_liste[porta_lager.anzahl_artikel] = artikeltyp;
        porta_lager.anzahl_artikel++;
        printf("\nDer Artikel wurde erfolgreich in Porta Westfalica eingelagert.\nDruecken Sie Enter, um fortzufahren.\n");
        break;
    default:
        printf("\nUngültige Eingabe. Bitte wählen Sie eine der angegebenen Optionen.");
        break;
    }


    getchar();
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

    // Hinzufügen des neuen Artikels zur Liste
    artikel_liste[anzahl_artikel] = artikeltyp;
    anzahl_artikel++;

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
    printf("\nMoechten Sie den Artikel anhand der Artikelnummer (1) oder des Namens (2) suchen?");
    scanf("%d", &auswahl);

    switch (auswahl) {
    case 1:
        printf("\nBitte geben Sie die Artikelnummer ein:");
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
                    printf("\n(2) Preis");
                    printf("\n(3) Hoehe");
                    printf("\n(4) Breite");
                    printf("\n(5) Tiefe");
                    printf("\n");

                    scanf("%d", &auswahl);
                    switch (auswahl) {
                    case 1:
                        printf("\nBitte geben Sie den neuen Namen ein: ");
                        scanf("%s", artikel_liste[i].name);
                        // Überprüfen, ob der neue Name bereits existiert
                        for (int j = 0; j < anzahl_artikel; j++) {
                            if (j != i && strcmp(artikel_liste[j].name, artikel_liste[i].name) == 0) {
                                printf("\nFehler: Ein Artikel mit diesem Namen existiert bereits.\n");
                                strcpy(artikel_liste[i].name, artikel_liste[j].name); // Ursprünglichen Namen wiederherstellen
                                return 0; // Rückkehr, da Fehler aufgetreten ist
                            }
                        }
                        break;
                    case 2:
                        printf("\nBitte geben Sie den neuen Preis ein: ");
                        scanf("%lf", &artikel_liste[i].preis);
                        break;
                    case 3:
                        printf("\nBitte geben Sie die neue Hoehe ein: ");
                        scanf("%lf", &artikel_liste[i].hoehe);
                        break;
                    case 4:
                        printf("\nBitte geben Sie die neue Breite ein: ");
                        scanf("%lf", &artikel_liste[i].breite);
                        break;
                    case 5:
                        printf("\nBitte geben Sie die neue Tiefe ein: ");
                        scanf("%lf", &artikel_liste[i].tiefe);
                        break;
                    default:
                        printf("\nUngueltige Auswahl.");
                        break;
                    }

                    printf("\nMoechten Sie weitere Details bearbeiten? (1 = Ja, 0 = Nein): ");
                    scanf("%d", &weitere_aenderungen);
                } while (weitere_aenderungen);

                printf("\nSind Sie sicher, dass Sie diese Änderungen speichern möchten? (1 = Ja, 0 = Nein): ");
                scanf("%d", &auswahl);
                if (auswahl == 1) {
                    printf("\nDie Änderungen wurden erfolgreich gespeichert.");
                }
                else {
                    printf("\nDie Änderungen wurden abgebrochen.");
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
        printf("\nBitte geben Sie den Artikelnamen ein:");
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
                    printf("\n(2) Preis");
                    printf("\n(3) Hoehe");
                    printf("\n(4) Breite");
                    printf("\n(5) Tiefe");
                    printf("\n");

                    scanf("%d", &auswahl);
                    switch (auswahl) {
                    case 1:
                        printf("\nBitte geben Sie den neuen Namen ein: ");
                        scanf("%s", artikel_liste[i].name);
                        // Überprüfen, ob der neue Name bereits existiert
                        for (int j = 0; j < anzahl_artikel; j++) {
                            if (j != i && strcmp(artikel_liste[j].name, artikel_liste[i].name) == 0) {
                                printf("\nFehler: Ein Artikel mit diesem Namen existiert bereits.\n");
                                strcpy(artikel_liste[i].name, artikel_liste[j].name); // Ursprünglichen Namen wiederherstellen
                                return 0; // Rückkehr, da Fehler aufgetreten ist
                            }
                        }
                        break;
                    case 2:
                        printf("\nBitte geben Sie den neuen Preis ein: ");
                        scanf("%lf", &artikel_liste[i].preis);
                        break;
                    case 3:
                        printf("\nBitte geben Sie die neue Hoehe ein: ");
                        scanf("%lf", &artikel_liste[i].hoehe);
                        break;
                    case 4:
                        printf("\nBitte geben Sie die neue Breite ein: ");
                        scanf("%lf", &artikel_liste[i].breite);
                        break;
                    case 5:
                        printf("\nBitte geben Sie die neue Tiefe ein: ");
                        scanf("%lf", &artikel_liste[i].tiefe);
                        break;
                    default:
                        printf("\nUngueltige Auswahl.");
                        break;
                    }

                    printf("\nMoechten Sie weitere Details bearbeiten? (1 = Ja, 0 = Nein): ");
                    scanf("%d", &weitere_aenderungen);
                } while (weitere_aenderungen);

                printf("\nSind Sie sicher, dass Sie diese Änderungen speichern möchten? (1 = Ja, 0 = Nein): ");
                scanf("%d", &auswahl);
                if (auswahl == 1) {
                    printf("\nDie Änderungen wurden erfolgreich gespeichert.");
                }
                else {
                    printf("\nDie Änderungen wurden abgebrochen.");
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

            // Überprüfen, ob der Artikel im Hauptartikelarray aktualisiert wurde und die Aktualisierung im Lager durchführen
            for (int j = 0; j < anzahl_artikel; j++) {
                if (artikel_liste[j].art_nummer == artikeltyp.art_nummer) {
                    halle_lager.artikel_liste[i] = artikel_liste[j];
                    break;
                }
            }
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

            // Überprüfen, ob der Artikel im Hauptartikelarray aktualisiert wurde und die Aktualisierung im Lager durchführen
            for (int j = 0; j < anzahl_artikel; j++) {
                if (artikel_liste[j].art_nummer == artikeltyp.art_nummer) {
                    porta_lager.artikel_liste[i] = artikel_liste[j];
                    break;
                }
            }
        }
        break;
    default:
        printf("\nUngültige Eingabe. Bitte wählen Sie eine der angegebenen Optionen.");
        break;
    }

    printf("\n\nDruecke Enter, um zum Hauptmenue zurueckzukehren.");
    while (getchar() != '\n');
    getchar();
}

void bs_loeschen(void)
{
    system("CLS");   // MS-DOS-Kommando
}